#
# The Python Imaging Library.
# $Id: Image.py 2812 2006-10-07 10:08:37Z fredrik $
#
# the Image class wrapper
#
# partial release history:
# 1995-09-09 fl   Created
# 1996-03-11 fl   PIL release 0.0 (proof of concept)
# 1996-04-30 fl   PIL release 0.1b1
# 1999-07-28 fl   PIL release 1.0 final
# 2000-06-07 fl   PIL release 1.1
# 2000-10-20 fl   PIL release 1.1.1
# 2001-05-07 fl   PIL release 1.1.2
# 2002-03-15 fl   PIL release 1.1.3
# 2003-05-10 fl   PIL release 1.1.4
# 2005-03-28 fl   PIL release 1.1.5
# 2006-10-07 fl   PIL release 1.1.6b2
#
# Copyright (c) 1997-2006 by Secret Labs AB.  All rights reserved.
# Copyright (c) 1995-2006 by Fredrik Lundh.
#
# See the README file for information on usage and redistribution.
#

VERSION = "pil-lite"

try:
    import warnings
except ImportError:
    warnings = None

class _imaging_not_installed:
    # module placeholder
    def __getattr__(self, id):
        raise ImportError("The _imaging C module is not installed")

try:
    # If the _imaging C module is not present, you can still use
    # the "open" function to identify files, but you cannot load
    # them.  Note that other modules should not refer to _imaging
    # directly; import Image and use the Image.core variable instead.
    import _imaging
    core = _imaging
    del _imaging
except ImportError, v:
    core = _imaging_not_installed()
    if str(v)[:20] == "Module use of python" and warnings:
        # The _imaging C module is present, but not compiled for
        # the right version (windows only).  Print a warning, if
        # possible.
        warnings.warn(
            "The _imaging extension was built for another version "
            "of Python; most PIL functions will be disabled",
            RuntimeWarning
            )

import ImageMode
import ImagePalette

import os, string, sys

# type stuff
from types import IntType, StringType, TupleType

try:
    UnicodeStringType = type(unicode(""))
    ##
    # (Internal) Checks if an object is a string.  If the current
    # Python version supports Unicode, this checks for both 8-bit
    # and Unicode strings.
    def isStringType(t):
        return isinstance(t, StringType) or isinstance(t, UnicodeStringType)
except NameError:
    def isStringType(t):
        return isinstance(t, StringType)

##
# (Internal) Checks if an object is a tuple.

def isTupleType(t):
    return isinstance(t, TupleType)

##
# (Internal) Checks if an object is an image object.

def isImageType(t):
    return hasattr(t, "im")

##
# (Internal) Checks if an object is a string, and that it points to a
# directory.

def isDirectory(f):
    return isStringType(f) and os.path.isdir(f)

from operator import isNumberType, isSequenceType

#
# Debug level

DEBUG = 0

#
# Constants (also defined in _imagingmodule.c!)

NONE = 0

# palettes/quantizers
WEB = 0
ADAPTIVE = 1

# categories
NORMAL = 0
SEQUENCE = 1
CONTAINER = 2

# --------------------------------------------------------------------
# Registries

ID = []
OPEN = {}
MIME = {}
SAVE = {}
EXTENSION = {}

# --------------------------------------------------------------------
# Modes supported by this version

_MODEINFO = {
    # NOTE: this table will be removed in future versions.  use
    # getmode* functions or ImageMode descriptors instead.

    # official modes
    "1": ("L", "L", ("1",)),
    "L": ("L", "L", ("L",)),
    "I": ("L", "I", ("I",)),
    "F": ("L", "F", ("F",)),
    "P": ("RGB", "L", ("P",)),
    "RGB": ("RGB", "L", ("R", "G", "B")),
    "RGBX": ("RGB", "L", ("R", "G", "B", "X")),
    "RGBA": ("RGB", "L", ("R", "G", "B", "A")),
    "CMYK": ("RGB", "L", ("C", "M", "Y", "K")),
    "YCbCr": ("RGB", "L", ("Y", "Cb", "Cr")),

    # Experimental modes include I;16, I;16B, RGBa, BGR;15,
    # and BGR;24.  Use these modes only if you know exactly
    # what you're doing...

}

