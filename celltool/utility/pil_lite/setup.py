import os, numpy, sys, struct
import numpy.distutils

# put (library_dir, include_dir) tuple here for custom libjpeg and libz locations
JPEG_ROOT = None
ZLIB_ROOT = ('pc-dist', 'pc-dist')

def add_directory(path, dir):
  if dir and os.path.isdir(dir) and dir not in path:
    path.append(dir)

def find_library_file(library, lib_dirs, include, include_dirs, info):
  lib_dir = None
  for d in lib_dirs:
    r = info.check_libs(d, [library])
    if r is not None:
      lib_dir = r['library_dirs']
      break
  if lib_dir is None:
    return
  
  include_dir = None
  for d in include_dirs:
    if os.path.isfile(os.path.join(d, include)):
      include_dir = d
      break
  if include_dir is None:
    return
  
  return (lib_dir, include_dir)

def configuration(parent_package='',top_path=None):
    from numpy.distutils.misc_util import Configuration
    config = Configuration('pil_lite',parent_package,top_path, 'PIL')
    
    from numpy.distutils import system_info
    info = system_info.system_info()
    library_search_dirs = info.get_lib_dirs()
    include_search_dirs = info.get_include_dirs()
    
    for root in [JPEG_ROOT, ZLIB_ROOT]:
      if root is not None:
        lib_root, include_root = root
        add_directory(library_search_dirs, lib_root)
        add_directory(include_search_dirs, include_root)
    
    imaging_library_dirs = []
    imaging_include_dirs = ['libImaging']
    imaging_libraries = []
    imaging_defines = []
    
    def add_lib(library, include, macro):
      ret = find_library_file(library, library_search_dirs, include, include_search_dirs, info)
      if ret is not None:
        imaging_libraries.append(library)
        imaging_library_dirs.append(ret[0])
        imaging_include_dirs.append(ret[1])
        imaging_defines.append((macro, None))
        return True
      else:
        return False
    
    z = add_lib('z', 'zlib.h', 'HAVE_LIBZ')
    if not z and sys.platform == 'win32':
      z = add_lib('zlib', 'zlib.h', 'HAVE_LIBZ')
    if z:
      numpy.distutils.log.info('Libz found.')
    else:
      numpy.distutils.log.info('Libz not found.')
    
    jpeg = add_lib('jpeg', 'jpeglib.h', 'HAVE_LIBJPEG')
    if jpeg:
      numpy.distutils.log.info('Libjpeg found.')
    else:
      numpy.distutils.log.info('Libjpeg not found.')
    
    if sys.platform == "win32":
        imaging_libraries.extend(["kernel32", "user32"])
    if struct.unpack("h", "\0\1")[0] == 1:
        imaging_defines.append(("WORDS_BIGENDIAN", None))
        numpy.distutils.log.info('Detected big-endian system.')
    else:
      numpy.distutils.log.info('Detected little-endian system.')
    
    config.add_extension('_imaging',
      sources=['*.c', os.path.join('libImaging', '*.c')],
      include_dirs=imaging_include_dirs,
      library_dirs=imaging_library_dirs,
      libraries=imaging_libraries,
      define_macros=imaging_defines)
    
    return config


if __name__ == '__main__':
    from numpy.distutils.core import setup
    setup(**configuration(top_path='').todict())
