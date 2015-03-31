// -*- C++ -*-

/*! 
  \file SparseArray.h
  \brief A class for an N-D sparse array.
*/

#if !defined(__ads_SparseArray_h__)
#define __ads_SparseArray_h__

#include "Array.h"

#include "../iterator/TransformIterator.h"

// If we are debugging the whole ads package.
#if defined(DEBUG_ads) && !defined(DEBUG_SparseArray)
#define DEBUG_SparseArray
#endif

BEGIN_NAMESPACE_ADS

// Note: Doxygen will not pick up this class documentation because it is a 
// declaration and there is no class definition.  Only specializations are
// defined.
//! A sparse multi-array of type T in N dimensions.
/*!
  \param N is the dimension.
  \param T is the value type.
*/
template<int N, typename T = double>
class SparseArray;


//-----------------------------------------------------------------------------
/*! \defgroup ads_array_SparseArrayFunctions Free functions for SparseArray. */
//@{

// CONTINUE: I think I can remove these since I have functions defined for
// ArrayContainer's.
#if 0
//
// Mathematical functions
//

//! Return the sum of the non-null components.
template<int N, typename T>
inline
T
computeSum(const SparseArray<N,T>& x) {
  return std::accumulate(x.begin(), x.end(), T(0));
}
    
//! Return the product of the non-null components.
template<int N, typename T>
inline
T
computeProduct(const SparseArray<N,T>& x) {
  return std::accumulate(x.begin(), x.end(), T(1), std::multiplies<T>());
}

//! Return the minimum non-null component.  Use < for comparison.
template<int N, typename T>
inline
T
computeMinimum(const SparseArray<N,T>& x) {
  return *std::min_element(x.begin(), x.end());
}
    
//! Return the maximum non-null component.  Use > for comparison.
template<int N, typename T>
inline
T
computeMaximum(const SparseArray<N,T>& x) {
  return *std::max_element(x.begin(), x.end());
}

#endif

//
// Equality.
//

// CONTINUE: I don't think I need these because I have the member functions.
#if 0
//! Return true if the arrays are equal.
template<int N, typename T>
inline
bool
operator==(const SparseArray<N,T>& x, const SparseArray<N,T>& y) {
  return x.isEqual(y);
}

//! Return true if the arrays are not equal.
template<int N, typename T>
inline
bool
operator!=(const SparseArray<N,T>& x, const SparseArray<N,T>& y) {
  return !(x == y);
}
#endif

//
// File I/O.
//

//! Write a SparseArray in ascii format.
/*!
  Here is the 1-D file format.  \c size indicates the number of 
  non-null elements.
  \verbatim
  null_value
  size
  index[0]
  ...
  index[size-1]
  value[0]
  ...
  value[size-1] \endverbatim
*/
template<int N, typename T>
inline
std::ostream&
operator<<(std::ostream& out, const SparseArray<N,T>& x) {
  x.put(out);
  return out;
}

//! Read a SparseArray in ascii format.
/*!
  Here is the 1-D file format.  \c size indicates the number of 
  non-null elements.
  \verbatim
  null_value
  size
  index[0]
  ...
  index[size-1]
  value[0]
  ...
  value[size-1] \endverbatim
*/
template<int N, typename T>
inline
std::istream&
operator>>(std::istream& in, SparseArray<N,T>& x) {
  x.get(in);
  return in;
}

//@}

END_NAMESPACE_ADS

#define __ads_SparseArray1_h__
#include "SparseArray1.h"
#undef __ads_SparseArray1_h__

#define __ads_SparseArray2_h__
#include "SparseArray2.h"
#undef __ads_SparseArray2_h__

#endif
