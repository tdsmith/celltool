// -*- C++ -*-

/*! 
  \file numerical/random.h
  \brief Includes the random number generator classes.
*/

#if !defined(__numerical_random_h__)
#define __numerical_random_h__

// Discrete, finite deviates.
#include "discreteFinite.h"
// Exponential
#include "exponential.h"
// Exponential
#include "gamma.h"
// Normal (Gaussian) deviates.
#include "normal.h"
// Poisson deviates.
#include "poisson.h"
// Uniform random deviates.
#include "uniform.h"

BEGIN_NAMESPACE_NUMERICAL

/*!
  \page numerical_random Random Number Package

  This package has functors for generating random deviates with 
  \ref numerical_random_uniform "uniform",
  \ref numerical_random_discreteFinite "discrete, finite",
  \ref numerical_random_exponential "exponential",
  \ref numerical_random_gamma "gamma",
  \ref numerical_random_normal "normal", and 
  \ref numerical_random_poisson "Poisson"
  distributions.  
  The code for measuring the performance of the algorithms is 
  in <code>stlib/performance/numerical/random</code>.  
  There are makefiles for running
  the performance code and gnuplot scripts for generating graphs in the
  <code>stlib/results/numerical/random</code> directory.
  For the performance results presented herein, the testing code was compiled 
  with GNU g++ 4.0 using the flags: 
  <code>-O3 -funroll-loops -fstrict-aliasing</code>.
  I ran the tests on a Mac Mini with a 1.66 GHz Intel Core Duo processor and
  512 MB DDR2 SDRAM.

  Some of the generators use the 
  <a href="http://www.gnu.org/software/gsl/">GNU Scientific Library</a>.
  (Namely the the ones with a "Gsl" suffix.)  
  To use these, you must have GSL installed
  on your system.  Also, you should define appropriate environment variables 
  so the compiler can find the GSL header files and libraries.  For example,
  with the GNU compiler define <strong>CPATH</strong> to the include directory
  and <strong>LIBRARY_PATH</strong> to the library directory.
  If you do not use these generators, you don't need to have GSL installed.

  Links:
  - <a href="http://www.math.sci.hiroshima-u.ac.jp/~m-mat/MT/emt.html">Mersenne
  Twister Home Page</a>
  - http://random.mat.sbg.ac.at/
*/

//----------------------------------------------------------------------------
/*!
\page numerical_random_bibliography Bibliography for numerical/random.

\anchor numerical_random_ahrens1974
J. H. Ahrens and U. Dieter, 
"Computer methods for sampling from gamma, beta, Poisson and binomial 
distributions" 
Computing, 
Vol. 12, 1974, pp. 223-246.

\anchor numerical_random_devroye1986
Luc Devroye,
"Non-Uniform Random Variate Generation"
Springer-Verlag, New York, 1986.
http://cg.scs.carleton.ca/~luc/rnbookindex.html

\anchor numerical_random_marsaglia2000
George Marsaglia and Wai Wan Tsang
"The ziggurat method for generating random variables" 
Journal of Statistical Software, 
Vol. 5, 2000, Issue 8.
http://www.jstatsoft.org/v05/i08/

\anchor numerical_random_gammaMarsaglia2000
George Marsaglia and Wai Wan Tsang
"A Simple Method for Generating Gamma Variables" 
ACM Transactions on Mathematical Software, 
Vol. 26, No. 3, 2000, pp. 363-372.

\anchor numerical_random_knuth1998
Donald E. Knuth
"The Art of Computer Programming - Seminumerical Algorithms"
Addison-Wesley, 1998.

\anchor numerical_random_press2002
William H. Press, Saul A. Teukolsky, William T. Vetterling, and Brian P. Flannery,
"Numerical Recipes in C++"
Cambridge University Press, Cambridge, UK, 2002.

\anchor numerical_random_austern1999
Matthew H. Austern,
"Generic Programming and the STL"
Addison Wesley, Reading, Massachusetts, 1999.
*/

END_NAMESPACE_NUMERICAL

#endif