if sys.byteorder == 'little':
    _ARRAY_ENDIAN = '<'
else:
    _ARRAY_ENDIAN = '>'

_MODE_CONV = {
    # official modes
    "1": ('t8', None),
    "L": ('|u1', None,),
    "S": ('%su2' % _ARRAY_ENDIAN, None ),
    "I": ('%si4' % _ARRAY_ENDIAN, None ),
    "F": ('%sf4' % _ARRAY_ENDIAN, None),
    "P": ('|u1', None),
    "RGB": ('|u1', 3),
    "RGBX": ('|u1', 4),
    "RGBA": ('|u1', 4),
    "CMYK": ('|u1', 4),
    "YCbCr": ('|u1', 4),
}

def _conv_type_shape(im):
    shape = im.size[::-1]
    typ, extra = _MODE_CONV[im.mode]
    if extra is None:
        return shape, typ
    else:
        return shape+(extra,), typ


MODES = _MODEINFO.keys()
MODES.sort()

# raw modes that may be memory mapped.  NOTE: if you change this, you
# may have to modify the stride calculation in map.c too!
_MAPMODES = ("L", "P", "RGBX", "RGBA", "CMYK", "S")

##
# Gets the "base" mode for given mode.  This function returns "L" for
# images that contain grayscale data, and "RGB" for images that
# contain color data.
#
# @param mode Input mode.
# @return "L" or "RGB".
# @exception KeyError If the input mode was not a standard mode.

def getmodebase(mode):
    return ImageMode.getmode(mode).basemode

##
# Gets the storage type mode.  Given a mode, this function returns a
# single-layer mode suitable for storing individual bands.
#
# @param mode Input mode.
# @return "L", "I", or "F".
# @exception KeyError If the input mode was not a standard mode.

def getmodetype(mode):
    return ImageMode.getmode(mode).basetype

##
# Gets a list of individual band names.  Given a mode, this function
# returns a tuple containing the names of individual bands (use
# {@link #getmodetype} to get the mode used to store each individual
# band.
#
# @param mode Input mode.
# @return A tuple containing band names.  The length of the tuple
#     gives the number of bands in an image of the given mode.
# @exception KeyError If the input mode was not a standard mode.

def getmodebandnames(mode):
    return ImageMode.getmode(mode).bands

##
# Gets the number of individual bands for this mode.
#
# @param mode Input mode.
# @return The number of bands in this mode.
# @exception KeyError If the input mode was not a standard mode.

def getmodebands(mode):
    return len(ImageMode.getmode(mode).bands)

# --------------------------------------------------------------------
# Helpers

_initialized = 0

##
# Explicitly loads standard file format drivers.

def preinit():
    "Load standard file format drivers."

    global _initialized
    if _initialized >= 1:
        return

    try:
        import BmpImagePlugin
    except ImportError:
        pass
    try:
        import GifImagePlugin
    except ImportError:
        pass
    try:
        import JpegImagePlugin
    except ImportError:
        pass
    try:
        import PpmImagePlugin
    except ImportError:
        pass
    try:
        import PngImagePlugin
    except ImportError:
        pass
#   try:
#       import TiffImagePlugin
#   except ImportError:
#       pass

    _initialized = 1

##
# Explicitly initializes the Python Imaging Library.  This function
# loads all available file format drivers.

def init():
    "Load all file format drivers."

    global _initialized
    if _initialized >= 2:
        return

    visited = {}

    directories = sys.path

    try:
        directories = directories + [os.path.dirname(__file__)]
    except NameError:
        pass

    # only check directories (including current, if present in the path)
    for directory in filter(isDirectory, directories):
        fullpath = os.path.abspath(directory)
        if visited.has_key(fullpath):
            continue
        for file in os.listdir(directory):
            if file[-14:] == "ImagePlugin.py":
                f, e = os.path.splitext(file)
                try:
                    sys.path.insert(0, directory)
                    try:
                        __import__(f, globals(), locals(), [])
                    finally:
                        del sys.path[0]
                except ImportError:
                    if DEBUG:
                        print "Image: failed to import",
                        print f, ":", sys.exc_value
        visited[fullpath] = None

    if OPEN or SAVE:
        _initialized = 2


