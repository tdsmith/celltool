// -*- C++ -*-

/*! 
  \file numerical/random/uniform.h
  \brief Includes the uniform random number generator classes.
*/

#if !defined(__numerical_random_uniform_h__)
#define __numerical_random_uniform_h__

// Uniform random deviates.
#include "uniform/DiscreteUniformGenerator0Nr.h"
#include "uniform/DiscreteUniformGenerator1Nr.h"
#include "uniform/DiscreteUniformGenerator2Nr.h"
#include "uniform/DiscreteUniformGeneratorMc32.h"
#include "uniform/DiscreteUniformGeneratorMt19937MatsumotoNishimura.h"
#include "uniform/DiscreteUniformGeneratorMt19937MatsumotoNishimuraCached.h"
#include "uniform/DiscreteUniformGeneratorMt19937Gsl.h"
#include "uniform/DiscreteUniformGeneratorTt800.h"

// Default generator
#include "uniform/Default.h"
// Continuous deviates
#include "uniform/ContinuousUniformGenerator.h"

BEGIN_NAMESPACE_NUMERICAL

/*!
  \page numerical_random_uniform Generators for Uniform Deviates

  This package has a number of generators for discrete, uniform deviates.
  Most of these compute unsigned, 32-bit integer deviates.
  - DiscreteUniformGeneratorMt19937
  The Mersenne Twister algorithm.  It generates
  deviates with a period of \f$2^{19937} - 1\f$.  
  - DiscreteUniformGeneratorTt800
  Matsumoto's TT800 algorithm, which has a period of \f$2^{800}\f$.  
  - DiscreteUniformGeneratorMc32 Multiplicative congruential generators.
  By default I use Marsaglia's super-duper parameter.
  For odd seeds, it has a period of \f$2^{30}\f$.
  - DiscreteUniformGeneratorMt19937Gsl 
  The GSL implementation of the Mersenne Twister.
  - DiscreteUniformGeneratorNr0 Numerical Recipes' minimal 
  generator of Park and Miller.  
  - DiscreteUniformGeneratorNr1 Numerical Recipes' minimal 
  generator of Park and Miller with Bays-Durham shuffle.
  - DiscreteUniformGeneratorNr2 Numerical Recipes' long period 
  generator of L'Ecuyer with Bays-Durham shuffle.

  \note The methods from 
  \ref numerical_random_press2002 "Numerical Recipes"
  are each linear congruential generators.  They generate <em>31-bit</em>
  random numbers.  
  They are adapted from Fortran routines, and Fortran does not have the
  unsigned integer type.  Thus they use \c int internally and may be seeded 
  with an \c int.

  One can transform a discrete deviate to a uniform deviate using
  simple \ref numerical_random_uniform_continuous "conversion functions".
  There are also the functors ContinuousUniformGeneratorOpen and
  ContinuousUniformGeneratorClosed which generate continuous, uniform 
  deviates for the ranges (0..1) and [0..1], respectively.
  These classes are templated on the floating point number type.  By 
  default is is \c double.  

  Below is a table that summarizes some important properties of the 
  continuous, uniform distribution on the interval (0..1).
  <table>
  <tr> <th> Probability density function 
  <td> \f$\mathrm{pdf}(x) = 1\f$
  <tr> <th> Cumulative distribution function 
  <td> \f$\mathrm{cdf}(x) = x\f$
  <tr> <th> Mean <td> 1/2
  <tr> <th> Median <td> 1/2
  <tr> <th> Mode <td> any value in (0..1)
  <tr> <th> Variance <td> 1/12
  <tr> <th> Skewness <td> 0
  <tr> <th> Kurtosis <td> -6 / 5
  </table>


  I have implemented each of the random number generators as 
  an <em>adaptable generator</em>, a functor that takes no arguments.
  (See \ref numerical_random_austern1999 "Generic Programming and the STL".)
  One can construct a discrete, uniform generator in the following ways.
  \code
  // The default constructor uses an appropriate default seed.
  numerical::DiscreteUniformGeneratorMt19937 a;
  // Construct and seed with 123.
  numerical::DiscreteUniformGeneratorMt19937 b(123U);\endcode

  Each class defines the following types.
  - \c Number is the floating point number type.
  - \c argument_type is \c void.
  - \c result_type is \c unsigned.  (Except for the classes adapted from 
  "Numerical Recipes" for which it is \c int .)

  Each generator has the following member functions.
  - \c operator()() return a discrete, uniform deviate.
  - \c seed() Seed the random number generator.
  .
  \code
  // Define the discrete, uniform generator type.
  typedef numerical::DiscreteUniformGeneratorMt19937 Generator;
  // Define the integer type.
  typedef Generator::result_type Integer;
  // Use the default constructor to make a generator.
  Generator generator;
  // Draw a deviate.
  Integer i = generator();
  // Seed the generator.
  Integer s = 123U;
  generator.seed(s);
  \endcode

  The default uniform deviate generator is defined to be
  DiscreteUniformGeneratorMt19937 with the macro 
  \c DISCRETE_UNIFORM_DEVIATE_DEFAULT in the file 
  numerical/random/uniform/Default.h .  
  The other generators in this package
  (\ref numerical_random_discreteFinite "discrete, finite",
  \ref numerical_random_exponential "exponential",
  \ref numerical_random_gamma "gamma",
  \ref numerical_random_normal "normal", and
  \ref numerical_random_poisson "Poisson")
  are all templated on the discrete, uniform deviate generator type.  
  They use this macro to define the default generator.
  If you would like to change the default, just define the macro before 
  you include files from this package.

  \note I assume that \c unsigned is a 32-bit integer, so you probably won't 
  be able to run this code on your toaster oven.
  
  The table below gives the execution times for computing discrete
  deviates and continuous deviates.
  The execution times are given in nanoseconds.

  <table>
  <tr> 
  <th> Functor 
  <th> Discrete
  <th> Continuous Closed
  <th> Continuous Open
  <tr> 
  <td> DiscreteUniformGeneratorMt19937
  <td> 6.1
  <td> 11.4
  <td> 12.4
  <tr> 
  <td> DiscreteUniformGeneratorTt800
  <td> 4.3
  <td> 9.5
  <td> 10.1
  <tr> 
  <td> DiscreteUniformGeneratorMc32
  <td> 3.9
  <td> 4.3
  <td> 5.2
  <!--
  <tr> 
  <td> DiscreteUniformGeneratorMt19937Gsl
  <td> 
  <td> 
  <td> 
  -->
  <tr> 
  <td> DiscreteUniformGeneratorNr0
  <td> 13
  <td> 13.9
  <td> 14
  <tr> 
  <td> DiscreteUniformGeneratorNr1
  <td> 14.7
  <td> 14.7
  <td> 14.9
  <tr> 
  <td> DiscreteUniformGeneratorNr2
  <td> 26.7
  <td> 27.7
  <td> 27.9
  </table>

  Timings on SHC using the Pathscale compiler.

  Timings on ASAP using the Intel compiler.

  <table>
  <tr> 
  <th> Functor 
  <th> Discrete
  <th> Continuous Closed
  <th> Continuous Open
  <tr> 
  <td> DiscreteUniformGeneratorMt19937
  <td> 34.5
  <td> 72.6
  <td> 55.3
  <tr> 
  <td> DiscreteUniformGeneratorTt800
  <td> 16.8
  <td> 29.8
  <td> 60.4
  <tr> 
  <td> DiscreteUniformGeneratorMc32
  <td> 4.1
  <td> 4.7
  <td> 5.8
  <!--
  <tr> 
  <td> DiscreteUniformGeneratorMt19937Gsl
  <td> 
  <td> 
  <td> 
  -->
  <tr> 
  <td> DiscreteUniformGeneratorNr0
  <td> 23.2
  <td> 23.4
  <td> 32.1
  <tr> 
  <td> DiscreteUniformGeneratorNr1
  <td> 24.3
  <td> 26.5
  <td> 30.6
  <tr> 
  <td> DiscreteUniformGeneratorNr2
  <td> 67.4
  <td> 103.8
  <td> 88.7
  </table>

*/

END_NAMESPACE_NUMERICAL

#endif
