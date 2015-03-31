// -*- C++ -*-

/*!
  \file ads/indexedPriorityQueue/HashingChaining.h
  \brief Hashing data structure with chaining.
*/

#if !defined(__ads_indexedPriorityQueue_HashingChaining_h__)
#define __ads_indexedPriorityQueue_HashingChaining_h__

#include "../defs.h"

#include "../array/Array.h"

#include <vector>

BEGIN_NAMESPACE_ADS

//! The chaining container.
template<typename _Value>
class ChainingContainer {
  //
  // Public types.
  //
public:

  //! The value type.
  typedef _Value value_type;
  //! Iterator on the value type.
  typedef value_type* iterator;
  //! Const iterator on the value type.
  typedef const value_type* const_iterator;

  //
  // Member data.
  //
private:

  iterator _begin, _end, _endOfStorage;

  //
  // Not implemented.
  //
private:


  //--------------------------------------------------------------------------
  //! \name Constructors etc.
  //@{
public:

  //! Default constructor.
  ChainingContainer() {
    _begin = new value_type[1];
    _end = _begin;
    _endOfStorage = _begin + 1;
  }

  //! Destructor.
  ~ChainingContainer() {
    delete[] _begin;
    _begin = _end = _endOfStorage = 0;
  }

  //@}
  //--------------------------------------------------------------------------
  //! \name Accessors.
  //@{
public:

  const_iterator
  begin() const {
    return _begin;
  }

  const_iterator
  end() const {
    return _end;
  }

  bool
  empty() const {
    return _begin == _end;
  }
  
  //@}
  //--------------------------------------------------------------------------
  //! \name Manipulators.
  //@{
public:

  iterator
  search(const value_type value) {
#ifdef DEBUG_ads
    assert(! empty());
#endif
    iterator i = _begin;
    while(*i != value) {
      ++i;
    }
    return i;
  }

  void
  push(const value_type value) {
    if (_end == _endOfStorage) {
      increaseStorage();
    }
    *_end = value;
    ++_end;
  }

  value_type
  pop() {
#ifdef DEBUG_ads
    assert(! empty());
#endif
    iterator minimum = _begin;
    for (iterator i = _begin + 1; i != _end; ++i) {
      if (**i < **minimum) {
	minimum = i;
      }
    }
    erase(minimum);
    return *_end;
  }

  void
  erase(const value_type value) {
    erase(search(value));
  }

  void
  clear() {
    _end = _begin;
  }

private:

  void
  erase(const iterator i) {
#ifdef DEBUG_ads
    assert(_begin <= i && i < _end);
#endif
    --_end;
    std::swap(*i, *_end);
  }

  void
  increaseStorage() {
    // Got new storage.
    const int oldSize = _endOfStorage - _begin;
    iterator tmp = new value_type[2 * oldSize];
    // Copy the old values.
    std::copy(_begin, _end, tmp);
    // Delete the old storage.
    delete[] _begin;
    // Point to the new storage.
    _begin = tmp;
    _end = _begin + oldSize;
    _endOfStorage = _begin + 2 * oldSize;
  }

  //@}
};



//! Hashing data structure with chaining.
/*!
  \param _Iterator is the value type.
  \param _Number is the number type.
*/
template<typename _Iterator,
	 typename _Number = double>
class HashingChaining {
  //
  // Public types.
  //
public:

  //! The value type.
  typedef _Iterator Iterator;
  //! The number type.
  typedef _Number Number;

  //
  // Private types.
  //
private:

  //! The chaining container.
  typedef ChainingContainer<Iterator> ChainingContainer;
  typedef typename ChainingContainer::iterator ChainingIterator;
  typedef typename ChainingContainer::const_iterator ChainingConstIterator;

  //
  // Nested classes.
  //
private:

  //! Compare two iterators.
  struct Compare {
    bool
    operator()(const Iterator x, const Iterator y) const {
      return *x < *y;
    }
  };

  //
  // Member data.
  //
private:

  //! The hash table.
  ads::Array<1, ChainingContainer> _hashTable;
  Compare _compare;
  //! The number of elements in the hash table.
  int _size;
  int _currentIndex;
  Number _targetLoad;
  Number _lowerBound;
  Number _upperBound;
  Number _inverseLength;

  //--------------------------------------------------------------------------
  //! \name Constructors etc.
  //@{
public:

  //! Construct from the table size.
  HashingChaining(const int tableSize, const Number targetLoad) :
    _hashTable(tableSize),
    _compare(),
    _size(0),
    _currentIndex(0),
    _targetLoad(targetLoad),
    // Start with invalid values for the following.
    _lowerBound(std::numeric_limits<Number>::max()),
    _upperBound(- std::numeric_limits<Number>::max()),
    _inverseLength(0) {
  }