# --------------------------------------------------------------------
# Codec factories (used by tostring/fromstring and ImageFile.load)

def _getdecoder(mode, decoder_name, args, extra=()):

    # tweak arguments
    if args is None:
        args = ()
    elif not isTupleType(args):
        args = (args,)

    try:
        # get decoder
        decoder = getattr(core, decoder_name + "_decoder")
        # print decoder, (mode,) + args + extra
        return apply(decoder, (mode,) + args + extra)
    except AttributeError:
        raise IOError("decoder %s not available" % decoder_name)

def _getencoder(mode, encoder_name, args, extra=()):

    # tweak arguments
    if args is None:
        args = ()
    elif not isTupleType(args):
        args = (args,)

    try:
        # get encoder
        encoder = getattr(core, encoder_name + "_encoder")
        # print encoder, (mode,) + args + extra
        return apply(encoder, (mode,) + args + extra)
    except AttributeError:
        raise IOError("encoder %s not available" % encoder_name)



# --------------------------------------------------------------------
# Implementation wrapper

##
# This class represents an image object.  To create Image objects, use
# the appropriate factory functions.  There's hardly ever any reason
# to call the Image constructor directly.
#
# @see #open
# @see #new
# @see #fromstring

class Image:

    format = None
    format_description = None

    def __init__(self):
        self.im = None
        self.mode = ""
        self.size = (0, 0)
        self.palette = None
        self.info = {}
        self.category = NORMAL
        self.readonly = 0

    def _new(self, im):
        new = Image()
        new.im = im
        new.mode = im.mode
        new.size = im.size
        new.palette = self.palette
        if im.mode == "P":
            new.palette = ImagePalette.ImagePalette()
        try:
            new.info = self.info.copy()
        except AttributeError:
            # fallback (pre-1.5.2)
            new.info = {}
            for k, v in self.info:
                new.info[k] = v
        return new

    _makeself = _new # compatibility

    def _copy(self):
        self.load()
        self.im = self.im.copy()
        self.readonly = 0

    def _dump(self, file=None, format='png'):
        import tempfile
        if not file:
            file = tempfile.mktemp()
        self.load()
        file = file + "." + format
        self.save(file, format)
        return file

    def __getattr__(self, name):
        if name == "__array_interface__":
            # numpy array interface support
            new = {}
            shape, typestr = _conv_type_shape(self)
            new['shape'] = shape
            new['typestr'] = typestr
            new['data'] = self.tostring()
            return new
        raise AttributeError(name)

    ##
    # Returns a string containing pixel data.
    #
    # @param encoder_name What encoder to use.  The default is to
    #    use the standard "raw" encoder.
    # @param *args Extra arguments to the encoder.
    # @return An 8-bit string.

    def tostring(self, encoder_name="raw", *args):
        "Return image as a binary string"

        # may pass tuple instead of argument list
        if len(args) == 1 and isTupleType(args[0]):
            args = args[0]

        if encoder_name == "raw" and args == ():
            args = self.mode

        self.load()

        # unpack data
        e = _getencoder(self.mode, encoder_name, args)
        e.setimage(self.im)

        bufsize = max(65536, self.size[0] * 4) # see RawEncode.c

        data = []
        while 1:
            l, s, d = e.encode(bufsize)
            data.append(d)
            if s:
                break
        if s < 0:
            raise RuntimeError("encoder error %d in tostring" % s)

        return string.join(data, "")

    ##
    # Loads this image with pixel data from a string.
    # <p>
    # This method is similar to the {@link #fromstring} function, but
    # loads data into this image instead of creating a new image
    # object.

    def fromstring(self, data, decoder_name="raw", *args):
        "Load data to image from binary string"

        # may pass tuple instead of argument list
        if len(args) == 1 and isTupleType(args[0]):
            args = args[0]

        # default format
        if decoder_name == "raw" and args == ():
            args = self.mode

        # unpack data
        d = _getdecoder(self.mode, decoder_name, args)
        d.setimage(self.im)
        s = d.decode(data)

        if s[0] >= 0:
            raise ValueError("not enough image data")
        if s[1] != 0:
            raise ValueError("cannot decode image data")

    ##
    # Allocates storage for the image and loads the pixel data.  In
    # normal cases, you don't need to call this method, since the
    # Image class automatically loads an opened image when it is
    # accessed for the first time.
    #

    def load(self):
        "Explicitly load pixel data."
        if self.im and self.palette and self.palette.dirty:
            # realize palette
            apply(self.im.putpalette, self.palette.getdata())
            self.palette.dirty = 0
            self.palette.mode = "RGB"
            self.palette.rawmode = None
            if self.info.has_key("transparency"):
                self.im.putpalettealpha(self.info["transparency"], 0)
                self.palette.mode = "RGBA"

    ##
    # Verifies the contents of a file. For data read from a file, this
    # method attempts to determine if the file is broken, without
    # actually decoding the image data.  If this method finds any
    # problems, it raises suitable exceptions.  If you need to load
    # the image after using this method, you must reopen the image
    # file.

    def verify(self):
        "Verify file contents."
        pass

    ##
    # Copies this image. Use this method if you wish to paste things
    # into an image, but still retain the original.
    #
    # @return An Image object.

    def copy(self):
        "Copy raster data"

        self.load()
        im = self.im.copy()
        return self._new(im)

    ##
    # Configures the image file loader so it returns a version of the
    # image that as closely as possible matches the given mode and
    # size.  For example, you can use this method to convert a colour
    # JPEG to greyscale while loading it, or to extract a 128x192
    # version from a PCD file.
    # <p>
    # Note that this method modifies the Image object in place.  If
    # the image has already been loaded, this method has no effect.
    #
    # @param mode The requested mode.
    # @param size The requested size.

    def draft(self, mode, size):
        "Configure image decoder"

        pass

    ##
    # Returns a tuple containing the name of each band in this image.
    # For example, <b>getbands</b> on an RGB image returns ("R", "G", "B").
    #
    # @return A tuple containing band names.

    def getbands(self):
        "Get band names"

        return ImageMode.getmode(self.mode).bands

    ##
    # Returns a PyCObject that points to the internal image memory.
    #
    # @return A PyCObject object.

    def getim(self):
        "Get PyCObject pointer to internal image memory"

        self.load()
        return self.im.ptr

    ##
    # Returns the image palette as a list.
    #
    # @return A list of color values [r, g, b, ...], or None if the
    #    image has no palette.

    def getpalette(self):
        "Get palette contents."

        self.load()
        try:
            return map(ord, self.im.getpalette())
        except ValueError:
            return None # no palette

    ##
    # Adds or replaces the alpha layer in this image.  If the image
    # does not have an alpha layer, it's converted to "LA" or "RGBA".
    # The new layer must be either "L" or "1".
    #
    # @param im The new alpha layer.  This can either be an "L" or "1"
    #    image having the same size as this image, or an integer or
    #    other color value.

    def putalpha(self, alpha):
        "Set alpha layer"

        self.load()
        if self.readonly:
            self._copy()

        if self.mode not in ("LA", "RGBA"):
            # attempt to promote self to a matching alpha mode
            try:
                mode = getmodebase(self.mode) + "A"
                try:
                    self.im.setmode(mode)
                except (AttributeError, ValueError):
                    # do things the hard way
                    im = self.im.convert(mode)
                    if im.mode not in ("LA", "RGBA"):
                        raise ValueError # sanity check
                    self.im = im
                self.mode = self.im.mode
            except (KeyError, ValueError):
                raise ValueError("illegal image mode")

        if self.mode == "LA":
            band = 1
        else:
            band = 3

        if isImageType(alpha):
            # alpha layer
            if alpha.mode not in ("1", "L"):
                raise ValueError("illegal image mode")
            alpha.load()
            if alpha.mode == "1":
                alpha = alpha.convert("L")
        else:
            # constant alpha
            try:
                self.im.fillband(band, alpha)
            except (AttributeError, ValueError):
                # do things the hard way
                alpha = new("L", self.size, alpha)
            else:
                return

        self.im.putband(alpha.im, band)

    ##
    # Attaches a palette to this image.  The image must be a "P" or
    # "L" image, and the palette sequence must contain 768 integer
    # values, where each group of three values represent the red,
    # green, and blue values for the corresponding pixel
    # index. Instead of an integer sequence, you can use an 8-bit
    # string.
    #
    # @def putpalette(data)
    # @param data A palette sequence (either a list or a string).

    def putpalette(self, data, rawmode="RGB"):
        "Put palette data into an image."

        self.load()
        if self.mode not in ("L", "P"):
            raise ValueError("illegal image mode")
        if not isStringType(data):
            data = string.join(map(chr, data), "")
        self.mode = "P"
        self.palette = ImagePalette.raw(rawmode, data)
        self.palette.mode = "RGB"
        self.load() # install new palette

    ##
    # Saves this image under the given filename.  If no format is
    # specified, the format to use is determined from the filename
    # extension, if possible.
    # <p>
    # Keyword options can be used to provide additional instructions
    # to the writer. If a writer doesn't recognise an option, it is
    # silently ignored. The available options are described later in
    # this handbook.
    # <p>
    # You can use a file object instead of a filename. In this case,
    # you must always specify the format. The file object must
    # implement the <b>seek</b>, <b>tell</b>, and <b>write</b>
    # methods, and be opened in binary mode.
    #
    # @def save(file, format=None, **options)
    # @param file File name or file object.
    # @param format Optional format override.  If omitted, the
    #    format to use is determined from the filename extension.
    #    If a file object was used instead of a filename, this
    #    parameter should always be used.
    # @param **options Extra parameters to the image writer.
    # @return None
    # @exception KeyError If the output format could not be determined
    #    from the file name.  Use the format option to solve this.
    # @exception IOError If the file could not be written.  The file
    #    may have been created, and may contain partial data.

    def save(self, fp, format=None, **params):
        "Save image to file or stream"

        if isStringType(fp):
            filename = fp
        else:
            if hasattr(fp, "name") and isStringType(fp.name):
                filename = fp.name
            else:
                filename = ""

        # may mutate self!
        self.load()

        self.encoderinfo = params
        self.encoderconfig = ()

        preinit()

        ext = string.lower(os.path.splitext(filename)[1])

        if not format:
            try:
                format = EXTENSION[ext]
            except KeyError:
                init()
                try:
                    format = EXTENSION[ext]
                except KeyError:
                    raise KeyError(ext) # unknown extension

        try:
            save_handler = SAVE[string.upper(format)]
        except KeyError:
            init()
            save_handler = SAVE[string.upper(format)] # unknown format

        if isStringType(fp):
            import __builtin__
            fp = __builtin__.open(fp, "wb")
            close = 1
        else:
            close = 0

        try:
            save_handler(self, fp, filename)
        finally:
            # do what we can to clean up
            if close:
                fp.close()

    ##
    # Seeks to the given frame in this sequence file. If you seek
    # beyond the end of the sequence, the method raises an
    # <b>EOFError</b> exception. When a sequence file is opened, the
    # library automatically seeks to frame 0.
    # <p>
    # Note that in the current version of the library, most sequence
    # formats only allows you to seek to the next frame.
    #
    # @param frame Frame number, starting at 0.
    # @exception EOFError If the call attempts to seek beyond the end
    #     of the sequence.
    # @see #Image.tell

    def seek(self, frame):
        "Seek to given frame in sequence file"

        # overridden by file handlers
        if frame != 0:
            raise EOFError

    ##
    # Split this image into individual bands. This method returns a
    # tuple of individual image bands from an image. For example,
    # splitting an "RGB" image creates three new images each
    # containing a copy of one of the original bands (red, green,
    # blue).
    #
    # @return A tuple containing bands.

    def split(self):
        "Split image into bands"

        ims = []
        self.load()
        for i in range(self.im.bands):
            ims.append(self._new(self.im.getband(i)))
        return tuple(ims)

    ##
    # Returns the current frame number.
    #
    # @return Frame number, starting with 0.
    # @see #Image.seek

    def tell(self):
        "Return current frame number"

        return 0


