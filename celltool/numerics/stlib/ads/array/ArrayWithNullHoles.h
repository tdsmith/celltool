// -*- C++ -*-

/*! 
  \file ArrayWithNullHoles.h
  \brief A class for an N-D sparse array.
*/

#if !defined(__ads_array_ArrayWithNullHoles_h__)
#define __ads_array_ArrayWithNullHoles_h__

#include "../defs.h"

#include "../../third-party/loki/TypeTraits.h"

#include <vector>
#include <set>

#include <cassert>

// If we are debugging the whole ads package.
#if defined(DEBUG_ads) && !defined(DEBUG_ArrayWithNullHoles)
#define DEBUG_ArrayWithNullHoles
#endif

BEGIN_NAMESPACE_ADS

//! A 1-D array with holes.
/*!
  \param T is the value type.
*/
template<typename T>
class ArrayWithNullHoles {
  //
  // Private types.
  //

private:

  typedef std::vector<T> ValueContainer;
  typedef std::vector<int> IndexContainer;

  //
  // Public types.
  //

public:

  //! The element type of the array.
  typedef T ValueType;
  //! The parameter type.
  /*! 
    This is used for passing the value type as an argument.
  */
  typedef typename Loki::TypeTraits<ValueType>::ParameterType ParameterType;

  //! A pointer to an element.
  typedef typename ValueContainer::pointer Pointer;
  //! A pointer to a constant element.
  typedef typename ValueContainer::const_pointer ConstPointer;

  // CONTINUE: Iterators need to skip the holes.
#if 0
  //! An iterator in the array.
  typedef typename Something Iterator;
  //! A iterator on constant elements in the array.
  typedef typename Something ConstIterator;
#endif

  //! A reference to an array element.
  typedef typename ValueContainer::reference Reference;
  //! A reference to a constant array element.
  typedef typename ValueContainer::const_reference ConstReference;

  //! The size type is a signed integer.
  /*!
    Having \c std::size_t (which is an unsigned integer) as the size type
    causes minor problems.  Consult "Large Scale C++ Software Design" by 
    John Lakos for a discussion of using unsigned integers in a class 
    interface.
  */
  typedef int SizeType;
  //! Pointer difference type.
  typedef typename ValueContainer::difference_type DifferenceType;

  //
  // Data.
  //

private:

  //! The array elements.
  ValueContainer _data;
  //! The holes.
  IndexContainer _holes;
  //! The null element.
  ValueType _null;

  //
  // Not implemented.
  //

private:

  //! Default constructor not implemented.
  ArrayWithNullHoles();

public:

  //--------------------------------------------------------------------------
  //! \name Constructors etc.
  // @{

  //! Construct from the null value.
  ArrayWithNullHoles(ParameterType null) :
    _data(),
    _holes(),
    _null(null)
  {}

  //! Copy constructor.  Deep copy.
  ArrayWithNullHoles(const ArrayWithNullHoles& other) :
    _data(other._data),
    _holes(other._holes),
    _null(other._null)
  {}

  //! Assignment operator.
  ArrayWithNullHoles& 
  operator=(const ArrayWithNullHoles& other)
  {
    // Avoid assignment to self.
    if (&other != this) {
      _data = other._data;
      _holes = other._holes;
      _null = other._null;
    }
    // Return *this so assignments can chain.
    return *this;
  }

  //! Destructor.
  ~ArrayWithNullHoles()
  {}

  // @}
  //--------------------------------------------------------------------------
  //! \name Accessors.
  // @{

  //! Return the size of the array (non-null elements and holes combined).
  int
  size() const {
    return int(_data.size());
  }

  //! Return the number of null elements (holes).
  int
  sizeNull() const {
    return int(_holes.size());
  }

  //! Return the number of non-null elements.
  int
  sizeNonNull() const {
    return size() - sizeNull();
  }

  //! Return true if the specified element is null.
  bool
  isNull(int index) const;

  //! Return true if the specified element is non-null.
  bool
  isNonNull(int index) const;

  //! Return the specified element.
  ParameterType
  get(int index) const;

  // @}
  //--------------------------------------------------------------------------
  //! \name Manipulators.
  // @{

  //! Insert an element into a hole (or at the end if there are no holes).
  /*!
    \return The index of the element.
  */
  int
  insert(ParameterType value);

  //! Erase the specified element.
  /*!
    \pre The location must not already be a hole.
  */
  void
  erase(int index);

  //! Erase a range of elements.
  /*!
    \pre The location of each must not already be a hole.
  */
  template<typename IntInputIterator>
  void
  erase(IntInputIterator begin, IntInputIterator end);

  //! Set the specified element.
  /*!
    \pre 0 <= index, index < size(), and value is not null.
  */
  void
  set(int index, ParameterType value);

  // @}
  //--------------------------------------------------------------------------
  //! \name Validity.
  // @{

  //! Return true if the data structure is valid.
  bool
  isValid() const;

  // @}
};


END_NAMESPACE_ADS

#define __ads_array_ArrayWithNullHoles_ipp__
#include "ArrayWithNullHoles.ipp"
#undef __ads_array_ArrayWithNullHoles_ipp__

#endif
