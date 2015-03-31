// -*- C++ -*-

/*! 
  \file numerical/random/discreteFinite/DfgPmfInteger.h
  \brief Probability mass function for a discrete, finite generator.
*/

#if !defined(__numerical_DfgPmfInteger_h__)
#define __numerical_DfgPmfInteger_h__

#include "DfgPmfAndSum.h"
#include "DfgPmf.h"

#include "../../../ads/array/Array.h"

#include <limits>

// If we are debugging the whole numerical package.
#if defined(DEBUG_numerical) && !defined(DEBUG_numerical_DfgPmfInteger)
#define DEBUG_numerical_DfgPmfInteger
#endif

BEGIN_NAMESPACE_NUMERICAL

//! Whether to use branching and meta-programming.
template<bool _IsDynamic, bool _UseBranching, typename _Integer>
class TraitsForDynamicAndBranchingAndInteger {
public:
  //! Whether it is dynamic.
  static const bool IsDynamic = _IsDynamic;
  //! Whether to use branching.
  static const bool UseBranching = _UseBranching;
  //! The integer type.
  typedef _Integer Integer;
};


//! Probability mass function for a discrete, finite generator.
/*!
  \param T The number type.  By default it is double.

  Manage the probability mass function.
*/
template<class PmfAndSum = DfgPmfAndSum<DfgPmf<> >,
	 class Traits = TraitsForDynamicAndBranchingAndInteger<false /*CONTINUE*/, true, unsigned> >
class DfgPmfInteger;


