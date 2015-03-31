// -*- C++ -*-

/*! 
  \file TrivialAssignable.h
  \brief A trivial assignable object.
*/

#if !defined(__ads_TrivialAssignable_h__)
#define __ads_TrivialAssignable_h__

#include "../defs.h"

BEGIN_NAMESPACE_ADS

//! A trivial assignable object.
/*!
  CONTINUE
*/
class TrivialAssignable {
public:

  //--------------------------------------------------------------------------
  //! \name Constructors etc.
  //@{

  //! Default constructor.
  TrivialAssignable() {}

  //! Copy constructor.
  TrivialAssignable(const TrivialAssignable&) {}

  //! Assignment operator for any type.
  template<typename X>
  const X& 
  operator=(const X& x) {
    return x;
  }

  //@}
};

END_NAMESPACE_ADS

#endif
