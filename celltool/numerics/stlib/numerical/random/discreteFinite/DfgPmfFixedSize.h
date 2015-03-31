// -*- C++ -*-

/*! 
  \file numerical/random/discreteFinite/DfgPmfFixedSize.h
  \brief Probability mass function for a discrete, finite generator.
*/

#if !defined(__numerical_DfgPmfFixedSize_h__)
#define __numerical_DfgPmfFixedSize_h__

#include "linearSearch.h"

#include "../../../ads/array/Array.h"

#include <limits>

// If we are debugging the whole numerical package.
#if defined(DEBUG_numerical) && !defined(DEBUG_numerical_DfgPmfFixedSize)
#define DEBUG_numerical_DfgPmfFixedSize
#endif

BEGIN_NAMESPACE_NUMERICAL

//! Whether to use branching and meta-programming.
template<bool Branching, bool MetaProgramming>
class TraitsForBranchingAndMetaProgramming {
public:
  //! Whether to use branching.
  static const bool UseBranching = Branching;
  //! Whether to use meta-programming.
  static const bool UseMetaProgramming = MetaProgramming;
};


//! Probability mass function for a discrete, finite generator.
/*!
  \param N The number of elements.
  \param T The number type.  By default it is double.

  Manage the probability mass function.
*/
template<int N, 
	 class Traits = TraitsForBranchingAndMetaProgramming<false, false>,
	 typename T = double>
class DfgPmfFixedSize {
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
  /*!
    It is more efficient to use a dynamically sized array than one of 
    fixed size.
  */
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
  DfgPmfFixedSize() :
    // Allocate memory for the array.
    _pmf(N)
  {}

  //! Construct from the probability mass function.
  template<typename ForwardIterator>
  DfgPmfFixedSize(ForwardIterator begin, ForwardIterator end) :
    // Allocate memory for the array.
    _pmf(N) {
    initialize(begin, end);
  }

  //! Copy constructor.
  DfgPmfFixedSize(const DfgPmfFixedSize& other) :
    _pmf(other._pmf)
  {}

  //! Assignment operator.
  DfgPmfFixedSize&
  operator=(const DfgPmfFixedSize& other) {
    if (this != &other) {
      _pmf = other._pmf;
    }
    return *this;
  }

  //! Destructor.
  ~DfgPmfFixedSize()
  {}

  //@}
  //--------------------------------------------------------------------------
  //! \name Random number generation.
  //@{
protected:

  //! Return a discrete, finite deviate.
  /*!
    Use a linear search to sum probabilities until the sum reaches r.
  */
  int
  operator()(Number r) const {
    return computeDeviate(r, Loki::Int2Type<Traits::UseBranching>(), 
			  Loki::Int2Type<Traits::UseMetaProgramming>());
  }

private:

  int
  computeDeviate(Number r, Loki::Int2Type<false> /*UseBranching*/,
		 Loki::Int2Type<false> /*UseMetaProgramming*/) const {
    int result = 0;
    for (int i = 0 ; i != N - 1; ++i) {
      r -= _pmf[i];
      result += (r >= 0);
    }
    return result;
  }

  int
  computeDeviate(Number r, Loki::Int2Type<true> /*UseBranching*/,
		 Loki::Int2Type<false> /*UseMetaProgramming*/) const {
    int i = 0;
    for ( ; i != N - 1; ++i) {
      if (r < _pmf[i]) {
	break;
      }
      r -= _pmf[i];
    }
    return i;

#if 0
    int i = 0;
    for ( ; i != N - 1; ++i) {
      r -= _pmf[i];
      if (r < 0) {
	break;
      }
    }
    return i;
#endif
  }

  int
  computeDeviate(Number r, Loki::Int2Type<false> /*UseBranching*/,
		 Loki::Int2Type<true> /*UseMetaProgramming*/) const {
    return LinearSearchChopDown<N, ConstIterator>::result(_pmf.begin(), r);
  }


#if 0
    return (r >= _pmf[0]) + (r >= _pmf[0] + _pmf[1]) + 
      (r >= _pmf[0] + _pmf[1] + _pmf[2]);
#endif

#if 0
    r -= _pmf[0];
    int i = (r >= 0);
    r -= _pmf[1];
    i += (r >= 0);
    r -= _pmf[2];
    i += (r >= 0);
    return i;
#endif

#if 0
    int i = (r >= _pmf[0]);
    r -= _pmf[0];
    i += (r >= _pmf[1]);
    r -= _pmf[1];
    i += (r >= _pmf[2]);
    return i;
#endif

#if 0
    if (r < _pmf[0]) {
      return 0;
    }
    r -= _pmf[0];

    if (r < _pmf[1]) {
      return 1;
    }
    r -= _pmf[1];

    if (r < _pmf[2]) {
      return 2;
    }

    return 3;
#endif

#if 0
    r -= _pmf[0];
    if (r < 0) {
      return 0;
    }

    r -= _pmf[1];
    if (r < 0) {
      return 1;
    }

    r -= _pmf[2];
    if (r < 0) {
      return 2;
    }

    return 3;
#endif



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
    return N;
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
  operator==(const DfgPmfFixedSize& other) const {
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
    assert(std::distance(begin, end) == getSize());
    std::copy(begin, end, _pmf.begin());
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
