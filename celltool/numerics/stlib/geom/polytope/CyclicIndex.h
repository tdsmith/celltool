// -*- C++ -*-

#if !defined(__geom_CyclicIndex_h__)
#define __geom_CyclicIndex_h__

#include "../defs.h"

#include <cassert>

// If we are debugging the whole geom namespace.
#if defined(DEBUG_geom) && !defined(DEBUG_CyclicIndex)
#define DEBUG_CyclicIndex
#endif

BEGIN_NAMESPACE_GEOM

//! A class for a cyclic index.
class CyclicIndex {
  //
  // Data
  //

private:

  int _index, _n;

  //
  // Friends
  //

  friend CyclicIndex& operator++(CyclicIndex& ci);
  friend CyclicIndex& operator--(CyclicIndex& ci);

  //! Default constructor not implemented.
  CyclicIndex();

public:

  //--------------------------------------------------------------------------
  //! \name Constructors etc.
  // @{

  //! Constructor.  Initialize index to zero.
  CyclicIndex(const int n) : 
    _index(0), 
    _n(n) { 
#ifdef DEBUG_CyclicIndex
    assert(_n > 0); 
#endif
  }

  //! Copy constructor.
  CyclicIndex(const CyclicIndex& other) : 
    _index(other._index), 
    _n(other._n) { 
#ifdef DEBUG_CyclicIndex
    assert(_n > 0); 
#endif
  }

  //! Assignment operator.
  CyclicIndex& 
  operator=(const CyclicIndex& other);
  
  //! Trivial destructor.
  ~CyclicIndex()
  {}

  // @}
  //--------------------------------------------------------------------------
  //! \name Accesors.
  // @{

  //! Return the index.
  int 
  operator()() const { 
    return _index; 
  }

  // @}
  //--------------------------------------------------------------------------
  //! \name Manipulators.
  // @{

  //! Set the index to i mod N.
  void set(int i);
};

//
// Increment and Decrement Operators.
//
  
//! Increment the index.
/*! \relates CyclicIndex */
CyclicIndex& 
operator++(CyclicIndex& ci);

//! Decrement the index.
/*! \relates CyclicIndex */
CyclicIndex& 
operator--(CyclicIndex& ci);

END_NAMESPACE_GEOM

#define __geom_CyclicIndex_ipp__
#include "CyclicIndex.ipp"
#undef __geom_CyclicIndex_ipp__

#endif