  //! Build the hash table.
  void
  rebuild(Iterator begin, const Iterator end, const Number length) {
#ifdef DEBUG_ads
    assert(length != 0 && _hashTable.size() > 1);
    assert(begin != end);
    assert(_size == 0);
#endif

    // If we don't have a valid value for the old upper bound.
    if (_upperBound == - std::numeric_limits<Number>::max()) {
      // Compute the correct value.
      _upperBound = *std::min_element(begin, end);
    }

    // Reset the current index to the first row.
    _currentIndex = 0;
    // Determine the geometry.
    _lowerBound = _upperBound;
    // Allow for round-off errors by not using the last row in the table.
    _upperBound = _lowerBound + length * _targetLoad *
      (_hashTable.size() - 1);
    _inverseLength = (_hashTable.size() - 1) / (_upperBound - _lowerBound);

    // Insert the elements.
    while (begin != end) {
      if (isIn(*begin)) {
	insert(begin);
      }
      ++begin;
    }
  }
  
  //@}
  //--------------------------------------------------------------------------
  //! \name Accessors.
  //@{
public:

  //! Return true if the hash table is empty.
  bool
  isEmpty() const {
    return _size == 0;
  }

private:

  //! Return if the element is in the hash table.
  bool
  isIn(const Number value) const {
    return value < _upperBound;
  }

  //! The hashing function.
  int
  hashFunction(const Number x) const {
#ifdef DEBUG_ads
    assert(_lowerBound < _upperBound);
#endif
    return int((x - _lowerBound) * _inverseLength);
  }

  //@}
  //--------------------------------------------------------------------------
  //! \name Manipulators.
  //@{
public:

  //! Find and remove the minimum element.
  Iterator
  pop() {
    // Find a non-empty row.
    while (_hashTable[_currentIndex].empty()) {
      ++_currentIndex;
#ifdef DEBUG_ads
      assert(_currentIndex != _hashTable.size());
#endif
    }

    // Find the minimum element in the row and remove it from the hash table.
    --_size;
    return _hashTable[_currentIndex].pop();
  }

  //! Push the element into the hash table.
  void
  push(const Iterator element) {
    // If the element should be in the hash table.
    if (isIn(*element)) {
      insert(element);
    }
  }

  //! Change the value of an element.
  void
  set(const Iterator element, const Number oldValue) {
    // If old value is in the hash table.
    if (isIn(oldValue)) {
      const int oldIndex = hashFunction(oldValue);
      // If the new value is in the hash table.
      if (isIn(*element)) {
	const int newIndex = hashFunction(*element);
	// If the element moved.
	if (oldIndex != newIndex) {
	  erase(oldIndex, element);
	  insert(newIndex, element);
	}
      }
      // If the new value is not in the hash table.
      else {
	erase(oldIndex, element);
      }
    }
    // If old value is not in the hash table.
    else {
      // If the new value is in the hash table.
      if (isIn(*element)) {
	insert(element);
      }
      // If the new value is not in the hash table, do nothing.
    }
  }

  //! Insert the element at the appropriate position.
  void
  insert(const Iterator element) {
    insert(hashFunction(*element), element);
  }
  
  //! Insert the element at the given position.
  void
  insert(const int index, const Iterator element) {
    ++_size;
    _hashTable[index].push(element);
  }
  
  //! Erase the element if it is in the hash table.
  void
  erase(const Iterator element) {
    // If the value is in the hash table.
    if (isIn(*element)) {
      erase(hashFunction(*element), element);
    }
  }

  //! Erase the element at the given position.
  void
  erase(const int index, const Iterator element) {
    _hashTable[index].erase(element);
    --_size;
  }

  //! Clear the hash table.
  void
  clear() {
    if (_size != 0) {
      for (int i = 0; i != _hashTable.size(); ++i) {
	_hashTable[i].clear();
      }
      _size = 0;
    }
    _currentIndex = 0;

    // Give these invalid values.
    _lowerBound = std::numeric_limits<Number>::max();
    _upperBound = - std::numeric_limits<Number>::max();
    _inverseLength = 0;
  }

  //@}
  //--------------------------------------------------------------------------
  //! \name File I/O.
  //@{
public:

  //! Print the elements in the hash table.
  void
  print(std::ostream& out) const {
    out << _hashTable.size();
    for (int i = 0; i != _hashTable.size(); ++i) {
      for (ChainingConstIterator j = _hashTable[i].begin(); 
	   j != _hashTable[i].end(); ++j) {
	out << **j << " ";
      }
      out << "\n";
    }
  }

  //@}
};

END_NAMESPACE_ADS

#endif
