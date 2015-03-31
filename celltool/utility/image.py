# Copyright 2007 Zachary Pincus
# This file is part of CellTool.
# 
# CellTool is free software; you can redistribute it and/or modify
# it under the terms of version 2 of the GNU General Public License as
# published by the Free Software Foundation.

"""Tools to read and write numpy arrays from and to image files.

This module provides a thin wrapper around the IO functions from the pil_lite
module, which is a highly-modified version of PIL (the Python Image Library)
that has better support for conveting to/from numpy arrays and for 16-bit images.
"""

import pil_lite.Image
import numpy
import warn_tools

_MODE_CONV = {
  "1": (None, None),
  "L": ('|u1', None,),
  "S": ('=u2', None ),
  "I": ('=i4', None ),
  "F": ('=f4', None),
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


def read_array_from_image_file(filename):
  """Read an image from disk and return a numpy array corresponding to that image.
  
  The read-in image is suitably transformed so that pixel (x, y) of the image
  corresponds to array element [x,y]. (The normal __array_interface__ from PIL
  yields the transpose of the 'expected' array.)
  """
  im = pil_lite.Image.open(filename)
  shape, typestr = _conv_type_shape(im)
  if im.mode == "1":
    byte_array = numpy.fromstring(im.tostring(), dtype=numpy.uint8)
    array = numpy.unpackbits(byte_array).astype(bool)
  else:
    array = numpy.fromstring(im.tostring(), dtype=typestr)
  array = numpy.reshape(array, shape).swapaxes(1, 0)
  if im.mode == 'P':
    # try to convert the paletted image to a color image
    if im.palette.mode != 'RGB':
      raise ValueError('Cannot convert an image to an array if the image has a color palette that is not RGB.')
    palette = numpy.array(im.getpalette(), dtype=numpy.uint8).reshape((256, 3))
    array = make_color_array(*[numpy.take(p, array) for p in palette.transpose()]).astype(numpy.uint8)
  return array

def read_grayscale_array_from_image_file(filename, warn = True):
  """Read an image from disk into a 2-D grayscale array, converting from color if necessary.
  
  If 'warn' is True, issue a warning when arrays are converted from color to grayscale.
  """
  image_array = read_array_from_image_file(filename)
  if len(image_array.shape) == 3:
    image_array = make_grayscale_array(image_array)
    if warn:
      warn_tools.warn('Image %s converted from RGB to grayscale: intensity values have been scaled and combined.'%filename)
  return image_array

def write_array_as_image_file(array, filename):
  """Write an array to disk as an image file (with the type determined by the file suffix).
  
  2D arrays of type uint8, uint16, int32 and float32 are supported for TIFF files;
  types uint8 and uint16 are supported for PNG files. Other types typically support
  only uint8 arrays.
  
  3D arrays are treated as RGB or RGBA images (if the third dimension has size
  3 or 4, respectively); typically only uint8 arrays are supported.
  """
  array = numpy.asarray(array).swapaxes(0,1)
  im = pil_lite.Image.fromarray(array)
  im.save(filename)

def make_color_array(r,g,b):
  """Given arrays for the red, green, and blue pixel values, return an (x,y,3)
  array."""
  return numpy.dstack([r,g,b])

def make_grayscale_array(array):
  """Giiven an array of shape (x,y,3) where the last dimension indexes the 
  (r,g,b) pixel value, return a (x,y) grayscale array, where intensity is
  calculated with the ITU-R BT 709 luma transform:
      intensity = 0.2126r + 0.7152g + 0.0722b
  """
  dtype = array.dtype
  new_array = numpy.round((array * [0.2126, 0.7152, 0.0722]).sum(axis = 2))
  return new_array.astype(dtype)
