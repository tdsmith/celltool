// -*- C++ -*-

/*! 
  \file numerical/specialFunctions/Gamma.h
  \brief Uniform specialFunctions deviates.
*/

#if !defined(__numerical_Gamma_h__)
#define __numerical_Gamma_h__

#include "../defs.h"

#include "../../ads/array/FixedArray.h"

#include <functional>

#include <cassert>

// If we are debugging the whole numerical package.
#if defined(DEBUG_numerical) && !defined(DEBUG_numerical_Gamma)
#define DEBUG_numerical_Gamma
#endif

BEGIN_NAMESPACE_NUMERICAL

//! Compute the logarithm of the gamma function for positive argument.
/*!
  \param T The number type.  By default it is double.

  This is adapted from the gammln() function in "Numerical Recipes".

  The figure below shows execution times for a range of arguments.
  The test code is in stlib/performance/numerical/specialFunctions.  
  It was compiled with GNU g++ 4.0 using the 
  flags: -O3 -funroll-loops -fstrict-aliasing.
  I ran the tests on a Mac Mini with a 1.66 GHz Intel Core Duo processor and
  512 MB DDR2 SDRAM.

  \image html LogarithmOfGamma.jpg "Execution times for the logarithm of Gamma."
  \image latex LogarithmOfGamma.pdf "Execution times for the logarithm of Gamma." width=0.5\textwidth

  Below is a table of the relative error for a range of arguments.
  <!--Generated with the unit test code.  Gamma.txt-->
  <table>
  <tr> <th> \f$x\f$ <th> \f$\log(\Gamma(x))\f$ <th> Relative Error
  <tr> <td> 1e-008 <td> 18.4207 <td> 0
  <tr> <td> 1e-007 <td> 16.1181 <td> -2.20418e-016
  <tr> <td> 1e-006 <td> 13.8155 <td> 0
  <tr> <td> 1e-005 <td> 11.5129 <td> -1.54292e-016
  <tr> <td> 0.0001 <td> 9.21028 <td> 0
  <tr> <td> 0.001 <td> 6.90718 <td> 0
  <tr> <td> 0.01 <td> 4.59948 <td> 1.54483e-015
  <tr> <td> 0.1 <td> 2.25271 <td> 2.16849e-014
  <tr> <td> 1 <td> 0 <td> 0
  <tr> <td> 10 <td> 12.8018 <td> 3.8436e-014
  <tr> <td> 100 <td> 359.134 <td> 2.90125e-013
  <tr> <td> 1000 <td> 5905.22 <td> 3.0341e-014
  <tr> <td> 10000 <td> 82099.7 <td> 2.30421e-015
  <tr> <td> 100000 <td> 1.05129e+006 <td> 0
  <tr> <td> 1e+006 <td> 1.28155e+007 <td> 0
  </table>
*/
template<typename T = double>
class LogarithmOfGamma : 
  public std::unary_function<T,T> {
public:

  //! The number type.
  typedef T Number;

private:

  ads::FixedArray<6,Number> _cof;

public:

  //! Default constructor.
  LogarithmOfGamma() :
    _cof() {
    _cof[0] = 76.18009172947146;
    _cof[1] = -86.50532032941677;
    _cof[2] = 24.01409824083091;
    _cof[3] = -1.231739572450155;
    _cof[4] = 0.1208650973866179e-2;
    _cof[5] = -0.5395239384953e-5;
  }

  //! Copy constructor.
  LogarithmOfGamma(const LogarithmOfGamma& other) :
    _cof(other._cof)
  {}

  //! Assignment operator.
  LogarithmOfGamma&
  operator=(const LogarithmOfGamma& other) {
    if (this != &other) {
      _cof = other._cof;
    }
    return *this;
  }

  //! Trivial destructor.
  ~LogarithmOfGamma()
  {}

  //! Return the logarithm of gamma.
  Number
  operator()(Number x) const;
};


//! Convenience function for constructing a LogarithmOfGamma.
/*!
  \relates LogarithmOfGamma
*/
template<typename T>
inline
LogarithmOfGamma<T>
constructLogarithmOfGamma() {
  return LogarithmOfGamma<T>();
}


END_NAMESPACE_NUMERICAL

#define __numerical_specialFunctions_Gamma_ipp__
#include "Gamma.ipp"
#undef __numerical_specialFunctions_Gamma_ipp__

#endif
