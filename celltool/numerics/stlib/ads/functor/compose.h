// -*- C++ -*-

/*! 
  \file compose.h
  \brief Contains functors for composing other functors.

  This file contains functors with more functionality that the standard
  functor composition classes.  It defines the following functors:
  - ads::unary_compose_unary_unary is a unary functor that composes two 
  unary functors, f(g(x)).
  - ads::binary_compose_unary_binary is a binary functor that composes 
  a unary and a binary functor: f(g(x,y)).
  - ads::unary_compose_binary_unary is a unary functor that composes 
  a binary functor with two unary functors: f(g(x),h(x)).
  - ads::binary_compose_binary_unary is a binary functor that composes a 
  binary functor with two unary functors: f(g(x),h(y)).
*/

#if !defined(__ads_compose_h__)
#define __ads_compose_h__

#include "../defs.h"

#include <functional>

BEGIN_NAMESPACE_ADS

//-----------------------------------------------------------------------------
/*! \defgroup functor_compose Functor: Compose 

  We define functors with more functionality that the standard
  functor composition classes:
  - ads::unary_compose_unary_unary is a unary functor that composes two 
  unary functors, f(g(x)).
  - ads::binary_compose_unary_binary is a binary functor that composes 
  a unary and a binary functor: f(g(x,y)).
  - ads::unary_compose_binary_unary is a unary functor that composes 
  a binary functor with two unary functors: f(g(x),h(x)).
  - ads::binary_compose_binary_unary is a binary functor that composes a 
  binary functor with two unary functors: f(g(x),h(y)).
*/
// @{

//! Unary function composition of two unary functions: f(g(x)).
/*!
  This has the same functionality as the SGI extension functor
  unary_compose.
*/
template <class F, class G>
class unary_compose_unary_unary
  : public std::unary_function< typename G::argument_type,
				typename F::result_type >
{
private:
  typedef std::unary_function< typename G::argument_type,
			       typename F::result_type > base_type;
protected:
  //! The outer unary functor.
  F _f;
  //! The inner unary functor.
  G _g;

public:
  //! The argument type.
  typedef typename base_type::argument_type argument_type;
  //! The result type.
  typedef typename base_type::result_type result_type;

  //! Default constructor.
  unary_compose_unary_unary() 
    : _f(), _g() 
  {}

  //! Construct from functors.
  unary_compose_unary_unary( const F& f, const G& g ) 
    : _f( f ), _g( g ) 
  {}

  //! Return f(g(x)).
  result_type
  operator()( const argument_type& x ) const {
    return _f( _g( x ) );
  }
};


//! Binary function composition of a unary and binary function: f(g(x,y)).
template <class F, class G>
class binary_compose_unary_binary
  : public std::binary_function< typename G::first_argument_type,
				 typename G::second_argument_type,
				 typename F::result_type >
{
private:
  typedef std::binary_function< typename G::first_argument_type,
				typename G::second_argument_type,
				typename F::result_type > base_type;
protected:
  //! The outer unary functor.
  F _f;
  //! The inner binary functor.
  G _g;

public:
  //! The first argument type.
  typedef typename base_type::first_argument_type first_argument_type;
  //! The second argument type.
  typedef typename base_type::second_argument_type second_argument_type;
  //! The result type.
  typedef typename base_type::result_type result_type;

  //! Default constructor.
  binary_compose_unary_binary()
    : _f(), _g() 
  {}

  //! Construct from functors.
  binary_compose_unary_binary( const F& f, const G& g ) 
    : _f( f ), _g( g ) 
  {}

  //! Return f(g(x,y)).
  result_type
  operator()( const first_argument_type& x, const second_argument_type& y ) 
    const {
    return _f( _g( x, y ) );
  }
};


//! Unary function composition of a binary and unary functions: f(g(x),h(x)).
/*!
  This has the same functionality as the SGI extension functor
  binary_compose.
*/
template <class F, class G, class H>
class unary_compose_binary_unary
  : public std::unary_function< typename G::argument_type,
				typename F::result_type >
{
private:
  typedef std::unary_function< typename G::argument_type,
			       typename F::result_type > base_type;
protected:
  //! The outer binary functor.
  F _f;
  //! The first inner unary functor.
  G _g;
  //! The second inner unary functor.
  H _h;

public:
  //! The argument type.
  typedef typename base_type::argument_type argument_type;
  //! The result type.
  typedef typename base_type::result_type result_type;

  //! Default constructor.
  unary_compose_binary_unary()
    : _f(), _g(), _h() 
  {}

  //! Construct from functors.
  unary_compose_binary_unary( const F& f, const G& g, const H& h ) 
    : _f( f ), _g( g ), _h( h ) 
  {}

  //! Return f(g(x),h(x)).
  result_type
  operator()( const argument_type& x ) const {
    return _f( _g( x ), _h( x ) );
  }
};


//! Binary function composition of a binary and unary functions: f(g(x),h(y)).
template <class F, class G, class H>
class binary_compose_binary_unary
  : public std::binary_function< typename G::argument_type,
				 typename H::argument_type,
				 typename F::result_type >
{
private:
  typedef std::binary_function< typename G::argument_type,
				typename H::argument_type,
				typename F::result_type > base_type;
protected:
  //! The outer binary functor.
  F _f;
  //! The first inner unary functor.
  G _g;
  //! The second inner unary functor.
  H _h;

public:
  //! The first argument type.
  typedef typename base_type::first_argument_type first_argument_type;
  //! The second argument type.
  typedef typename base_type::second_argument_type second_argument_type;
  //! The result type.
  typedef typename base_type::result_type result_type;

  //! Default constructor.
  binary_compose_binary_unary()
    : _f(), _g(), _h()
  {}

  //! Construct from functors.
  binary_compose_binary_unary( const F& f, const G& g, const H& h ) 
    : _f( f ), _g( g ), _h( h )
  {}

  //! Return f(g(x),h(y)).
  result_type
  operator()( const first_argument_type& x, const second_argument_type& y ) 
    const {
    return _f( _g( x ), _h( y ) );
  }

  // CONTINUE: Are these a good idea?  Should I add them to the rest of the 
  // functors?
  //! Return a reference to the outer binary function.
  F&
  outer()
  {
    return _f;
  }

  //! Return a reference to the first inner unary function.
  G&
  first_inner()
  {
    return _g;
  }

  //! Return a reference to the second inner unary function.
  H&
  second_inner()
  {
    return _h;
  }
};

// @}

END_NAMESPACE_ADS

#endif
