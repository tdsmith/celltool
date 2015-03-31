import numpy

try:
  unique = numpy.unique
except AttributeError:
  unique = numpy.unique1d