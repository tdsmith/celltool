// -*- C++ -*-

/*! 
  \file numerical/random/discreteFinite/DfgBinConstants.h
  \brief Policy classes for handling bin constants.
*/

#if !defined(__numerical_DfgBinConstants_h__)
#define __numerical_DfgBinConstants_h__

#include "../../constants/Exponentiation.h"

#include <limits>

#include <cassert>
#include <cmath>

// If we are debugging the whole numerical package.
#if defined(DEBUG_numerical) && !defined(DEBUG_numerical_DfgBinConstants)
#define DEBUG_DfgBinConstants
#endif

BEGIN_NAMESPACE_NUMERICAL

//! Bin constants are determined at compile time.
/*!
  \param IndexBits This determines the number of bins (2^IndexBits).  
  \param T The number type.
*/
template<int IndexBits, typename T = double>
class DfgBinConstantsStatic {
  //
  // Public types.
  //
public:

  //! The number type.
  typedef T Number;

  //-------------------------------------------------------------------------
  /*! \name Constructors, etc.
    The big four are protected so that only derived classes can use them.
  */
  //@{
protected:

  //! Default constructor.
  DfgBinConstantsStatic()
  {}

  //! Copy constructor.
  DfgBinConstantsStatic(const DfgBinConstantsStatic& other)
  {}

  //! Assignment operator.
  DfgBinConstantsStatic&
  operator=(const DfgBinConstantsStatic& other) {
    return *this;
  }

  //! Destructor.
  ~DfgBinConstantsStatic()
  {}
  //@}
  //-------------------------------------------------------------------------
  //! \name Accessors.
  //@{
public:

  //! The number of bits used for indexing the bins.
  static
  int
  getIndexBits() {
    return IndexBits;
  }

  //! The number of bins. 2^IndexBits.
  static
  int 
  getNumberOfBins() {
    return Exponentiation<2, IndexBits>::Result;
  }

  //! The mask for extracting the index.  
  /*!
    An unsigned representation of _numberOfBins - 1.
  */
  static
  unsigned 
  getIndexMask() {
    return getNumberOfBins() - 1;
  }

  //! The inverse of the maximum height.  1 / (2^(32-_indexBits) - 1).
  static
  Number
  getMaxHeightInverse() {
    return 1.0 / (std::numeric_limits<unsigned>::max() / getNumberOfBins() - 1);
  }

  //@}
};


//! Bin constants can be set at run time.
template<typename T = double>
class DfgBinConstantsDynamic {
  //
  // Public types.
  //
public:

  //! The number type.
  typedef T Number;

  //
  // Member data.
  //
private:

  int _indexBits;
  int _numberOfBins;
  unsigned _indexMask;
  Number _maxHeightInverse;

  //-------------------------------------------------------------------------
  /*! \name Constructors, etc.
    The big four are protected so that only derived classes can use them.
  */
  //@{
protected:

  //! Default constructor.
  /*!
    The default number of index bits is 8.
  */
  DfgBinConstantsDynamic() {
    setIndexBits(8);
  }

  //! Copy constructor.
  DfgBinConstantsDynamic(const DfgBinConstantsDynamic& other) :
    _indexBits(other._indexBits),
    _numberOfBins(other._numberOfBins),
    _indexMask(other._indexMask),
    _maxHeightInverse(other._maxHeightInverse)
  {}

  //! Assignment operator.
  DfgBinConstantsDynamic&
  operator=(const DfgBinConstantsDynamic& other) {
    if (&other != this) {
      _indexBits = other._indexBits;
      _numberOfBins = other._numberOfBins;
      _indexMask = other._indexMask;
      _maxHeightInverse = other._maxHeightInverse;
    }
    return *this;
  }

  //! Destructor.
  ~DfgBinConstantsDynamic()
  {}

  //@}
  //-------------------------------------------------------------------------
  //! \name Accessors.
  //@{
public:

  //! The number of bits used for indexing the bins.
  int
  getIndexBits() const {
    return _indexBits;
  }

  //! The number of bins. 2^IndexBits.
  int 
  getNumberOfBins() const {
    return _numberOfBins;
  }

  //! The mask for extracting the index.  
  /*!
    An unsigned representation of NumberOfBins - 1.
  */
  unsigned 
  getIndexMask() const {
    return _indexMask;
  }

  //! The inverse of the maximum height.  1 / (2^(32 - IndexBits) - 1).
  Number
  getMaxHeightInverse() const {
    return _maxHeightInverse;
  }

  //@}
  //-------------------------------------------------------------------------
  //! \name Manipulators.
  //@{
public:

  //! Set the number of index bits.
  void
  setIndexBits(const int indexBits) {
    assert(0 <= indexBits && indexBits < 32);
    _indexBits = indexBits;
    _numberOfBins = int(std::pow(Number(2), _indexBits));
    _indexMask = _numberOfBins - 1;
    _maxHeightInverse = 1.0 / (std::numeric_limits<unsigned>::max() / 
			       _numberOfBins - 1);
  }

  //@}
};


END_NAMESPACE_NUMERICAL

#endif
