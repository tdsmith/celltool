// -*- C++ -*-

/*! 
  \file numerical/random/discreteFinite/DiscreteFiniteGeneratorLinearSearchInteger.h
  \brief Discrete, finite deviate.  Linear search.
*/

#if !defined(__numerical_DiscreteFiniteGeneratorLinearSearchInteger_h__)
#define __numerical_DiscreteFiniteGeneratorLinearSearchInteger_h__

#include "DfgPmfInteger.h"

#include "../uniform/Default.h"

#include <numeric>

// If we are debugging the whole numerical package.
#if defined(DEBUG_numerical) && !defined(DEBUG_DiscreteFiniteGeneratorLinearSearchInteger)
#define DEBUG_DiscreteFiniteGeneratorLinearSearchInteger
#endif

BEGIN_NAMESPACE_NUMERICAL

//! Discrete, finite deviate.  Linear search.
/*!
  \param _Pmf is the policy class that handles the probability mass function.
  By default it is DfgPmfAndSum<> .
  The \c Number type is inherited from this class.
  Because the different policies have different template parameters, this
  is a concrete class, and not a template template.
  \param Generator is the discrete, uniform generator.

  This class determines the deviate with a linear search on the probabilities.
*/
template<class _Pmf = DfgPmfInteger<>,
	 class _Generator = DISCRETE_UNIFORM_GENERATOR_DEFAULT>
class DiscreteFiniteGeneratorLinearSearchInteger :
  public _Pmf {
  //
  // Private types.
  //
private:

  //! The interface for the probability mass function and its sum.
  typedef _Pmf Base;

  //
  // Public types.
  //
public:

  //! The discrete uniform generator.
  typedef _Generator DiscreteUniformGenerator;
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

  //! The discrete uniform generator.
  DiscreteUniformGenerator* _discreteUniformGenerator;

  //
  // Not implemented.
  //
private:

  //! Default constructor not implemented.
  DiscreteFiniteGeneratorLinearSearchInteger();

  //--------------------------------------------------------------------------
  //! \name Constructors etc.
  //@{
public:

  //! Construct using the uniform generator.
  explicit
  DiscreteFiniteGeneratorLinearSearchInteger
  (DiscreteUniformGenerator* generator) :
    // The PMF array is empty.
    Base(),
    // Store a pointer to the discrete uniform generator.
    _discreteUniformGenerator(generator)
  {}

  //! Construct from the uniform generator and the probability mass function.
  template<typename ForwardIterator>
  DiscreteFiniteGeneratorLinearSearchInteger
  (DiscreteUniformGenerator* generator,
   ForwardIterator begin, ForwardIterator end) :
    Base(),
    // Store a pointer to the discrete uniform generator.
    _discreteUniformGenerator(generator) {
    initialize(begin, end);
  }

  //! Copy constructor.
  /*!
    \note The discrete, uniform generator is not copied.  Only the pointer
    to it is copied.
  */
  DiscreteFiniteGeneratorLinearSearchInteger
  (const DiscreteFiniteGeneratorLinearSearchInteger& other) :
    Base(other),
    _discreteUniformGenerator(other._discreteUniformGenerator)
  {}

  //! Assignment operator.
  /*!
    \note The discrete, uniform generator is not copied.  Only the pointer
    to it is copied.
  */
  DiscreteFiniteGeneratorLinearSearchInteger&
  operator=(const DiscreteFiniteGeneratorLinearSearchInteger& other) {
    if (this != &other) {
      Base::operator=(other);
      _discreteUniformGenerator = other._discreteUniformGenerator;
    }
    return *this;
  }

  //! Destructor.
  /*!
    The memory for the discrete, uniform generator is not freed.
  */
  ~DiscreteFiniteGeneratorLinearSearchInteger()
  {}

  //@}
  //--------------------------------------------------------------------------
  //! \name Random number generation.
  //@{
public:

  //! Seed the uniform random number generator.
  void
  seed(const typename DiscreteUniformGenerator::result_type seedValue) {
    _discreteUniformGenerator->seed(seedValue);
  }

  //! Return a discrete, finite deviate.
  result_type
  operator()() {
    result_type index;
    do {
      index = Base::operator()((*_discreteUniformGenerator)());
    } while (getPmf(index) == 0);
    return index;
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

  // CONTINUE
  //! Set the probability mass function with the specified index.
  //using Base::setPmf;

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
