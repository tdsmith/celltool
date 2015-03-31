#!/usr/bin/env python

# Copyright 2007 Zachary Pincus
# This file is part of CellTool.
# 
# CellTool is free software; you can redistribute it and/or modify
# it under the terms of version 2 of the GNU General Public License as
# published by the Free Software Foundation.
import sys
try:
  import py2exe
  have_py2exe = True
except:
  have_py2exe = False

def configuration(parent_package='',top_path=None):
  from numpy.distutils.misc_util import Configuration
  config = Configuration(None, parent_package, top_path)
  config.set_options(ignore_setup_xxx_py=True,
                      assume_default_configuration=True,
                      delegate_options_to_subpackages=True,
                      quiet=True)

  config.add_subpackage('celltool')
  return config

def setup_package():
  from numpy.distutils.core import setup
  setup_kws = {}
  if have_py2exe:
    sys.path.insert(0, './build/lib.win32-%d.%d'%(sys.version_info[0], sys.version_info[1]))
    setup_kws['console'] = ['celltool/command_line/celltool']
    setup_kws['options'] = {'py2exe':{'packages':['celltool', 'numpy.numarray']}}
  setup(
      name = 'celltool',
      version = '2.0',
      maintainer = "Zachary Pincus",
      maintainer_email = "zpincus at stanford . edu",
      description = "Tools for performing shape analysis of images.",
      url = "none yet",
      license = 'GPL',
      configuration=configuration,
      **setup_kws)

if __name__ == '__main__':
    setup_package()