##
# Creates a new image with the given mode and size.
#
# @param mode The mode to use for the new image.
# @param size A 2-tuple, containing (width, height) in pixels.
# @return An Image object.

def new(mode, size):
    "Create a new image"
    return Image()._new(core.new(mode, size))

##
# Creates an image memory from pixel data in a string.
# <p>
# In its simplest form, this function takes three arguments
# (mode, size, and unpacked pixel data).
# <p>
# You can also use any pixel decoder supported by PIL.  For more
# information on available decoders, see the section <a
# href="pil-decoder.htm"><i>Writing Your Own File Decoder</i></a>.
# <p>
# Note that this function decodes pixel data only, not entire images.
# If you have an entire image in a string, wrap it in a
# <b>StringIO</b> object, and use {@link #open} to load it.
#
# @param mode The image mode.
# @param size The image size.
# @param data An 8-bit string containing raw data for the given mode.
# @param decoder_name What decoder to use.
# @param *args Additional parameters for the given decoder.
# @return An Image object.

def fromstring(mode, size, data, decoder_name="raw", *args):
    "Load image from string"

    # may pass tuple instead of argument list
    if len(args) == 1 and isTupleType(args[0]):
        args = args[0]

    if decoder_name == "raw" and args == ():
        args = mode

    im = new(mode, size)
    im.fromstring(data, decoder_name, args)
    return im

