// -*- C++ -*-

/*! 
  \file numerical/random/discreteFinite/DfgPmf.h
  \brief Probability mass function for a discrete, finite generator.
*/

#if !defined(__numerical_DfgPmf_h__)
#define __numerical_DfgPmf_h__

#include "linearSearch.h"

#include "../../../ads/array/Array.h"

#include <limits>

// If we are debugging the whole numerical package.
#if defined(DEBUG_numerical) && !defined(DEBUG_numerical_DfgPmf)
#define DEBUG_numerical_DfgPmf
#endif

BEGIN_NAMESPACE_NUMERICAL

//! Whether to use Branching.
template<bool Value>
class TraitsForBranching {
public:
  //! Whether to use branching.
  static const bool UseBranching = Value;
};


//! Probability mass function for a discrete, finite generator.
/*!
  \param T The number type.  By default it is double.

  Manage the probability mass function.
*/
template<class Traits = TraitsForBranching<true>, typename T = double>
class DfgPmf {
  //
  // Public types.
  //
public:

  //! The number type.
  typedef T Number;

  //
  // Private types.
  //
private:

  //! The array type.
  typedef ads::Array<1, Number> Container;
  
  //
  // More public types.
  //
public:

  //! An iterator on the probabilities.
  typedef typename Container::iterator Iterator;
  //! A const iterator on the probabilities.
  typedef typename Container::const_iterator ConstIterator;

  //
  // Member data.
  //
private:

  //! Probability mass function.  (This is scaled and may not sum to unity.)
  Container _pmf;

  //--------------------------------------------------------------------------
  //! \name Constructors etc.
  //@{
protected:

  //! Default constructor.
  DfgPmf() :
    // The PMF array is empty.
    _pmf()
  {}

  //! Construct from the probability mass function.
  template<typename ForwardIterator>
  DfgPmf(ForwardIterator begin, ForwardIterator end) :
    _pmf(begin, end)
  {}

  //! Copy constructor.
  DfgPmf(const DfgPmf& other) :
    _pmf(other._pmf)
  {}

  //! Assignment operator.
  DfgPmf&
  operator=(const DfgPmf& other) {
    if (this != &other) {
      _pmf = other._pmf;
    }
    return *this;
  }

  //! Destructor.
  ~DfgPmf()
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
  operator()(const Number r, const int offset) const {
    return computeDeviate(r, offset, Loki::Int2Type<Traits::UseBranching>());
  }

  //! Return a discrete, finite deviate.
  /*!
    Use a linear search to sum probabilities until the sum reaches r.
  */
  int
  operator()(const Number r) const {
    return computeDeviate(r, Loki::Int2Type<Traits::UseBranching>());
  }

private:

  int
  computeDeviate(Number r, const int offset, 
		 Loki::Int2Type<true> /*UseBranching*/) const {
    return offset + 
      linearSearchChopDownUnguarded(_pmf.begin() + offset, _pmf.end(), r);
  }

  int
  computeDeviate(Number r, const int offset, 
		 Loki::Int2Type<false> /*UseBranching*/) const {
    return offset + 
      linearSearchChopDownNoBranching(_pmf.begin() + offset, _pmf.end(), r);
  }

  int
  computeDeviate(Number r, Loki::Int2Type<true> /*UseBranching*/) const {
    return linearSearchChopDownUnguarded(_pmf.begin(), _pmf.end(), r);
  }

  int
  computeDeviate(Number r, Loki::Int2Type<false> /*UseBranching*/) const {
    return linearSearchChopDownNoBranching(_pmf.begin(), _pmf.end(), r);
  }

  //@}
  //--------------------------------------------------------------------------
  //! \name Accessors.
  //@{
public:

  //! Get the probability mass function with the specified index.
  Number
  getPmf(const int index) const {
    return _pmf[index];
  }

  //! Get the number of possible deviates.
  int
  getSize() const {
    return _pmf.size();
  }

protected:

  //! Get the beginning of the probabilities in the PMF.
  ConstIterator
  getPmfBeginning() const {
    return _pmf.begin();
  }

  //! Get the end of the probabilities in the PMF.
  ConstIterator
  getPmfEnd() const {
    return _pmf.end();
  }

  //! Get the index of the specified element.
  int
  getIndex(const int n) const {
    return n;
  }

  //@}
  //--------------------------------------------------------------------------
  //! \name Equality.
  //@{
public:

  bool
  operator==(const DfgPmf& other) const {
    return _pmf == other._pmf;
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
    _pmf.rebuild(begin, end);
  }

  //! Set the probability mass function with the specified index.
  void
  setPmf(int index, Number value) {
    _pmf[index] = value;
  }

  //! Set the probability mass functions.
  template<typename _RandomAccessIterator>
  void
  setPmf(_RandomAccessIterator iterator) {
    for (int i = 0; i != _pmf.size(); ++i) {
      _pmf[i] = iterator[i];
    }
  }

protected:

  //! Do nothing.
  void
  updatePmf() {
  }

  //! Get the beginning of the probabilities in the PMF.
  Iterator
  getPmfBeginning() {
    return _pmf.begin();
  }

  //! Get the end of the probabilities in the PMF.
  Iterator
  getPmfEnd() {
    return _pmf.end();
  }

  //@}
  //--------------------------------------------------------------------------
  //! \name File I/O.
  //@{
protected:

  //! Print information about the data structure.
  void
  print(std::ostream& out) const {
    out << "PMF = \n" << _pmf << "\n";
  }

  //@}
};

END_NAMESPACE_NUMERICAL

#endif
