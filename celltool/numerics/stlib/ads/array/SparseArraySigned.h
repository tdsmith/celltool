// -*- C++ -*-

/*! 
  \file SparseArraySigned.h
  \brief Sparse array with a signed null value.
*/

#if !defined(__ads_SparseArraySigned_h__)
#define __ads_SparseArraySigned_h__

#include "SparseArray.h"

// If we are debugging the whole ads package.
#if defined(DEBUG_ads) && !defined(DEBUG_ads_SparseArraySigned)
#define DEBUG_ads_SparseArraySigned
#endif

BEGIN_NAMESPACE_ADS


// Note: Doxygen will not pick up this class documentation because it is a 
// declaration and there is no class definition.  Only specializations are
// defined.
//! Sparse array with a signed null value.
/*!
  \param N is the dimension.
  \param T is the value type.
*/
template<int N, typename T = double>
class SparseArraySigned;


//-----------------------------------------------------------------------------
/*! \defgroup ads_array_SparseArraySignedFunctions Free functions for SparseArraySigned. 
  We use the sum(), product(), min() and max() defined for SparseArray.
*/
//@{

//
// Mathematical functions
//


//! Merge the arrays.
template<typename T>
void
merge(const SparseArraySigned<1,T>& a, const SparseArraySigned<1,T>& b, 
      SparseArraySigned<1,T>& c);


//! Remove the unecessary elements.
/*!
 Remove positive elements that do not have an adjacent non-positive neighbor
 and vice-versa.
*/
template<typename T>
void
remove_unecessary_elements(SparseArraySigned<1,T>& a);


//! Compute the union of the arrays.
template<typename T>
void
compute_union(const SparseArraySigned<1,T>& a, 
	      const SparseArraySigned<1,T>& b, 
	      SparseArraySigned<1,T>& c);


//! Compute the intersection of the arrays.
template<typename T>
void
compute_intersection(const SparseArraySigned<1,T>& a, 
		      const SparseArraySigned<1,T>& b, 
		      SparseArraySigned<1,T>& c);


//
// Equality.
//

//! Return true if the arrays are equal.
template<int N, typename T>
inline
bool
operator==(const SparseArraySigned<N,T>& x, const SparseArraySigned<N,T>& y) {
  return x.equal(y);
}

//! Return true if the arrays are not equal.
template<int N, typename T>
inline
bool
operator!=(const SparseArraySigned<N,T>& x, const SparseArraySigned<N,T>& y) {
  return !(x == y);
}

//
// File I/O.
//

//! Write a SparseArraySigned in ascii format.
/*!
  Here is the 1-D file format.  \c size indicates the number of 
  non-null elements.
  \verbatim
  null_value
  sign
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
operator<<(std::ostream& out, const SparseArraySigned<N,T>& x) {
  x.put(out);
  return out;
}


//! Read a SparseArraySigned in ascii format.
/*!
  Here is the 1-D file format.  \c size indicates the number of 
  non-null elements.
  \verbatim
  null_value
  sign
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
operator>>(std::istream& in, SparseArraySigned<N,T>& x) {
  x.get(in);
  return in;
}


//@}


END_NAMESPACE_ADS

#define __ads_SparseArraySigned1_h__
#include "SparseArraySigned1.h"
#undef __ads_SparseArraySigned1_h__

//#define __SparseArraySigned2_ipp__
//#include "SparseArraySigned2.ipp"
//#undef __SparseArraySigned2_ipp__

//#define __SparseArraySigned_ipp__
//#include "SparseArraySigned.ipp"
//#undef __SparseArraySigned_ipp__

#endif
