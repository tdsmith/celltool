// -*- C++ -*-

/*! 
  \file sign.h
  \brief Contains the sign function.
*/

#if !defined(__ads_sign_h__)
#define __ads_sign_h__

#include "../defs.h"

BEGIN_NAMESPACE_ADS

//-----------------------------------------------------------------------------
/*! \defgroup algorithm_sign Algorithm: Sign */
// @{

/*!
  \brief This does what you think it does.
  The number type must be less than and greater than comparable.
  \param x is a number.
  \return 1, 0 or -1 if \c x is positive, zero or negative.
*/
template<typename T>
inline 
int
sign(const T x) {
  if (x > 0) {
    return 1;
  }
  else if (x < 0) {
    return -1;
  }
  return 0;
}

// @}

END_NAMESPACE_ADS

#endif