##
# (New in 1.1.4) Creates an image memory from pixel data in a string
# or byte buffer.
# <p>
# This function is similar to {@link #fromstring}, but uses data in
# the byte buffer, where possible.  This means that changes to the
# original buffer object are reflected in this image).  Not all modes
# can share memory; supported modes include "L", "RGBX", "RGBA", and
# "CMYK".
# <p>
# Note that this function decodes pixel data only, not entire images.
# If you have an entire image file in a string, wrap it in a
# <b>StringIO</b> object, and use {@link #open} to load it.
# <p>
# In the current version, the default parameters used for the "raw"
# decoder differs from that used for {@link fromstring}.  This is a
# bug, and will probably be fixed in a future release.  The current
# release issues a warning if you do this; to disable the warning,
# you should provide the full set of parameters.  See below for
# details.
#
# @param mode The image mode.
# @param size The image size.
# @param data An 8-bit string or other buffer object containing raw
#     data for the given mode.
# @param decoder_name What decoder to use.
# @param *args Additional parameters for the given decoder.  For the
#     default encoder ("raw"), it's recommended that you provide the
#     full set of parameters:
#     <b>frombuffer(mode, size, data, "raw", mode, 0, 1)</b>.
# @return An Image object.
# @since 1.1.4

def frombuffer(mode, size, data, decoder_name="raw", *args):
    "Load image from string or buffer"

    # may pass tuple instead of argument list
    if len(args) == 1 and isTupleType(args[0]):
        args = args[0]

    if decoder_name == "raw":
        if args == ():
            if warnings:
                warnings.warn(
                    "the frombuffer defaults may change in a future release; "
                    "for portability, change the call to read:\n"
                    "  frombuffer(mode, size, data, 'raw', mode, 0, 1)",
                    RuntimeWarning, stacklevel=2
                )
            args = mode, 0, -1 # may change to (mode, 0, 1) post-1.1.6
        if args[0] in _MAPMODES:
            im = new(mode, (1,1))
            im = im._new(
                core.map_buffer(data, size, decoder_name, None, 0, args)
                )
            im.readonly = 1
            return im

    return apply(fromstring, (mode, size, data, decoder_name, args))


