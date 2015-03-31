// -*- C++ -*-

/*! 
  \file tensor.h
  \brief Includes the tensor classes.
*/

/*!
  \page ads_tensor Tensor Package

  The tensor package has the \c ads::SquareMatrix class which, as the
  name suggests, implements square matrices.  Through template specialization,
  I have implemented optimized versions for 1x1, 2x2 and 3x3 matrices.

  Use this package by including the file tensor.h.
*/

#if !defined(__ads_tensor_h__)
#define __ads_tensor_h__

#include "tensor/SquareMatrix.h"

#endif
