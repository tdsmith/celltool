// -*- C++ -*-

/*! 
  \file numerical/random/poisson.h
  \brief Includes the Poisson random number generator classes.
*/

#if !defined(__numerical_random_poisson_h__)
#define __numerical_random_poisson_h__

// Probability density and cumulative density for the Poisson distribution.
#include "poisson/PoissonCdf.h"
#include "poisson/PoissonCdfAtTheMode.h"
#include "poisson/PoissonPdf.h"
#include "poisson/PoissonPdfAtTheMode.h"
#include "poisson/PoissonPdfCached.h"
#include "poisson/PoissonPdfCdfAtTheMode.h"

// Poisson generators.
#include "poisson/PoissonGeneratorAcceptanceComplementWinrand.h"
#include "poisson/PoissonGeneratorDirectNr.h"
#include "poisson/PoissonGeneratorDirectRejectionNr.h"
#include "poisson/PoissonGeneratorExpAcNorm.h"
#include "poisson/PoissonGeneratorExpInvAc.h"
#include "poisson/PoissonGeneratorExpInvAcNorm.h"
#include "poisson/PoissonGeneratorExponentialInterArrival.h"
#include "poisson/PoissonGeneratorExponentialInterArrivalUnrolled.h"
#include "poisson/PoissonGeneratorExponentialInterArrivalUsingUniform.h"
#include "poisson/PoissonGeneratorIfmAcNorm.h"
#include "poisson/PoissonGeneratorInvAcNorm.h"
#include "poisson/PoissonGeneratorInvIfmAcNorm.h"
#include "poisson/PoissonGeneratorInversionBuildUp.h"
#include "poisson/PoissonGeneratorInversionBuildUpSimple.h"
#include "poisson/PoissonGeneratorInversionCheckPdf.h"
#include "poisson/PoissonGeneratorInversionChopDown.h"
#include "poisson/PoissonGeneratorInversionChopDownSimple.h"
#include "poisson/PoissonGeneratorInversionChopDownUnrolled.h"
#include "poisson/PoissonGeneratorInversionFromModeBuildUp.h"
#include "poisson/PoissonGeneratorInversionFromModeChopDown.h"
#include "poisson/PoissonGeneratorInversionRatioOfUniformsWinrand.h"
#include "poisson/PoissonGeneratorInversionRejectionPatchworkWinrand.h"
#include "poisson/PoissonGeneratorInversionTable.h"
#include "poisson/PoissonGeneratorInversionTableAcceptanceComplementWinrand.h"
#include "poisson/PoissonGeneratorNormal.h"
#include "poisson/PoissonGeneratorRanlib.h"
#include "poisson/PoissonGeneratorRecursiveGsl.h"
#include "poisson/PoissonGeneratorRejectionNr.h"
#include "poisson/PoissonGeneratorStochKit.h"

BEGIN_NAMESPACE_NUMERICAL

