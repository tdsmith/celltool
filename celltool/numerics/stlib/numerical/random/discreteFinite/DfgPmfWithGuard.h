// -*- C++ -*-

/*! 
  \file numerical/random/discreteFinite/DfgPmfWithGuard.h
  \brief Probability mass function for a discrete, finite generator.
*/

#if !defined(__numerical_DfgPmfWithGuard_h__)
#define __numerical_DfgPmfWithGuard_h__

#include "linearSearch.h"

#include "../../../ads/array/Array.h"

#include <limits>

// If we are debugging the whole numerical package.
#if defined(DEBUG_numerical) && !defined(DEBUG_numerical_DfgPmfWithGuard)
#define DEBUG_numerical_DfgPmfWithGuard
#endif

BEGIN_NAMESPACE_NUMERICAL

//! Probability mass function for a discrete, finite generator.
/*!
  \param T The number type.  By default it is double.

  Manage the probability mass function.  It has an extra guard element
  to aid in searching.
*/
template<typename T = double>
class DfgPmfWithGuard {
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
  /*! This array has one extra element to make searching more efficient. */
  Container _pmf;

  //--------------------------------------------------------------------------
  //! \name Constructors etc.
  //@{
protected:

  //! Default constructor.
  DfgPmfWithGuard() :
    // The PMF array is empty.
    _pmf()
  {}

  //! Construct from the probability mass function.
  template<typename ForwardIterator>
  DfgPmfWithGuard(ForwardIterator begin, ForwardIterator end) :
    _pmf() {
    initialize(begin, end);
  }

  //! Copy constructor.
  DfgPmfWithGuard(const DfgPmfWithGuard& other) :
    _pmf(other._pmf)
  {}

  //! Assignment operator.
  DfgPmfWithGuard&
  operator=(const DfgPmfWithGuard& other) {
    if (this != &other) {
      _pmf = other._pmf;
    }
    return *this;
  }

  //! Destructor.
  ~DfgPmfWithGuard()
  {}

  //@}
  //--------------------------------------------------------------------------
  //! \name Random number generation.
  //@{
protected:

  // CONTINUE: Write two versions.
  //! Return a discrete, finite deviate.
  /*!
    Use a linear search to sum probabilities until the sum reaches r.
    Optionally, you can specify an offset if you have allready subtracted
    a number of the probabilities.
  */
  int
  operator()(Number r, const int offset = 0) const {
    return offset +
      linearSearchChopDownGuarded(getPmfBeginning() + offset, getPmfEnd(), r);
  }

  //@}
  //--------------------------------------------------------------------------
  //! \name Accessors.
  //@{
public:

  //! Get the probability mass function with the specified index.
  Number
  getPmf(const int index) const {
#ifdef DEBUG_numerical_DfgPmfWithGuard
    assert(0 <= index && index < _pmf.size() - 1);
#endif
    return _pmf[index];
  }

  //! Get the number of possible deviates.
  int
  getSize() const {
    // The PMF array has one extra element.
    return _pmf.size() - 1;
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
    return _pmf.end() - 1;
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
  operator==(const DfgPmfWithGuard& other) const {
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
    _pmf.resize(std::distance(begin, end) + 1);
    std::copy(begin, end, _pmf.begin());
    // The guard element for searching.
    *(_pmf.end() - 1) = 0.5 * std::numeric_limits<Number>::max();
  }

  //! Set the probability mass function with the specified index.
  void
  setPmf(int index, Number value) {
#ifdef DEBUG_numerical_DfgPmfWithGuard
    // I need this check because _pmf has a guard element.
    assert(0 <= index && index < _pmf.size() - 1);
#endif
    _pmf[index] = value;
  }

  //! Set the probability mass functions.
  template<typename _RandomAccessIterator>
  void
  setPmf(_RandomAccessIterator iterator) {
    for (int i = 0; i != _pmf.size() = 1; ++i) {
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
    // Subtract one to account for the guard element.
    return _pmf.end() - 1;
  }

  //@}
  //--------------------------------------------------------------------------
  //! \name File I/O.
  //@{
protected:

  //! Print information about the data structure.
  void
  print(std::ostream& out) const {
    out << "PMF with guard = \n" << _pmf << "\n";
  }

  //@}
};


END_NAMESPACE_NUMERICAL

#endif