##
# (New in 1.1.6) Create an image memory from an object exporting
# the array interface (using the buffer protocol).
#
# If obj is not contiguous, then the tostring method is called
# and {@link frombuffer} is used.
#
# @param obj Object with array interface
# @return An image memory.

_mode_dict = {
  'u1' : 'L',
  'u2' : 'S',
  'b1' : '1',
  'i4' : 'I',
  'f4' : 'F',
}

def fromarray(obj):
    arr = obj.__array_interface__
    shape = arr['shape']
    ndim = len(shape)
    if ndim < 2 or ndim > 3:
      raise TypeError("Only 2 and 3-dimensional arrays can be converted to images.")
    try:
        strides = arr['strides']
    except KeyError:
        strides = None
    typestr = arr['typestr']
    endian = typestr[0]
    type_bits = typestr[1:]
    try:
      mode = _mode_dict[type_bits]
    except:
      raise TypeError("Only arrays of type uint8, uint16, bool, int32, and float32 can be converted to images.")
    if ndim == 3:
      if mode != 'L':
        raise TypeError("Can only unpack 8-bit values into LA/RGB/RGBA images.")
      if shape[2] == 2:
        mode = 'LA'
      elif shape[2] == 3:
        mode = 'RGB'
      elif shape[2] == 4:
        mode = 'RGBA'
      else:
        raise TypeError("Can only unpack LA/RGB/RGBA images")
    size = shape[:2][::-1]
    if mode == '1':
        import numpy
        obj = numpy.packbits(obj.astype(numpy.uint8).flat).tostring()
    elif strides is not None:
        obj = obj.tostring()
    return frombuffer(mode, size, obj, "raw", mode, 0, 1)

