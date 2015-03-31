// -*- C++ -*-

/*! 
  \file unique.h
  \brief Contains the areElementsUnique() functions.
*/

#if !defined(__ads_algorithm_unique_h__)
#define __ads_algorithm_unique_h__

#include "../defs.h"

#include <algorithm>
#include <functional>
#include <iterator>
#include <vector>

BEGIN_NAMESPACE_ADS

//-----------------------------------------------------------------------------
/*! \defgroup algorithm_unique Algorithm: Unique Elements */
// @{

//! Return true if the elements are unique.
/*!
  The elements are not modified.
*/
template<typename InputIterator>
bool
areElementsUnique(InputIterator first, InputIterator last);

//! Return true if the elements are unique.
/*!
  The elements are not modified.
*/
template<typename InputIterator, typename StrictWeakOrdering, 
	 typename BinaryPredicate>
bool
areElementsUnique(InputIterator first, InputIterator last, 
		  StrictWeakOrdering ordering, BinaryPredicate pred);

// @}

END_NAMESPACE_ADS

#define __ads_algorithm_unique_ipp__
#include "unique.ipp"
#undef __ads_algorithm_unique_ipp__

#endif