//! Probability mass function for a discrete, finite generator.
/*!
  \param T The number type.  By default it is double.

  Manage the probability mass function.  Use an array of unsigned integers
  that represent the probabilities with fixed precision.

  \note  This is just a test.  It does not yet have dynamic capability.
*/
template<class PmfAndSum, bool _UseBranching, typename _Integer>
class DfgPmfInteger<PmfAndSum, TraitsForDynamicAndBranchingAndInteger<false, _UseBranching, _Integer> > :
public PmfAndSum {
  //
  // Private types.
  //
private:

  //! The PMF and sum base.
  typedef PmfAndSum Base;

  //
  // Public types.
  //
public:

  //! The floating point number type.
  typedef typename Base::Number Number;

  //
  // More private types.
  //
private:

  //! The traits.
  typedef TraitsForDynamicAndBranchingAndInteger<false, _UseBranching, _Integer> Traits;
  //! The integer type.
  typedef typename Traits::Integer Integer;

  //! The array type for floating point numbers.
  typedef ads::Array<1, Number> NumberContainer;
  //! The array type for integers.
  typedef ads::Array<1, Integer> IntegerContainer;
  
  //
  // More public types.
  //
public:

  //! An iterator on the probabilities.
  typedef typename NumberContainer::iterator Iterator;
  //! A const iterator on the probabilities.
  typedef typename NumberContainer::const_iterator ConstIterator;

  //
  // Member data.
  //
private:

  //! Fixed precision probability mass function.
  IntegerContainer _ipmf;

  //--------------------------------------------------------------------------
  //! \name Constructors etc.
  //@{
protected:

  //! Default constructor.
  DfgPmfInteger() :
    Base(),
    // The PMF array is empty.
    _ipmf()
  {}

  //! Construct from the probability mass function.
  template<typename ForwardIterator>
  DfgPmfInteger(ForwardIterator begin, ForwardIterator end) :
    Base(),
    _ipmf() {
    initialize(begin, end);
  }

  //! Copy constructor.
  DfgPmfInteger(const DfgPmfInteger& other) :
    Base(other),
    _ipmf(other._ipmf)
  {}

  //! Assignment operator.
  DfgPmfInteger&
  operator=(const DfgPmfInteger& other) {
    if (this != &other) {
      Base::operator=(other);
      _ipmf = other._ipmf;
    }
    return *this;
  }

  //! Destructor.
  ~DfgPmfInteger()
  {}

  //@}
  //--------------------------------------------------------------------------
  //! \name Random number generation.
  //@{
protected:

  //! Return a discrete, finite deviate.
  /*!
    Use a linear search to sum probabilities until the sum reaches r.
    For this version, you specify an offset when you have allready subtracted
    a number of the probabilities.
  */
  int
  operator()(const unsigned r, const int offset) const {
    return Base::getIndex(computeDeviate
			  (r, offset, Loki::Int2Type<Traits::UseBranching>()));
  }

  //! Return a discrete, finite deviate.
  /*!
    Use a linear search to sum probabilities until the sum reaches r.
  */
  int
  operator()(const unsigned r) const {
    return Base::getIndex(computeDeviate
			  (r, Loki::Int2Type<Traits::UseBranching>()));
  }

private:

  int
  computeDeviate(const unsigned r, const int offset, 
		 Loki::Int2Type<true> /*UseBranching*/) const {
    return offset + 
      linearSearchChopDownUnguarded(_ipmf.begin() + offset, _ipmf.end(), r);
  }

  int
  computeDeviate(const unsigned r, const int offset, 
		 Loki::Int2Type<false> /*UseBranching*/) const {
    // CONTINUE:  I divide by 2 to get an integer.
    return offset + 
      linearSearchChopDownNoBranching(_ipmf.begin() + offset, _ipmf.end(), 
				      int(r / 2));
  }

  int
  computeDeviate(const unsigned r, Loki::Int2Type<true> /*UseBranching*/) 
    const {
    return linearSearchChopDownUnguarded(_ipmf.begin(), _ipmf.end(), r);
  }

  int
  computeDeviate(const unsigned r, Loki::Int2Type<false> /*UseBranching*/)
    const {
    // CONTINUE:  I divide by 2 to get an integer.
    return linearSearchChopDownNoBranching(_ipmf.begin(), _ipmf.end(), 
					   int(r / 2));
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

protected:

  //! Get the beginning of the probabilities in the PMF.
  using Base::getPmfBeginning;

  //! Get the end of the probabilities in the PMF.
  using Base::getPmfEnd;

  //@}
  //--------------------------------------------------------------------------
  //! \name Equality.
  //@{
public:

  bool
  operator==(const DfgPmfInteger& other) const {
    return Base::operator==(other) && _ipmf == other._ipmf;
  }

  //@}
  //--------------------------------------------------------------------------
  //! \name Manipulators.
  //@{
public:

  //! Initialize the probability mass function.
  template<typename ForwardIterator>
  void
  initialize(ForwardIterator begin, ForwardIterator end) {
    // Initialize the floating point PMF and the sum.
    Base::initialize(begin, end);

    //
    // Initialize the fixed precision PMF.
    // The integer array will sum to exactly 
    // std::numeric_limits<Integer>::max().
    //

    // Resize the array.
    _ipmf.resize(getSize());
    Number sum = getPmfSum();
    Integer iSum = std::numeric_limits<Integer>::max();
    for (int i = 0; i < _ipmf.size() - 1; ++i) {
      // Note: I use getPmfBeginning() instead of getPmf() because the 
      // array may be sorted.
      _ipmf[i] = Integer(Number(iSum) * getPmfBeginning()[i] / sum);
      sum -= getPmfBeginning()[i];
      iSum -= _ipmf[i];
    }
    // Set the last element.
    *(_ipmf.end() - 1) = iSum;
  }

  //! Set the number of steps between repairs.
  using Base::setStepsBetweenRepairs;

protected:

  //! Get the beginning of the probabilities in the PMF.
  //using Base::getPmfBeginning;

  //! Get the end of the probabilities in the PMF.
  //using Base::getPmfEnd;

  //@}
  //--------------------------------------------------------------------------
  //! \name File I/O.
  //@{
protected:

  //! Print information about the data structure.
  void
  print(std::ostream& out) const {
    Base::print(out);
    out << "Integer PMF = \n" << _ipmf << "\n";
  }

  //@}
};


END_NAMESPACE_NUMERICAL

#endif