/*!
  \page numerical_random_poisson Poisson Generators

  \section numerical_random_poisson_properties Properties

  Consider the number of discrete events occurring in a specified, finite time
  interval.  Let the events be independent and occur at a known average rate.  
  The Poisson distribution expresses the probability of how many events 
  occur during the time interval.  Since the events are discrete, 
  the Poisson distribution is a discrete probability distribution.
  The distribution depends on the parameter \f$\mu\f$, a non-negative,
  real-valued number which is the average number of events that will
  occur in the time interval.  The probability density function 
  \f$\mathrm{pdf}_{\mu}(n)\f$ expresses the probability that exactly 
  <em>n</em> events occur.  The cumulative distribution function
  \f$\mathrm{cdf}_{\mu}(n)\f$ expresses the probability that at most
  <em>n</em> events occur.

  For an example application of the Poisson distribution, 
  the events could be a chemical reaction occurring in a 
  well-stirred medium.  The reaction rate likely depends on the concentration
  of reactant and product, but during a short time interval the rate 
  could be approximated as constant.  Then the average expected number 
  of reactions \f$\mu\f$ is the product of the reaction rate and the length
  of the time interval.

  Below is a table that summarizes some important properties of the 
  Poisson distribution.
  <table>
  <tr> <th> Probability density function 
  <td> \f$\mathrm{pdf}_{\mu}(n) = e^{-\mu} \mu^n / n!\f$
  <tr> <th> Cumulative distribution function 
  <td> \f$\mathrm{cdf}_{\mu}(n) = \sum_{k = 0}^n e^{-\mu} \mu^k / k!\f$
  <tr> <th> Mean <td> \f$\mu\f$
  <tr> <th> Mode <td> \f$\lfloor \mu \rfloor\f$
  <tr> <th> Variance <td> \f$\mu\f$
  <tr> <th> Skewness <td> \f$1 / \sqrt{\mu}\f$
  <tr> <th> Kurtosis <td> \f$1 / \mu\f$
  </table>
  The CDF may also be expressed in terms of the incomplete Gamma function
  \f$\Gamma\f$ or the regularized Gamma function <em>Q</em>.
  \f[
  \mathrm{cdf}_{\mu}(n) = \Gamma(n + 1, \mu) / \Gamma(n + 1) = Q(n + 1, \mu)
  \f]
  The definitions of the Gamma functions follow.
  \f[
  \Gamma(a) = \int_0^\infty t^{a-1} e^{-t}\,\mathrm{d}t
  \f]
  \f[
  \Gamma(a, x) = \int_x^\infty t^{a-1} e^{-t}\,\mathrm{d}t
  \f]
  \f[
  Q(a, x) = \Gamma(a, x) / \Gamma(a)
  \f]

  Below we plot the PDF and CDF of the Poisson distribution for means of 
  1, 5.5, and 10.  (These functions are defined for non-negative integer
  values.  To plot them on the real axis, we show step functions.)

  \image html random/PoissonPdf.jpg "Some probabilty density functions for the Poisson distribution."
  \image latex random/PoissonPdf.pdf "Some probabilty density functions for the Poisson distribution." width=0.5\textwidth

  \image html random/PoissonCdf.jpg "Some cumulative distribution functions for the Poisson distribution."
  \image latex random/PoissonCdf.pdf "Some cumulative distribution functions for the Poisson distribution." width=0.5\textwidth





  \section numerical_random_poisson_classes Classes

  I have implemented a number of generators for Poisson deviates.  
  Generators that use exponential inter-arrival times.
  - PoissonGeneratorExponentialInterArrival uses exponential deviates in the
  exponential inter-arrival method.  It is very efficient for small means.
  - PoissonGeneratorExponentialInterArrivalUsingUniform uses uniform deviates 
  in the exponential inter-arrival method.  It is not as efficient as inversion 
  methods, but is reasonable for small means.
  - PoissonGeneratorExponentialInterArrivalUnrolled uses loop unrolling.
  - PoissonGeneratorDirectNr is the "Numerical Recipes" 
  implementation of the exponential inter-arrival method.

  Generators that use inversion.
  - PoissonGeneratorInversionBuildUp uses the build-up version of the inversion
  method.
  - PoissonGeneratorInversionBuildUpSimple is a simple implementation of 
  the build-up version of the inversion method.
  - PoissonGeneratorInversionChopDown uses the chop-down version of the 
  inversion method.
  - PoissonGeneratorInversionChopDownSimple is a simple implementation of
  the chop-down version of the inversion method.
  - PoissonGeneratorInversionChopDownUnrolled is an unsuccessful attempt to 
  use loop unrolling with the chop-down inversion method.
  - PoissonGeneratorInversionCheckPdf An inversion method that checks the value
  of the PDF during the loop.
  - PoissonGeneratorInversionTable stores the Poisson PDF's in a table.  This
  is useful when one repeatedly computes deviates with the same mean.

  Generators that use inversion from the mode.
  - PoissonGeneratorInversionFromModeBuildUp also uses the cumulative 
  distribution function, but starts the process at the mode of the distribution.
  Compared to the regular inversion methods, it is less efficient for small
  means and more efficient for large means.
  - PoissonGeneratorInversionFromModeChopDown is the chop-down version of the
  inversion from the mode method.

  Generators that are efficient for large means.
  - PoissonGeneratorAcceptanceComplementWinrand The Winrand implementation
  of the acceptance-complement method.
  - PoissonGeneratorNormal uses a normal approximation of the Poisson 
  distribution.  It is less expensive than the rejection method.  When 
  the normal approximation is good enough depends on the application.
  - PoissonGeneratorRecursiveGsl<T,DiscreteUniformGeneratorMt19937Gsl> 
  An algorithm that uses recursive properties of the Poisson distribution.
  - PoissonGeneratorRejectionNr is the "Numerical Recipes"
  implementation of the rejection method.

  I have adapted some methods from various random number packages.  Each
  of them are hybrid methods.
  - PoissonGeneratorDirectRejectionNr
  - PoissonGeneratorInversionRatioOfUniformsWinrand
  - PoissonGeneratorInversionRejectionPatchworkWinrand
  - PoissonGeneratorInversionTableAcceptanceComplementWinrand
  - PoissonGeneratorRanlib
  - PoissonGeneratorStochKit


  Finally, there are also hybrid methods which combine the simple algorithms.
  - PoissonGeneratorExpAcNorm
  - PoissonGeneratorExpInvAc
  - PoissonGeneratorExpInvAcNorm
  - PoissonGeneratorIfmAcNorm
  - PoissonGeneratorInvAcNorm
  - PoissonGeneratorInvIfmAcNorm
  
  There are functors to compute the probability density function and the 
  cumulative density function for the Poisson distribution.  These are used
  in the functors for computing Poisson random deviates.
  - PoissonPdf
  - PoissonPdfCached
  - PoissonCdf
  - PoissonCdfAtTheMode

  I have implemented each of the Poisson deviate generators as 
  an <em>adaptable unary function</em>, a functor that takes one argument.
  (See \ref numerical_random_austern1999 "Generic Programming and the STL".)
  The classes are templated on the floating point number type
  (\c double by default) and the uniform deviate generator type
  (DiscreteUniformGeneratorMt19937 by default).
  In addition, some of the classes are templated on the exponential deviate 
  generator type (ExponentialGeneratorZiggurat by default)
  and/or the normal deviate generator type
  (NormalGeneratorZigguratVoss by default).
  Below are a few examples of constructing a Poisson deviate generator.
  \code
  // Use the default number type (double) and the default generators.
  typedef numerical::PoissonGeneratorInversionChopDown<> Poisson; 
  Poisson::DiscreteUniformGenerator uniform;
  Poisson poisson(&uniform); \endcode
  \code
  // Use the default number type (double) and the default generators.
  typedef numerical::PoissonGeneratorExpInvAcNorm<> Poisson; 
  Poisson::DiscreteUniformGenerator uniform;
  Poisson::ExponentialGenerator exponential(&uniform);
  Poisson::NormalGenerator normal(&uniform);
  // Use the normal approximation for means greater than 1000.
  Poisson poisson(&exponential, &normal, 1000.0); \endcode

  Each class defines the following types.
  - \c Number is the floating point number type.
  - \c DiscreteUniformGenerator is the discrete uniform generator type.
  - \c argument_type is \c Number.
  - \c result_type is \c int.

  Each generator has the following member functions.
  - \c operator()(Number mean) Return a Poisson deviate with the specifed mean.
  - seed(typename DiscreteUniformGenerator::result_type seedValue) 
  Seed the uniform generator.
  - getDiscreteUniformGenerator() Get the discrete, uniform generator.
  .
  \code
  double mean = 2.0;
  int deviate = poisson(mean);
  poisson.seed(123U);
  Poisson::DiscreteUniformGenerator* uniform = poisson.getDiscreteUniformGenerator(); \endcode


  \section numerical_random_poisson_options Optimization Options

  Some of the methods which are used for small means evaluate 
  \f$e^{-x}\f$.  The exponential is an expensive function.
  <!--CONTINUE cite-->
  Defining \c NUMERICAL_POISSON_HERMITE_APPROXIMATION will enable the 
  use of a functor that uses Hermite interpolation to approximate 
  this function.  (See numerical::Hermite .)
  Note that this package is based on 32-bit uniform deviates.
  When these unsigned integer deviates are converted to floating-point
  deviates, the minimum distance between them is
  \f$2^{-32} \approx 2.3 \times 10^{-10}\f$.  For the Hermite interpolation,
  I use a patch length of 0.01.  For \f$x \in [0 \ldots 32]\f$, this 
  yields a maximum relative error of \f$2.6 \times 10^{-11}\f$, which is
  sufficiently accurate for 32-bit deviates.  Note that when this option
  is enabled, you must specify the maximum mean in the constructor.
  This is necessary to build the array of coefficients for the Hermite
  interpolation.


  \section numerical_random_poisson_tests Tests

  There are three tests that I use to assess the performance of the Poisson
  generators.  For each class I measure the execution time for a range of 
  means that is appropriate for that algorithm.  The three tests are as 
  follows.
  - Calculate deviates for the same mean.  Some algorithms are able to 
  re-use cached values when called with the same mean.
  - Calculate deviates for slightly different values of the mean.   For 
  each function call, I increase the mean by epsilon times the mean where 
  epsilon is the floating point precision.  This test allows one to 
  measure the performance without giving advantage to algorithms that 
  cache the old means.
  - Calculate deviates for a distribution of means.  For this test I
  make a make an array of equally-spaced means.  (The lower bound, number 
  of means, and multiplicity of each value can be specified.)  Then 
  I shuffle elements of the array.  This test is most relevant to 
  Gillespie's tau-leaping algorithm.  In that application, each reaction
  needs a Poisson deviate with a different mean.  




  \section numerical_random_poisson_comparison Comparison of the Best Methods
  

  \subsection numerical_random_poisson_comparison_same Same Mean

  \image html random/poisson/same/sameCompareSmallArgument.jpg "Best methods for small means."
  \image latex random/poisson/same/sameCompareSmallArgument.pdf "Best methods for small means." width=0.5\textwidth

  \image html random/poisson/same/sameCompareLargeArgument.jpg "Best methods for large means."
  \image latex random/poisson/same/sameCompareLargeArgument.pdf "Best methods for large means." width=0.5\textwidth

  \image html random/poisson/same/sameCompareExpSmallArgument.jpg "Best overall methods that start with exponential inter-arrival."
  \image latex random/poisson/same/sameCompareExpSmallArgument.pdf "Best overall methods that start with exponential inter-arrival."

  \image html random/poisson/same/sameCompareExpLargeArgument.jpg "Best overall methods that start with exponential inter-arrival."
  \image latex random/poisson/same/sameCompareExpLargeArgument.pdf "Best overall methods that start with exponential inter-arrival."


  \image html random/poisson/same/sameCompareInvSmallArgument.jpg "Best overall methods that start with inversion."
  \image latex random/poisson/same/sameCompareInvSmallArgument.pdf "Best overall methods that start with inversion."

  \image html random/poisson/same/sameCompareInvLargeArgument.jpg "Best overall methods that start with inversion."
  \image latex random/poisson/same/sameCompareInvLargeArgument.pdf "Best overall methods that start with inversion."


  \image html random/poisson/same/sameCompareIfmSmallArgument.jpg "Best overall methods that start with inversion from the mode."
  \image latex random/poisson/same/sameCompareIfmSmallArgument.pdf "Best overall methods that start with inversion from the mode."

  \image html random/poisson/same/sameCompareIfmLargeArgument.jpg "Best overall methods that start with inversion from the mode."
  \image latex random/poisson/same/sameCompareIfmLargeArgument.pdf "Best overall methods that start with inversion from the mode."


  \image html random/poisson/same/sameCompareThirdSmallArgument.jpg "Best overall third-party methods."
  \image latex random/poisson/same/sameCompareThirdSmallArgument.pdf "Best overall third-party methods."

  \image html random/poisson/same/sameCompareThirdLargeArgument.jpg "Best overall third-party methods."
  \image latex random/poisson/same/sameCompareThirdLargeArgument.pdf "Best overall third-party methods."

  \image html random/poisson/same/sameCompareOverallSmallArgument.jpg "Best overall methods."
  \image latex random/poisson/same/sameCompareOverallSmallArgument.pdf "Best overall methods."

  \image html random/poisson/same/sameCompareOverallLargeArgument.jpg "Best overall methods."
  \image latex random/poisson/same/sameCompareOverallLargeArgument.pdf "Best overall methods."





  \subsection numerical_random_poisson_comparison_different Different Means

  \image html random/poisson/different/differentCompareSmallArgument.jpg "Best methods for small means."
  \image latex random/poisson/different/differentCompareSmallArgument.pdf "Best methods for small means." width=0.5\textwidth

  \image html random/poisson/different/differentCompareLargeArgument.jpg "Best methods for large means."
  \image latex random/poisson/different/differentCompareLargeArgument.pdf "Best methods for large means." width=0.5\textwidth

  \image html random/poisson/different/differentCompareExpSmallArgument.jpg "Best overall methods that start with exponential inter-arrival."
  \image latex random/poisson/different/differentCompareExpSmallArgument.pdf "Best overall methods that start with exponential inter-arrival."

  \image html random/poisson/different/differentCompareExpLargeArgument.jpg "Best overall methods that start with exponential inter-arrival."
  \image latex random/poisson/different/differentCompareExpLargeArgument.pdf "Best overall methods that start with exponential inter-arrival."

  \image html random/poisson/different/differentCompareInvSmallArgument.jpg "Best overall methods that start with inversion."
  \image latex random/poisson/different/differentCompareInvSmallArgument.pdf "Best overall methods that start with inversion."

  \image html random/poisson/different/differentCompareInvLargeArgument.jpg "Best overall methods that start with inversion."
  \image latex random/poisson/different/differentCompareInvLargeArgument.pdf "Best overall methods that start with inversion."

  \image html random/poisson/different/differentCompareIfmSmallArgument.jpg "Best overall methods that start with inversion from the mode."
  \image latex random/poisson/different/differentCompareIfmSmallArgument.pdf "Best overall methods that start with inversion from the mode."

  \image html random/poisson/different/differentCompareIfmLargeArgument.jpg "Best overall methods that start with inversion from the mode."
  \image latex random/poisson/different/differentCompareIfmLargeArgument.pdf "Best overall methods that start with inversion from the mode."

  \image html random/poisson/different/differentCompareThirdSmallArgument.jpg "Best overall third-party methods."
  \image latex random/poisson/different/differentCompareThirdSmallArgument.pdf "Best overall third-party methods."

  \image html random/poisson/different/differentCompareThirdLargeArgument.jpg "Best overall third-party methods."
  \image latex random/poisson/different/differentCompareThirdLargeArgument.pdf "Best overall third-party methods."

  \image html random/poisson/different/differentCompareOverallSmallArgument.jpg "Best overall methods."
  \image latex random/poisson/different/differentCompareOverallSmallArgument.pdf "Best overall methods."

  \image html random/poisson/different/differentCompareOverallLargeArgument.jpg "Best overall methods."
  \image latex random/poisson/different/differentCompareOverallLargeArgument.pdf "Best overall methods."






  \subsection numerical_random_poisson_comparison_distribution Distribution of Means

  \image html random/poisson/distribution/distributionCompareSmallArgument.jpg "Best methods for small means."
  \image latex random/poisson/distribution/distributionCompareSmallArgument.pdf "Best methods for small means." width=0.5\textwidth

  \image html random/poisson/distribution/distributionCompareLargeArgument.jpg "Best methods for large means."
  \image latex random/poisson/distribution/distributionCompareLargeArgument.pdf "Best methods for large means." width=0.5\textwidth

  \image html random/poisson/distribution/distributionCompareExpSmallArgument.jpg "Best overall methods that start with exponential inter-arrival."
  \image latex random/poisson/distribution/distributionCompareExpSmallArgument.pdf "Best overall methods that start with exponential inter-arrival."

  \image html random/poisson/distribution/distributionCompareExpLargeArgument.jpg "Best overall methods that start with exponential inter-arrival."
  \image latex random/poisson/distribution/distributionCompareExpLargeArgument.pdf "Best overall methods that start with exponential inter-arrival."

  \image html random/poisson/distribution/distributionCompareInvSmallArgument.jpg "Best overall methods that start with inversion."
  \image latex random/poisson/distribution/distributionCompareInvSmallArgument.pdf "Best overall methods that start with inversion."

  \image html random/poisson/distribution/distributionCompareInvLargeArgument.jpg "Best overall methods that start with inversion."
  \image latex random/poisson/distribution/distributionCompareInvLargeArgument.pdf "Best overall methods that start with inversion."

  \image html random/poisson/distribution/distributionCompareIfmSmallArgument.jpg "Best overall methods that start with inversion from the mode."
  \image latex random/poisson/distribution/distributionCompareIfmSmallArgument.pdf "Best overall methods that start with inversion from the mode."

  \image html random/poisson/distribution/distributionCompareIfmLargeArgument.jpg "Best overall methods that start with inversion from the mode."
  \image latex random/poisson/distribution/distributionCompareIfmLargeArgument.pdf "Best overall methods that start with inversion from the mode."

  \image html random/poisson/distribution/distributionCompareThirdSmallArgument.jpg "Best overall third-party methods."
  \image latex random/poisson/distribution/distributionCompareThirdSmallArgument.pdf "Best overall third-party methods."

  \image html random/poisson/distribution/distributionCompareThirdLargeArgument.jpg "Best overall third-party methods."
  \image latex random/poisson/distribution/distributionCompareThirdLargeArgument.pdf "Best overall third-party methods."

  \image html random/poisson/distribution/distributionCompareOverallSmallArgument.jpg "Best overall methods."
  \image latex random/poisson/distribution/distributionCompareOverallSmallArgument.pdf "Best overall methods."

  \image html random/poisson/distribution/distributionCompareOverallLargeArgument.jpg "Best overall methods."
  \image latex random/poisson/distribution/distributionCompareOverallLargeArgument.pdf "Best overall methods."
*/

END_NAMESPACE_NUMERICAL

#endif
