// -*- C++ -*-

/*! 
  \file ObjectAndBlankSpace.h
  \brief Useful for avoiding false sharing.
*/

#if !defined(__ads_utility_ObjectAndBlankSpace_h__)
#define __ads_utility_ObjectAndBlankSpace_h__

#include "../defs.h"

BEGIN_NAMESPACE_ADS

//! Useful for avoiding false sharing.
template<typename _Object, int _BlankSpaceSize = 64>
class ObjectAndBlankSpace : public _Object {
  //
  // Public types.
  //

public:

  //! The object type.
  typedef _Object Base;

  //
  // Member data.
  //

private:

  // The blank space
  char _blankSpace[_BlankSpaceSize];

public:

  //--------------------------------------------------------------------------
  //! \name Constructors etc.
  //@{

  //! Default constructor.
  ObjectAndBlankSpace() :
    Base()
  {}
  
  //! Copy constructor.
  ObjectAndBlankSpace(const ObjectAndBlankSpace& other) :
    Base(other)
  {}
  
  //! Assignment operator.
  ObjectAndBlankSpace&
  operator=(const ObjectAndBlankSpace& other) {
    if (this != &other) {
      Base::operator=(other);
    }
    return *this;
  }
  
  //! Construct from an object
  ObjectAndBlankSpace(const Base& object) :
    Base(object)
  {}
  
  //! Trivial destructor.
  ~ObjectAndBlankSpace()
  {}

  //@}
};





#if 0
//! Useful for avoiding false sharing.
template<typename _Object, int _BlankSpaceSize = 64>
class ObjectAndBlankSpace {
  //
  // Public types.
  //

public:

  //! The object type.
  typedef _Object Object;

  //
  // Member data.
  //

private:

  // The object
  Object _object;
  // The blank space
  char _blankSpace[_BlankSpaceSize];

public:

  //--------------------------------------------------------------------------
  //! \name Constructors etc.
  //@{

  //! Default constructor.
  ObjectAndBlankSpace() :
    _object()
  {}
  
  //! Copy constructor.
  ObjectAndBlankSpace(const ObjectAndBlankSpace& other) :
    _object(other._object)
  {}
  
  //! Assignment operator.
  ObjectAndBlankSpace&
  operator=(const ObjectAndBlankSpace& other) {
    if (this != &other) {
      _object = other._object;
    }
    return *this;
  }
  
  //! Construct from an object
  ObjectAndBlankSpace(const Object& object) :
    _object(object)
  {}
  
  //! Trivial destructor.
  ~ObjectAndBlankSpace()
  {}

  //@}
  //--------------------------------------------------------------------------
  //! \name Conversion.
  //@{
  
  operator const Object&() const {
    return _object;
  }

  operator Object&() {
    return _object;
  }

  //@}
};
#endif

END_NAMESPACE_ADS

#endif
