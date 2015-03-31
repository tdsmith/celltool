# Copyright 2007 Zachary Pincus
# This file is part of CellTool.
# 
# CellTool is free software; you can redistribute it and/or modify
# it under the terms of version 2 of the GNU General Public License as
# published by the Free Software Foundation.

import os, numpy

def configuration(parent_package='',top_path=None):
    from numpy.distutils.misc_util import Configuration
    config = Configuration('numerics',parent_package,top_path)
    
    config.add_extension("_find_contours",
      sources=["_find_contoursmodule.c"],
      include_dirs=numpy.get_include() )
    
    config.add_extension("_closest_point_transform",
      sources=["_closest_point_transformmodule.cpp"],
      include_dirs=['stlib', numpy.get_include()],
      extra_compile_args=["-fpermissive"])
      
    config.add_subpackage('ndimage')
    config.add_subpackage('fitpack')
    return config

if __name__ == '__main__':
    from numpy.distutils.core import setup
    setup(**configuration(top_path='').todict())
