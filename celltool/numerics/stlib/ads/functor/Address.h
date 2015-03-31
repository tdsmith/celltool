// -*- C++ -*-

/*! 
  \file Address.h
  \brief Contains a functor for dereferencing handles to objects.

  The ads::Address structure is a functor for dereferencing a handle.
*/

#if !defined(__ads_Address_h__)
#define __ads_Address_h__

#include "../defs.h"

//#include <iterator>
#include <functional>

BEGIN_NAMESPACE_ADS

//-----------------------------------------------------------------------------
/*! \defgroup functor_address Functor: Address */
// @{

//! A functor for taking the address of an object.
/*!
  \param Object is the object type.
*/
template <typename Object>
struct Address : 
  public std::unary_function<Object,Object*> {
  //! The result type is a pointer to the object type.
  typedef typename std::unary_function<Object,Object*>::result_type 
  result_type;

  //! Return the address of \c x.
  result_type
  operator()( Object& x ) const 
  { 
    return &x;
  }
};

//! Return an \c Address<Object>.
/*!
  This is a convenience function for constructing an \c Address<Object>.
  Instead of writing
  \code
  ads::Address<int> add;
  y = add( x );
  \endcode
  one can write
  \code
  y = ads::address<int>()( x );
  \endcode
 */
template <typename Object>
inline
Address<Object>
address() {
  return Address<Object>();
}

// @}

END_NAMESPACE_ADS

#endif
