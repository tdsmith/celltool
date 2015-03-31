# Copyright 2007 Zachary Pincus
# This file is part of CellTool.
# 
# CellTool is free software; you can redistribute it and/or modify
# it under the terms of version 2 of the GNU General Public License as
# published by the Free Software Foundation.

import numpy

def spatial_filter(array, period_range, spacing = 1.0, order = 2, keep_dc = False):
  """Filter the given array with a butterworth filter of the specified order.
  
  The 'period_range' parameter is a (min_size, max_size) tuple representing
  the minimum and maximum spatial size of objects in the filter's pass-band.
  Objects smaller than min_size and larger than max_size will be filtered 
  out of the image. Either value can be None, indicating that there should be
  no size limit at that end of the scale.
  
  The 'spacing' parameter allows the physical spacing between array elements 
  to be specified, so that the size values (above) can be given in physical
  units. This parameter can either be a scalar or a list of spacings in each
  dimension.
  
  The 'order' parameter controls the order of the butterworth filter. Higher
  order filters remove more of the structure in the stop-bands (specifically, 
  those near the transition between pass-band and stop-band), but may introduce
  ringing artifacts.
  
  If the 'keep_dc' parameter is True, then the DC component of the array will
  not be removed; otherwise it will. (If the DC component is removed, then the
  average array value will be zero; otherwise the average value will remain
  unchanged.)  
  """
  array = numpy.asarray(array)
  low_cutoff_period, high_cutoff_period = period_range
  if (high_cutoff_period is not None and high_cutoff_period < 2) or (low_cutoff_period is not None and low_cutoff_period < 2):
    raise ValueError('Period cutoffs must be either None, or values greater than 2 (the Nyquist period).')
  if low_cutoff_period is None and high_cutoff_period is None:
    return array
  elif low_cutoff_period is None:
    filter = highpass_butterworth_nd(1.0 / high_cutoff_period, array.shape, spacing, order)
  elif high_cutoff_period is None:
    filter = lowpass_butterworth_nd(1.0 / low_cutoff_period, array.shape, spacing, order)
  else:
    filter = bandpass_butterworth_nd(1.0 / high_cutoff_period, 1.0 / low_cutoff_period, array.shape, spacing, order)
  if keep_dc:
    filter.flat[0] = 1
  else:
    filter.flat[0] = 0
  return filter_nd(array, filter).real

def lowpass_butterworth_nd(cutoff, shape, d = 1.0, order = 2):
  """Create a low-pass butterworth filter with the given pass-band and 
  n-dimensional shape. The 'd' parameter is a scalar or list giving the sample
  spacing in all/each dimension, and the 'order' parameter controls the order
  of the butterworth filter.  
  """
  cutoff = float(cutoff)
  if cutoff > 0.5:
    raise ValueError('Filter cutoff frequency must be <= 0.5.')
  return 1.0 / (1.0 + (fftfreq_nd(shape, d) / cutoff)**(2*order))

def highpass_butterworth_nd(cutoff, shape, d = 1.0, order = 2):
  """Create a high-pass butterworth filter with the given pass-band and 
  n-dimensional shape. The 'd' parameter is a scalar or list giving the sample
  spacing in all/each dimension, and the 'order' parameter controls the order
  of the butterworth filter.  
  """
  return 1 - lowpass_butterworth_nd(cutoff, shape, d, order)

def bandpass_butterworth_nd(low_cutoff, high_cutoff, shape, d = 1.0, order = 2):
  """Create a band-pass butterworth filter with the given pass-band and 
  n-dimensional shape. The 'd' parameter is a scalar or list giving the sample
  spacing in all/each dimension, and the 'order' parameter controls the order
  of the butterworth filter.  
  """
  return lowpass_butterworth_nd(high_cutoff, shape, d, order) * highpass_butterworth_nd(low_cutoff, shape, d, order)

def filter_nd(array, filter):
  """Filter an array's fft with a the given filter coefficients."""
  array, filter = numpy.asarray(array), numpy.asarray(filter)
  fft = numpy.fft.fftn(array)
  filtered = fft * filter
  return numpy.fft.ifftn(filtered)

def fftfreq_nd(shape, d = 1.0):
  """Return an array containing the frequency bins of an N-dimensional FFT.
  Parameter 'd' specifies the sample spacing."""
  nd = len(shape)
  ds = numpy.resize(d, nd)
  freqs1d = [numpy.linspace(0,1,n, endpoint=False)/d for n,d in zip(shape, ds)]
  freqs1d = [numpy.where(freqs < 0.5, freqs, freqs-1) for freqs in freqs1d]
  s = numpy.ones(shape)
  freqsnd = [numpy.swapaxes(freqs * numpy.swapaxes(s, i, nd-1), i, nd-1) for i,freqs in enumerate(freqs1d)]
  freqsnd = numpy.sqrt(numpy.sum(numpy.square(freqsnd), axis = 0))
  return freqsnd