// -*- C++ -*-

/*! 
  \file numerical/random/discreteFinite.h
  \brief Includes the discrete, finite random number generator classes.
*/

#if !defined(__numerical_random_discreteFinite_h__)
#define __numerical_random_discreteFinite_h__

#include "discreteFinite/DiscreteFiniteGeneratorBinarySearch.h"
#include "discreteFinite/DiscreteFiniteGeneratorBinned.h"
#include "discreteFinite/DiscreteFiniteGeneratorBinsSplitting.h"
#include "discreteFinite/DiscreteFiniteGeneratorBinsSplittingStacking.h"
#include "discreteFinite/DiscreteFiniteGeneratorCdfInversionUsingPartialPmfSums.h"
#include "discreteFinite/DiscreteFiniteGeneratorCdfInversionUsingPartialRecursiveCdf.h"
#include "discreteFinite/DiscreteFiniteGeneratorLinearSearch.h"
#include "discreteFinite/DiscreteFiniteGeneratorLinearSearchInteger.h"
#include "discreteFinite/DiscreteFiniteGeneratorRejectionBinsSplitting.h"

BEGIN_NAMESPACE_NUMERICAL

/*!
  \page numerical_random_discreteFinite General Discrete Random Number Generators

  <!----------------------------------------------------------------------->
  \section numerical_random_discreteFinite_introduction Introduction

  Many common discrete distributions have probablity masses that can be written
  in the form of a function. For example 
  \f$\mathrm{pmf}_{\mu}(n) = \mathrm{e}^{-\mu} \mu^n / n!\f$ for
  the Poisson distribution with mean \f$\mu\f$. Generators for such a
  distribution usually take advantage of the structure of the 
  probablity mass function (PMF).
  For the general discrete, finite distribution, the PMF
  does not have any special form. (Note the "finite" qualification. 
  One can work with the infinite case on paper, but It doesn't
  make sense to design algorithms for them. Representing the PMF would require
  an infinite amount of storage since the PMF has no special structure.
  The "finite" qualification will hence be taken for granted.)

  The static case is well studied. Of course one can iterate over the PMF and
  perform the inversion. This linear search has linear computational 
  complexity. A better approach is to store the cumulative mass function (CMF)
  and do a binary search. This improves the complexity to \f$\log_2(N)\f$
  for \e N events. The best method is Walker's algorithm, which uses lookup
  tables to achieve constant complexity.

  This package was designed for the dynamic case; the PMF changes after
  generating each deviate. Both a binary search on the CMF and 
  Walker's algorithm require at least \f$\mathcal{O}(N)\f$
  operations to initialize their data structures. This makes them ill-suited
  to the dynamic case. This package has algorithms that can generate deviates
  and modify probabilities efficiently.

  Note that if most of the probability masses change after
  generating each deviate, one cannot do better than the simple linear 
  search. For this dense update case, even setting the new probability 
  masses has linear complexity. Thus the overall algorithm for generating 
  deviates cannot have better than linear complexity. Most of the algorithms in 
  this package are intended for the sparse update case: after generating
  a deviate, a small number of probablity masses are changed.

  Note that in dealing with the dynamic case, it is necessary to work
  with scaled probablities. That is, the "probabilities" are not
  required to sum to unity. (With regular probabilities, changing one
  of them necessitates changing all of them.)  Then the probability
  mass is the scaled probability divided by the sum of the scaled
  probabilities.  In the following we will refer to scaled
  probabilities simply as probabilities.

  <!----------------------------------------------------------------------->
  \section numerical_random_discreteFinite_linear Linear Search

  The simplest method of computing a discrete deviate is CMF inversion:
  - Compute a continuous uniform deviate \em r in the range (0..1).
  - Scale \r by the sum of the probabilities.
  - The deviate is the smallest \em n such that \f$\mathrm{cmf}(n) > r\f$.
  .
  There are various methods for CMF inversion. They differ in whether they
  store the CMF, how they order the events, and how they search for \em n.

  The simplest algorithm for CMF inversion is a linear search on the PMF.
  A PMF array with 16 events is depicted below.

  \image html random/discreteFinite/Pmf.png "A probablity mass function."

  Below is one way of implementing the linear search.
  \code
  template<typename RandomAccessConstIterator, typename T>
  int
  linearSearchChopDownUnguarded(RandomAccessConstIterator begin,
                                RandomAccessConstIterator end, T r) {
    RandomAccessConstIterator i = begin;
    for ( ; i != end && (r -= *i) > 0; ++i) {
    }
    return i - begin - (i == end);
  } \endcode
  \c begin and \c end are iterators to the beginning and end of the PMF 
  array. \c r is the scaled uniform deviate. This is called a chop-down 
  search because we chop-down the value of \c r until it is non-positive.
  It is a guarded search because we check that we do not go past the end 
  of the array. Note that the final line handles the special case that 
  round-off errors make us reach the end of the array. (Using an if statement
  to do this would be a little more expensive.) The function returns the 
  discrete deviate.

  There are many ways to implement a linear search. The differ in performance,
  but they all have  (surprise) linear computational complexity. On the
  other hand, modifying a probability has constant complexity; we simply change
  an array value. The linear search method is suitable for small problems.

  <!----------------------------------------------------------------------->
  \section numerical_random_discreteFinite_linearSorting Linear Search With Sorting

  Sorting the events in the order of descending probability may improve the 
  performance of the linear search. In order to use the sorted PMF array, one 
  needs two additional arrays of integers. The first stores the index of
  the event in the original PMF array. The index array is useful when generating
  the deviate. We can efficiently go from an element in the sorted PMF array
  to an event index. The second array stores the rank of the elements in 
  the original PMF array. This is useful in modifying probabilities. Here one 
  needs to access event probabilities by their index. More concretely,
  <tt>sortedPmf[rank[i]]</tt> is the same as <tt>pmf[i]</tt>.
  The sorted PMF array along with the index and rank arrays are depicted below.

  \image html random/discreteFinite/PmfSorted.png "The probability mass function sorted in descending order."

  Note that as the event probabilities change, one needs to re-sort the PMF 
  array to maintain it in approximately sorted order. 
  Sorting the events may improve performance if the probabilities differ by 
  a large amount. Otherwise, it may just add overhead. See the performance 
  results below for timings with various probability distributions.

  <!----------------------------------------------------------------------->
  \section numerical_random_discreteFinite_binary Binary Search

  Another method for CMF inversion is to store the CMF in an array and 
  perform a binary search to generate the deviate. The CMF array is depicted
  below.

  \image html random/discreteFinite/Cmf.png "Cumulative mass function."
  
  Generating a deviate has logarithmic computational complexity, which is 
  pretty good. However, modifying a probablity has linear complexity.
  After modifying an event's probability, the CMF must be recomputed starting
  at that event. The binary search method is suitable for small problems.

  Sorting the event probabilities is applicable to the binary search method.
  For each event one accumulates the probabilities of the influencing events.
  (Event \e a influences event \e b if the occurence of the former changes
  the subsequent probability for the latter.) Here one sorts the events in
  ascending order of accumulated influencing probability. The idea is to 
  minimize the portion of the CMF one needs to rebuild after modifying 
  a probability.

  <!----------------------------------------------------------------------->
  \section numerical_random_discreteFinite_double Double Linear Search

  One can speed up the linear search method by storing an array with 
  partial PMF sums. For instance one could have an array of length 
  \e N / 2 where each element holds the sum of two probabilities. Specifically,
  element \e i holds the sum of probabilities 2 \e i and 2 \e i + 1.
  One first performs a linear search on the short array. If the first search
  returns \e n, then the deviate is either 2 \e n or 2 \e n + 1. Examining 
  those elements in the PMF array determines which one. The cost of searching
  has roughly been cut in half. 
  
  If one stores the sum of three probablities in each element of the 
  additional array, then the cost of searching is 
  \f$\mathcal{O}(N / 3 + 3)\f$. There is a linear 
  search on an array of length \e N / 3, followed by a linear search on 
  three elements in the PMF array. Choosing the additional array to have 
  size \f$\sqrt{N}\f$ yields the best complexity, namely 
  \f$\mathcal{O}(\sqrt{N})\f$. This partial PMF sum array is depicted 
  below.
  
  \image html random/discreteFinite/PartialPmfSums.png "Partial PMF sums."
  
  The double linear search does have great complexity for generating deviates,
  but it has constant complexity for modifying probabilities. To change a 
  probability, one sets an element in the PMF array and then uses the 
  difference between the old and new values to update the appropriate element
  in the partial PMF sums array. Because of its simple design, the double 
  linear search has good performance for a wide range of problem sizes.
  
  "Tripling", "Quadrupling", etc., will also work. In general, by using \e p
  arrays of sizes \e N, \f$N^{(p-1)/p}\f$, \f$\ldots\f$, \f$N^{1/p}\f$, 
  generating a deviate has complexity \f$\mathcal{O}(p N^{1/p})\f$ and
  modifying a probability has complexity \f$\mathcal{O}(p)\f$. The performance 
  of these higher order methods depends on the problem size. Doubling often
  yields the most bang for the buck.

  <!----------------------------------------------------------------------->
  \section numerical_random_discreteFinite_partial Partial Recursive CMF

  One can build a partial recursive CMF that enables generating deviates
  and modifying probabilities in \f$\mathcal{O}(\log_2 N)\f$ time. The 
  process of building this data structure is shown below.

  \image html random/discreteFinite/PartialRecursiveCmf.png "The process of building the partial recursive CMF."
  
  One starts with the PMF. To every second element, add the previous element.
  Then to every fourth element \e i, add the element at position \e i - 2.
  Then to every eighth element \e i, add the element at position \e i - 4.
  After \f$\log_2 N\f$ steps, the final element holds the sum of the 
  probabilities.

  One can generate a deviate in \f$\mathcal{O}(\log_2 N) + 1\f$ steps.
  Modifying a probablity necessitates updating at most 
  \f$\mathcal{O}(\log_2 N) + 1\f$ elements of the partial recursive CMF array.
  See the source code for details.

  There are re-orderings of the above partial recursive CMF that also work.
  Below we show an alternative. In the order above, searching progresses 
  back-to-front and probability modifications proceed front-to-back.
  Vice-versa for the ordering below.

  \image html random/discreteFinite/PartialRecursiveCmfAlternate.png "Another partial recursive CMF."

  <!----------------------------------------------------------------------->
  \section numerical_random_discreteFinite_rejection Rejection
  
  Draw a rectangular around the PMF array. This is illustrated below.
  The rejection method for generating deviates is:
  - Randomly pick a point in the bounding box. 
  - If you hit one of the event boxes, its index is the discrete deviate.
  - Otherwise, pick again.

  \image html random/discreteFinite/Rejection.png "The rejection method."
  
  To pick a random point, you could use two random numbers, one for each 
  coordinate. However, a better method is to split a single random integer.
  For the case above, we could use the first four bits to pick a bin and use
  the remaining bits to determine the height.
  
  The efficiency of the rejection method is determined by the area of the 
  event boxes divided by the area of the bounding box. If the event 
  probabilities are similar, the efficiency will be high. If the probabilities
  differ by large amounts, the efficiency will be low. 
  
  <!----------------------------------------------------------------------->
  \section numerical_random_discreteFinite_binning Rejection with Binning.

  The rejection method is interesting, but it is not a useful
  technique by itself. Now we'll combine it with binning to obtain on
  optimal method for generating discrete deviates and modifying
  probabilities.  We distribute the event probabilities accross a
  number of bins in order to maximize the efficiency of the rejection
  method. Below we depict packing the probabilities into 32 bins. Each
  event is given one or more bins.  For this case, we can use the
  first five bits of a random integer to pick a bin and rest to
  compute a height. This gives us our random point for the rejection
  method.

  \image html random/discreteFinite/RejectionBinsSplitting.png "The rejection method with bins and splitting."
  
  As with many sophisticated methods, the devil is in the details. What is 
  an appropriate number of bins? How do you pack the bins in order to minimize
  the height of the bounding box? As the probabilities change, the efficiency
  may degrade. When should should you re-pack the bins? For answers to these
  questions, consult the class documentation and source code. Skipping to 
  the punchline: You can guarantee a high efficiency, and for an efficiency 
  \e E, the expected computational complexity of generating a deviate is 
  \f$\mathcal{O}(1/E)\f$. The data structure can be designed so that modifying
  an event probability involves updating 
  a single bin. Thus we have constant complexity for generating deviates
  and modifying probabilities.
  
  <!----------------------------------------------------------------------->
  \section numerical_random_discreteFinite_classes Classes

  This package provides the the following functors for computing 
  discrete, finite random deviates.
  - DiscreteFiniteGeneratorBinarySearch<false,Generator,T>
  - DiscreteFiniteGeneratorBinarySearch<true,Generator,T>
  - DiscreteFiniteGeneratorBinned
  - DiscreteFiniteGeneratorBinned
  - DiscreteFiniteGeneratorBinsSplitting<false,UseImmediateUpdate,BinConstants,Generator>
  - DiscreteFiniteGeneratorBinsSplitting<true,UseImmediateUpdate,BinConstants,Generator>
  - DiscreteFiniteGeneratorBinsSplittingStacking<false,UseImmediateUpdate,BinConstants,Generator>
  - DiscreteFiniteGeneratorBinsSplittingStacking<true,UseImmediateUpdate,BinConstants,Generator>
  - DiscreteFiniteGeneratorCdfInversionUsingPartialPmfSums
  - DiscreteFiniteGeneratorCdfInversionUsingPartialRecursiveCdf
  - DiscreteFiniteGeneratorLinearSearch
  - DiscreteFiniteGeneratorLinearSearchInteger
  - DiscreteFiniteGeneratorRejectionBinsSplitting<false,ExactlyBalance,BinConstants,Generator>
  - DiscreteFiniteGeneratorRejectionBinsSplitting<true,ExactlyBalance,BinConstants,Generator>

  I have implemented each of the discrete, finite deviate generators as 
  an <em>adaptable generator</em>, a functor that takes no arguments.
  (See \ref numerical_random_austern1999 "Generic Programming and the STL".)
  The classes are templated on the floating point number type
  (\c double by default) and the discrete, uniform generator type
  (DiscreteUniformGeneratorMt19937 by default).
  Below are a few ways of constructing a discrete, finite generator.
  \code
  // Use the default number type (double) and the default discrete, uniform generator.
  typedef numerical::DiscreteFiniteGeneratorBinarySearch<> Generator;
  Generator::DiscreteUniformGenerator uniform;
  Generator generator(&uniform); \endcode
  \code
  // Use single precision numbers and the default uniform deviate generator.
  typedef numerical::DiscreteFiniteGeneratorBinary<float> Generator;
  Generator::DiscreteUniformGenerator uniform;
  Generator generator(&uniform); \endcode
  \code
  // Use double precision numbers and the Mersenne twister generator from the GSL.
  typedef numerical::DiscreteFiniteGeneratorBinarySearch<double, numerical::DiscreteUniformGeneratorMt19937Gsl> Generator;
  Generator::DiscreteUniformGenerator uniform;
  Generator generator(&uniform); \endcode


  Each class defines the following types.
  - \c Number is the floating point number type.
  - \c DiscreteUniformGenerator is the discrete uniform generator type.
  - \c argument_type is \c void.
  - \c result_type is \c int.
  .
  The generators may be seeded with the seed() member function.  You can
  access the discrete uniform generator with getDiscreteUniformGenerator() .

  Each generator has the \c operator()() member function which generates 
  a deviate.
  \code
  int deviate = generator(); \endcode

  The tables below give the execution times (in nanoseconds) 
  for calling the discrete, finite deviate generators.

  Unit probabilities.
  \htmlinclude randomDiscreteFiniteStaticUnit.txt

  Uniformly distributed probabilities.
  \htmlinclude randomDiscreteFiniteStaticUniform.txt

  Geometric series with a factor of 10.
  \htmlinclude randomDiscreteFiniteStaticGeometric1.txt

  Geometric series with a factor of 100.
  \htmlinclude randomDiscreteFiniteStaticGeometric2.txt

  Geometric series with a factor of 1000.
  \htmlinclude randomDiscreteFiniteStaticGeometric3.txt

  Next we measure the execution time for generating a deviate and 
  trivially modifying that probability.  That is, we call setPmf()
  for the drawn deviate, but do not change the value of the probability.

  Unit probabilities.
  \htmlinclude randomDiscreteFiniteDynamicTrivialUnit.txt

  Uniformly distributed probabilities.
  \htmlinclude randomDiscreteFiniteDynamicTrivialUniform.txt
*/

END_NAMESPACE_NUMERICAL

#endif
