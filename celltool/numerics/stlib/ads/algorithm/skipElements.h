// -*- C++ -*-

/*! 
  \file skipElements.h
  \brief Contains the areElementsSkipElements() functions.
*/

#if !defined(__ads_algorithm_skipElements_h__)
#define __ads_algorithm_skipElements_h__

#include "../defs.h"

BEGIN_NAMESPACE_ADS

//-----------------------------------------------------------------------------
/*! \defgroup ads_algorithm_skipElements Algorithm: Skip elements in a sequence. */
// @{


//! Advance the iterator while it's value is equal to any of the elements in the range.  Return the advanced iterator.
/*!
  This function uses iteration to skip the elements.  This is only efficient
  if the the size of the sequence [beginning .. end) is small.
*/
template<typename ForwardIterator1, typename ForwardIterator2>
ForwardIterator1
skipElementsUsingIteration(ForwardIterator1 iterator,
			   ForwardIterator2 beginning,
			   ForwardIterator2 end);


//! Advance the iterator while it is equal to any of the elements in the range.  Return the advanced iterator.
/*!
  This function uses iteration to skip the elements.  This is only efficient
  if the the size of the sequence [beginning .. end) is small.
*/
template<typename ForwardIterator, typename IteratorForwardIterator>
ForwardIterator
skipIteratorsUsingIteration(ForwardIterator iterator,
			    IteratorForwardIterator beginning,
			    IteratorForwardIterator end);



// CONTINUE: Add a skip*UsingSet function.

// @}

END_NAMESPACE_ADS

#define __ads_algorithm_skipElements_ipp__
#include "skipElements.ipp"
#undef __ads_algorithm_skipElements_ipp__

#endif