##
# Opens and identifies the given image file.
# <p>
# This is a lazy operation; this function identifies the file, but the
# actual image data is not read from the file until you try to process
# the data (or call the {@link #Image.load} method).
#
# @def open(file, mode="r")
# @param file A filename (string) or a file object.  The file object
#    must implement <b>read</b>, <b>seek</b>, and <b>tell</b> methods,
#    and be opened in binary mode.
# @param mode The mode.  If given, this argument must be "r".
# @return An Image object.
# @exception IOError If the file cannot be found, or the image cannot be
#    opened and identified.
# @see #new

def open(fp, mode="r"):
    "Open an image file, without loading the raster data"

    if mode != "r":
        raise ValueError("bad mode")

    if isStringType(fp):
        import __builtin__
        filename = fp
        fp = __builtin__.open(fp, "rb")
    else:
        filename = ""

    prefix = fp.read(16)

    preinit()

    for i in ID:
        try:
            factory, accept = OPEN[i]
            if not accept or accept(prefix):
                fp.seek(0)
                return factory(fp, filename)
        except (SyntaxError, IndexError, TypeError):
            pass

    init()

    for i in ID:
        try:
            factory, accept = OPEN[i]
            if not accept or accept(prefix):
                fp.seek(0)
                return factory(fp, filename)
        except (SyntaxError, IndexError, TypeError):
            pass

    raise IOError("cannot identify image file")

# --------------------------------------------------------------------
# Plugin registry

##
# Register an image file plugin.  This function should not be used
# in application code.
#
# @param id An image format identifier.
# @param factory An image file factory method.
# @param accept An optional function that can be used to quickly
#    reject images having another format.

def register_open(id, factory, accept=None):
    id = string.upper(id)
    ID.append(id)
    OPEN[id] = factory, accept

##
# Registers an image MIME type.  This function should not be used
# in application code.
#
# @param id An image format identifier.
# @param mimetype The image MIME type for this format.

def register_mime(id, mimetype):
    MIME[string.upper(id)] = mimetype

##
# Registers an image save function.  This function should not be
# used in application code.
#
# @param id An image format identifier.
# @param driver A function to save images in this format.

def register_save(id, driver):
    SAVE[string.upper(id)] = driver

##
# Registers an image extension.  This function should not be
# used in application code.
#
# @param id An image format identifier.
# @param extension An extension used for this format.

def register_extension(id, extension):
    EXTENSION[string.lower(extension)] = string.upper(id)
