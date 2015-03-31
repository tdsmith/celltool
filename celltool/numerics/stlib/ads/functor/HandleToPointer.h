// -*- C++ -*-

/*! 
  \file HandleToPointer.h
  \brief Contains a functor for converting handles to pointers.

  The ads::HandleToPointer structure is a functor for converting handles 
  to pointers.
*/

#if !defined(__ads_functor_HandleToPointer_h__)
#define __ads_functor_HandleToPointer_h__

#include "../defs.h"

#include <iterator>
#include <functional>

BEGIN_NAMESPACE_ADS

//-----------------------------------------------------------------------------
/*! \defgroup functor_handle_to_pointer Functor: HandleToPointer */
// @{

//! A functor for converting handles to pointers.
/*!
  \param Handle is the handle type.
  \param Result is the pointer type.  By default it is 
  \c std::iterator_traits<Handle>::pointer.
*/
template < typename Handle, 
	   typename Pointer = typename 
	   std::iterator_traits<Handle>::pointer >
struct HandleToPointer : 
  public std::unary_function<Handle,Pointer> 
{
  //! The base functor.
  typedef std::unary_function<Handle,Pointer> base_type;
  //! The argument type is the handle.
  typedef typename base_type::argument_type argument_type;
  //! The result type is the pointer.
  typedef typename base_type::result_type result_type;
  
  //! Convert the handle to a pointer.
  result_type
  operator()( argument_type x ) const 
  { 
    return &*x; 
  }
};

//! Return a \c HandleToPointer<Handle>.
/*!
  This is a convenience function for constructing a \c HandleToPointer<Handle>.
  Instead of writing
  \code
  ads::HandleToPointer<int*> htp;
  y = htp( x );
  \endcode
  one can write
  \code
  y = ads::handle_to_pointer<int*>()( x );
  \endcode
 */
template < typename Handle>
inline
HandleToPointer<Handle>
handle_to_pointer()
{
  return HandleToPointer<Handle>();
}

// @}

END_NAMESPACE_ADS

#endif
