// -*- C++ -*-

/*! 
  \file numerical/random/discreteFinite/DiscreteFiniteGeneratorLinearSearch.h
  \brief Discrete, finite deviate.  Linear search.
*/

#if !defined(__numerical_DiscreteFiniteGeneratorLinearSearch_h__)
#define __numerical_DiscreteFiniteGeneratorLinearSearch_h__

#include "DfgPmfAndSum.h"

#include "../uniform/ContinuousUniformGenerator.h"

#include <numeric>

// If we are debugging the whole numerical package.
#if defined(DEBUG_numerical) && !defined(DEBUG_DiscreteFiniteGeneratorLinearSearch)
#define DEBUG_DiscreteFiniteGeneratorLinearSearch
#endif

BEGIN_NAMESPACE_NUMERICAL


//! Discrete, finite deviate.  Linear search.
/*!
  \param Pmf is the policy class that handles the probability mass function.
  By default it is DfgPmfAndSum<> .
  The \c Number type is inherited from this class.
  Because the different policies have different template parameters, this
  is a concrete class, and not a template template.
  \param Generator is the discrete, uniform generator.

  This class determines the deviate with a linear search on the probabilities.
*/
template<class _PmfAndSum = DfgPmfAndSum<>,
	 class _Generator = DISCRETE_UNIFORM_GENERATOR_DEFAULT>
class DiscreteFiniteGeneratorLinearSearch :
  public _PmfAndSum {
  //
  // Private types.
  //
private:

  //! The interface for the probability mass function and its sum.
  typedef _PmfAndSum Base;

  //
  // Public types.
  //
public:

  //! The discrete uniform generator.
  typedef _Generator DiscreteUniformGenerator;
  //! The continuous uniform generator.
  typedef ContinuousUniformGeneratorClosed<DiscreteUniformGenerator>
  ContinuousUniformGenerator;
  //! The number type.
  typedef typename Base::Number Number;
  //! The argument type.
  typedef void argument_type;
  //! The result type.
  typedef int result_type;

  //
  // Member data.
  //
protected:

  //! The continuous uniform generator.
  ContinuousUniformGenerator _continuousUniformGenerator;

  //
  // Not implemented.
  //
private:

  //! Default constructor not implemented.
  DiscreteFiniteGeneratorLinearSearch();

  //--------------------------------------------------------------------------
  //! \name Constructors etc.
  //@{
public:

  //! Construct using the uniform generator.
  explicit
  DiscreteFiniteGeneratorLinearSearch(DiscreteUniformGenerator* generator) :
    // The PMF array is empty.
    Base(),
    // Make a continuous uniform generator using the discrete uniform generator.
    _continuousUniformGenerator(generator)
  {}

  //! Construct from the uniform generator and the probability mass function.
  template<typename ForwardIterator>
  DiscreteFiniteGeneratorLinearSearch(DiscreteUniformGenerator* generator,
				      ForwardIterator begin,
				      ForwardIterator end) :
    Base(),
    // Make a continuous uniform generator using the discrete uniform generator.
    _continuousUniformGenerator(generator) {
    initialize(begin, end);
  }

  //! Copy constructor.
  /*!
    \note The discrete, uniform generator is not copied.  Only the pointer
    to it is copied.
  */
  DiscreteFiniteGeneratorLinearSearch
  (const DiscreteFiniteGeneratorLinearSearch& other) :
    Base(other),
    _continuousUniformGenerator(other._continuousUniformGenerator)
  {}

  //! Assignment operator.
  /*!
    \note The discrete, uniform generator is not copied.  Only the pointer
    to it is copied.
  */
  DiscreteFiniteGeneratorLinearSearch&
  operator=(const DiscreteFiniteGeneratorLinearSearch& other) {
    if (this != &other) {
      Base::operator=(other);
      _continuousUniformGenerator = other._continuousUniformGenerator;
    }
    return *this;
  }

  //! Destructor.
  /*!
    The memory for the discrete, uniform generator is not freed.
  */
  ~DiscreteFiniteGeneratorLinearSearch()
  {}

  //@}
  //--------------------------------------------------------------------------
  //! \name Random number generation.
  //@{
public:

  //! Seed the uniform random number generator.
  void
  seed(const typename DiscreteUniformGenerator::result_type seedValue) {
    _continuousUniformGenerator.seed(seedValue);
  }

  //! Return a discrete, finite deviate.
  result_type
  operator()() {
    result_type index;
    do {
      index = Base::operator()(_continuousUniformGenerator() * getPmfSum());
    } while (getPmf(index) == 0);
    return index;
#if 0
    // REMOVE
    // Method that does not check the validity.
    return Base::operator()(_continuousUniformGenerator() * getPmfSum());
#endif
  }

  //@}
  //--------------------------------------------------------------------------
  //! \name Accessors.
  //@{
public:

  //! Get the probability mass function with the specified index.
  using Base::getPmf;

  //! Get the number of possible deviates.
  using Base::getSize;

  //! Get the sum of the probability mass functions.
  using Base::getPmfSum;

  //! Return true if the sum of the PMF is positive.
  using Base::isValid;

  //! Get the number of steps between repairs.
  using Base::getStepsBetweenRepairs;

  //! Get the number of steps between rebuilds.
  /*!
    \note You can use this only if the PMF base class utilizes rebuilding.
  */
  int
  getStepsBetweenRebuilds() const {
    return Base::getStepsBetweenRebuilds();
  }

  //@}
  //--------------------------------------------------------------------------
  //! \name Equality.
  //@{
public:

  using Base::operator==;

  //@}
  //--------------------------------------------------------------------------
  //! \name Manipulators.
  //@{
public:

  //! Initialize the probability mass function.
  using Base::initialize;

  //! Set the probability mass function with the specified index.
  using Base::setPmf;

  //! Recompute the sum of the PMF.
  void
  updatePmf() {
    Base::updatePmf();
  }

  //! Set the number of steps between repairs.
  using Base::setStepsBetweenRepairs;

  //! Set the number of steps between rebuilds.
  /*!
    \note You can use this only if the PMF base class utilizes rebuilding.
  */
  void
  setStepsBetweenRebuilds(const int n) {
    Base::setStepsBetweenRebuilds(n);
  }

  //@}
  //--------------------------------------------------------------------------
  //! \name File I/O.
  //@{
public:

  //! Print information about the data structure.
  using Base::print;

  //@}
};


END_NAMESPACE_NUMERICAL

#endif
