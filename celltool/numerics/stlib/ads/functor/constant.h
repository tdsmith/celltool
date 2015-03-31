// -*- C++ -*-

/*! 
  \file constant.h
  \brief Functors that return a constant.
*/

#if !defined(__ads_Constant_h__)
#define __ads_Constant_h__

#include "../defs.h"

#include <functional>

BEGIN_NAMESPACE_ADS

//-----------------------------------------------------------------------------
/*! \defgroup functor_constant Functor: Constant 
  Constant functors.
*/
// @{

//! Generator that returns a constant.
template<typename _Result>
class GeneratorConstant {
  //
  // Public types.
  //
public:

  //! The result type.
  typedef _Result result_type;

  //
  // Member data.
  //
private:

  //! The constant value.
  result_type _value;

  //--------------------------------------------------------------------------
  //! \name Constructors etc.
  //@{
public:

  //! Default constructor.  Uninitialized value.
  GeneratorConstant() :
    _value()
  {}

  //! Construct from the constant value.
  GeneratorConstant(const result_type& x) :
    _value(x)
  {}

  //@}
  //--------------------------------------------------------------------------
  //! \name Manipulators.
  //@{
public:

  //! Set the return value.
  void
  set(const result_type& x) {
    _value = x;
  }

  //@}
  //--------------------------------------------------------------------------
  //! \name Functor.
  //@{
public:

  //! Return the constant value.
  const result_type&
  operator()() const {
    return _value;
  }

  //@}
};


//! Generator that returns void.
template<>
class GeneratorConstant<void> {
  //
  // Public types.
  //
public:

  //! The result type.
  typedef void result_type;

  //--------------------------------------------------------------------------
  //! \name Manipulators.
  //@{
public:

  //! Set the return value.  This is only here for compatibility.
  void
  set() {
  }

  //@}
  //--------------------------------------------------------------------------
  //! \name Functor.
  //@{
public:

  //! Return void.
  void
  operator()() const {
  }

  //@}
};


//! Convenience function for constructing a \c GeneratorConstant.
/*!
  \relates GeneratorConstant
  The constant value is specified.
*/
template<typename _Result>
inline
GeneratorConstant<_Result>
constructGeneratorConstant(const _Result& x) {
  return GeneratorConstant<_Result>(x);
}


//! Convenience function for constructing a \c GeneratorConstant.
/*!
  \relates GeneratorConstant
  The constant value is default initialized.
*/
template<typename _Result>
inline
GeneratorConstant<_Result>
constructGeneratorConstant() {
  return GeneratorConstant<_Result>();
}







//! Unary functor that returns a constant.
template<typename _Argument, typename _Result>
class UnaryConstant : 
  public std::unary_function<_Argument,_Result> {
  //
  // Private types.
  //
private:

  typedef std::unary_function<_Argument,_Result> Base;

  //
  // Public types.
  //
public:

  //! The argument type.
  typedef typename Base::argument_type argument_type;
  //! The result type.
  typedef typename Base::result_type result_type;

  //
  // Member data.
  //
private:

  //! The constant value.
  result_type _value;

  //--------------------------------------------------------------------------
  //! \name Constructors etc.
  //@{
public:

  //! Default constructor.  Default initialized value.
  UnaryConstant() :
    _value()
  {}

  //! Construct from the constant value.
  UnaryConstant(const result_type& x) :
    _value(x)
  {}

  //@}
  //--------------------------------------------------------------------------
  //! \name Manipulators.
  //@{
public:

  //! Set the return value.
  void
  set(const result_type& x) {
    _value = x;
  }

  //@}
  //--------------------------------------------------------------------------
  //! \name Functor.
  //@{
public:

  //! Return the constant value.
  const result_type&
  operator()(const argument_type& x) const {
    return _value;
  }

  //@}
};


//! Convenience function for constructing a \c UnaryConstant.
/*!
  \relates UnaryConstant
  The constant value is specified.
*/
template<typename _Argument, typename _Result>
inline
UnaryConstant<_Argument, _Result>
constructUnaryConstant(const _Result& x) {
  return UnaryConstant<_Argument, _Result>(x);
}

//! Convenience function for constructing a \c UnaryConstant.
/*!
  \relates UnaryConstant
  The constant value is default initialized.
*/
template<typename _Argument, typename _Result>
inline
UnaryConstant<_Argument, _Result>
constructUnaryConstant() {
  return UnaryConstant<_Argument, _Result>();
}







//! Binary functor that returns a constant.
template<typename _FirstArgument, typename _SecondArgument, typename _Result>
class BinaryConstant : 
  public std::binary_function<_FirstArgument,_SecondArgument,_Result> {
  //
  // Private types.
  //
private:

  typedef std::binary_function<_FirstArgument,_SecondArgument,_Result> Base;

  //
  // Public types.
  //
public:

  //! The first argument type.
  typedef typename Base::first_argument_type first_argument_type;
  //! The second argument type.
  typedef typename Base::second_argument_type second_argument_type;
  //! The result type.
  typedef typename Base::result_type result_type;

  //
  // Member data.
  //
private:

  //! The constant value.
  result_type _value;

  //--------------------------------------------------------------------------
  //! \name Constructors etc.
  //@{
public:

  //! Default constructor.  Default initialized value.
  BinaryConstant() :
    _value()
  {}

  //! Construct from the constant value.
  BinaryConstant(const result_type& x) :
    _value(x)
  {}

  //@}
  //--------------------------------------------------------------------------
  //! \name Manipulators.
  //@{
public:

  //! Set the return value.
  void
  set(const result_type& x) {
    _value = x;
  }

  //@}
  //--------------------------------------------------------------------------
  //! \name Functor.
  //@{
public:

  //! Return the constant value.
  const result_type&
  operator()(const first_argument_type& x, 
	     const second_argument_type& y) const {
    return _value;
  }

  //@}
};


//! Convenience function for constructing a \c BinaryConstant.
/*!
  \relates BinaryConstant
  The constant value is specified.
*/
template<typename _FirstArgument, typename _SecondArgument, typename _Result>
inline
BinaryConstant<_FirstArgument, _SecondArgument, _Result>
constructBinaryConstant(const _Result& x) {
  return BinaryConstant<_FirstArgument, _SecondArgument, _Result>(x);
}


//! Convenience function for constructing a \c BinaryConstant.
/*!
  \relates BinaryConstant
  The constant value is default initialized.
*/
template<typename _FirstArgument, typename _SecondArgument, typename _Result>
inline
BinaryConstant<_FirstArgument, _SecondArgument, _Result>
constructBinaryConstant() {
  return BinaryConstant<_FirstArgument, _SecondArgument, _Result>();
}

// @}

END_NAMESPACE_ADS

#endif
