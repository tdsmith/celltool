// -*- C++ -*-

/*!
  \file numerical/random/poisson/PoissonPdfAtTheMode.h
  \brief Probability density function evaluated at the mode for the Poisson distribution.
*/

#if !defined(__numerical_PoissonPdfAtTheMode_h__)
#define __numerical_PoissonPdfAtTheMode_h__

#include "PoissonPdf.h"

#include "../../interpolation/hermite.h"

#include "../../../ads/array/Array.h"

// If we are debugging the whole numerical package.
#if defined(DEBUG_numerical) && !defined(DEBUG_PoissonPdfAtTheMode)
#define DEBUG_PoissonPdfAtTheMode
#endif

BEGIN_NAMESPACE_NUMERICAL

//! Probability density function evaluated at the mode for the Poisson distribution.
/*!
  \param T The number type.  By default it is double.

  CONTINUE.
*/
template<typename T = double>
class PoissonPdfAtTheMode : 
  public std::unary_function<T,T> {
  //
  // Private types.
  //
private:

  typedef std::unary_function<T,T> Base;

  //
  // Public types.
  //
public:

  //! The argument type.
  typedef typename Base::argument_type argument_type;
  //! The result type.
  typedef typename Base::result_type result_type;
  //! The number type.
  typedef T Number;

  //
  // Member data.
  //
private:

  // The lower bound of the range of means.
  Number _lowerBound;
  // Factor that will scale the argument to the index.
  Number _scaleToIndex;
  // Polynomial coefficients.
  ads::Array<1,Number> _coefficients;

  //
  // Not implemented.
  //
private:

  //! Default constructor not implemented.
  PoissonPdfAtTheMode();

public:

  //! Construct from the range of means and the number of patches per unit.
  PoissonPdfAtTheMode(int closedLowerBound, int openUpperBound, 
		      int numberOfPatchesPerUnit);

  //! Copy constructor.
  /*!
    \note This function is expensive.
  */
  PoissonPdfAtTheMode(const PoissonPdfAtTheMode& other);

  //! Assignment operator.
  /*!
    \note This function is expensive.
  */
  PoissonPdfAtTheMode&
  operator=(const PoissonPdfAtTheMode& other);

  //! Destructor.
  ~PoissonPdfAtTheMode()
  {}

  //! Return the probability density function evaluated at the mode.
  result_type
  operator()(argument_type mean) const;
};


END_NAMESPACE_NUMERICAL

#define __numerical_random_PoissonPdfAtTheMode_ipp__
#include "PoissonPdfAtTheMode.ipp"
#undef __numerical_random_PoissonPdfAtTheMode_ipp__

#endif
