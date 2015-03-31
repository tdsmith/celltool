// -*- C++ -*-

/*! 
  \file compare_handle.h
  \brief Contains comparison functors for handles to objects.
*/

#if !defined(__ads_compare_handle_h__)
#define __ads_compare_handle_h__

#include "../defs.h"

#include <functional>

BEGIN_NAMESPACE_ADS

//-----------------------------------------------------------------------------
/*! \defgroup functor_compare_handle Functor: Compare Handles */
// @{

//! Functor for equality comparison of objects by their handles.
template<typename Handle>
struct EqualToByHandle : 
  public std::binary_function<Handle,Handle,bool> {
private:
  typedef std::binary_function<Handle,Handle,bool> Base;
public:
  //! The result type.
  typedef typename Base::result_type result_type;
  //! The first argument type.
  typedef typename Base::first_argument_type first_argument_type;
  //! The second argument type.
  typedef typename Base::second_argument_type second_argument_type;

  //! Return true if the objects are equal.
  result_type
  operator()(const first_argument_type x, const second_argument_type y) const { 
    return *x == *y; 
  }
};

//! Return an \c EqualToByHandle struct.
/*!
  \relates EqualToByHandle

  The template argument is required.
*/
template<typename Handle>
inline
EqualToByHandle<Handle>
constructEqualToByHandle() {
  return EqualToByHandle<Handle>();
}


//! Functor for inequality comparison of objects by their handles.
template<typename Handle>
struct NotEqualToByHandle : 
  public std::binary_function<Handle,Handle,bool> {
private:
  typedef std::binary_function<Handle,Handle,bool> Base;
public:
  //! The result type.
  typedef typename Base::result_type result_type;
  //! The first argument type.
  typedef typename Base::first_argument_type first_argument_type;
  //! The second argument type.
  typedef typename Base::second_argument_type second_argument_type;

  //! Return true if the objects are not equal.
  result_type
  operator()(const first_argument_type x, const second_argument_type y) const { 
    return *x != *y;
  }
};

//! Return an \c NotEqualToByHandle struct.
/*!
  \relates NotEqualToByHandle

  The template argument is required.
*/
template<typename Handle>
inline
NotEqualToByHandle<Handle>
constructNotEqualToByHandle() {
  return NotEqualToByHandle<Handle>();
}


//! Functor for greater than comparison of objects by their handles.
template<typename Handle>
struct GreaterByHandle : 
  public std::binary_function<Handle,Handle,bool> {
private:
  typedef std::binary_function<Handle,Handle,bool> Base;
public:
  //! The result type.
  typedef typename Base::result_type result_type;
  //! The first argument type.
  typedef typename Base::first_argument_type first_argument_type;
  //! The second argument type.
  typedef typename Base::second_argument_type second_argument_type;

  //! Return true if the first object is greater than the second.
  result_type
  operator()(const first_argument_type x, const second_argument_type y) const { 
    return *x > *y;
  }
};

//! Return an \c GreaterByHandle struct.
/*!
  \relates GreaterByHandle

  The template argument is required.
*/
template<typename Handle>
inline
GreaterByHandle<Handle>
constructGreaterByHandle() {
  return GreaterByHandle<Handle>();
}


//! Functor for less than comparison of objects by their handles.
template<typename Handle>
struct LessByHandle : 
  public std::binary_function<Handle,Handle,bool> {
private:
  typedef std::binary_function<Handle,Handle,bool> Base;
public:
  //! The result type.
  typedef typename Base::result_type result_type;
  //! The first argument type.
  typedef typename Base::first_argument_type first_argument_type;
  //! The second argument type.
  typedef typename Base::second_argument_type second_argument_type;

  //! Return true if the first object is less than the second.
  result_type
  operator()(const first_argument_type x, const second_argument_type y) const { 
    return *x < *y; 
  }
};

//! Return an \c LessByHandle struct.
/*!
  \relates LessByHandle

  The template argument is required.
*/
template<typename Handle>
inline
LessByHandle<Handle>
constructLessByHandle() {
  return LessByHandle<Handle>();
}


//! Functor for greater than or equal to comparison of objects by their handles.
template<typename Handle>
struct GreaterEqualByHandle : 
  public std::binary_function<Handle,Handle,bool> {
private:
  typedef std::binary_function<Handle,Handle,bool> Base;
public:
  //! The result type.
  typedef typename Base::result_type result_type;
  //! The first argument type.
  typedef typename Base::first_argument_type first_argument_type;
  //! The second argument type.
  typedef typename Base::second_argument_type second_argument_type;

  //! Return true if the first object is greater than or equal to the second.
  result_type
  operator()(const first_argument_type x, const second_argument_type y) const { 
    return *x >= *y; 
  }
};

//! Return an \c GreaterEqualByHandle struct.
/*!
  \relates GreaterEqualByHandle

  The template argument is required.
*/
template<typename Handle>
inline
GreaterEqualByHandle<Handle>
constructGreaterEqualByHandle() {
  return GreaterEqualByHandle<Handle>();
}


//! Functor for less than or equal to comparison of objects by their handles.
template<typename Handle>
struct LessEqualByHandle : 
  public std::binary_function<Handle,Handle,bool> {
private:
  typedef std::binary_function<Handle,Handle,bool> Base;
public:
  //! The result type.
  typedef typename Base::result_type result_type;
  //! The first argument type.
  typedef typename Base::first_argument_type first_argument_type;
  //! The second argument type.
  typedef typename Base::second_argument_type second_argument_type;

  //! Return true if the first object is less than or equal to the second.
  result_type
  operator()(const first_argument_type x, const second_argument_type y) const { 
    return *x <= *y; 
  }
};

//! Return an \c LessEqualByHandle struct.
/*!
  \relates LessEqualByHandle

  The template argument is required.
*/
template<typename Handle>
inline
LessEqualByHandle<Handle>
constructLessEqualByHandle() {
  return LessEqualByHandle<Handle>();
}


//
// Handle-Handle
//


//! Functor for equality comparison of objects by handles to their handles.
template<typename Handle>
struct EqualToByHandleHandle : 
  public std::binary_function<Handle,Handle,bool> {
private:
  typedef std::binary_function<Handle,Handle,bool> Base;
public:
  //! The result type.
  typedef typename Base::result_type result_type;
  //! The first argument type.
  typedef typename Base::first_argument_type first_argument_type;
  //! The second argument type.
  typedef typename Base::second_argument_type second_argument_type;

  //! Return true if the objects are equal.
  result_type
  operator()(const first_argument_type x, const second_argument_type y) const { 
    return **x == **y; 
  }
};

//! Return an \c EqualToByHandleHandle struct.
/*!
  \relates EqualToByHandleHandle

  The template argument is required.
*/
template<typename Handle>
inline
EqualToByHandleHandle<Handle>
constructEqualToByHandleHandle() {
  return EqualToByHandleHandle<Handle>();
}


//! Functor for inequality comparison of objects by handles to their handles.
template<typename Handle>
struct NotEqualToByHandleHandle : 
  public std::binary_function<Handle,Handle,bool> {
private:
  typedef std::binary_function<Handle,Handle,bool> Base;
public:
  //! The result type.
  typedef typename Base::result_type result_type;
  //! The first argument type.
  typedef typename Base::first_argument_type first_argument_type;
  //! The second argument type.
  typedef typename Base::second_argument_type second_argument_type;

  //! Return true if the objects are not equal.
  result_type
  operator()(const first_argument_type x, const second_argument_type y) const { 
    return **x != **y;
  }
};

//! Return an \c NotEqualToByHandleHandle struct.
/*!
  \relates NotEqualToByHandleHandle

  The template argument is required.
*/
template<typename Handle>
inline
NotEqualToByHandleHandle<Handle>
constructNotEqualToByHandleHandle() {
  return NotEqualToByHandleHandle<Handle>();
}


//! Functor for greater than comparison of objects by handles to their handles.
template<typename Handle>
struct GreaterByHandleHandle : 
  public std::binary_function<Handle,Handle,bool> {
private:
  typedef std::binary_function<Handle,Handle,bool> Base;
public:
  //! The result type.
  typedef typename Base::result_type result_type;
  //! The first argument type.
  typedef typename Base::first_argument_type first_argument_type;
  //! The second argument type.
  typedef typename Base::second_argument_type second_argument_type;

  //! Return true if the first object is greater than the second.
  result_type
  operator()(const first_argument_type x, const second_argument_type y) const { 
    return **x > **y;
  }
};

//! Return an \c GreaterByHandleHandle struct.
/*!
  \relates GreaterByHandleHandle

  The template argument is required.
*/
template<typename Handle>
inline
GreaterByHandleHandle<Handle>
constructGreaterByHandleHandle() {
  return GreaterByHandleHandle<Handle>();
}


//! Functor for less than comparison of objects by handles to their handles.
template<typename Handle>
struct LessByHandleHandle : 
  public std::binary_function<Handle,Handle,bool> {
private:
  typedef std::binary_function<Handle,Handle,bool> Base;
public:
  //! The result type.
  typedef typename Base::result_type result_type;
  //! The first argument type.
  typedef typename Base::first_argument_type first_argument_type;
  //! The second argument type.
  typedef typename Base::second_argument_type second_argument_type;

  //! Return true if the first object is less than the second.
  result_type
  operator()(const first_argument_type x, const second_argument_type y) const { 
    return **x < **y; 
  }
};

//! Return an \c LessByHandleHandle struct.
/*!
  \relates LessByHandleHandle

  The template argument is required.
*/
template<typename Handle>
inline
LessByHandleHandle<Handle>
constructLessByHandleHandle() {
  return LessByHandleHandle<Handle>();
}


//! Functor for greater than or equal to comparison of objects by handles to their handles.
template<typename Handle>
struct GreaterEqualByHandleHandle : 
  public std::binary_function<Handle,Handle,bool> {
private:
  typedef std::binary_function<Handle,Handle,bool> Base;
public:
  //! The result type.
  typedef typename Base::result_type result_type;
  //! The first argument type.
  typedef typename Base::first_argument_type first_argument_type;
  //! The second argument type.
  typedef typename Base::second_argument_type second_argument_type;

  //! Return true if the first object is greater than or equal to the second.
  result_type
  operator()(const first_argument_type x, const second_argument_type y) const { 
    return **x >= **y; 
  }
};

//! Return an \c GreaterEqualByHandleHandle struct.
/*!
  \relates GreaterEqualByHandleHandle

  The template argument is required.
*/
template<typename Handle>
inline
GreaterEqualByHandleHandle<Handle>
constructGreaterEqualByHandleHandle() {
  return GreaterEqualByHandleHandle<Handle>();
}


//! Functor for less than or equal to comparison of objects by handles to their handles.
template<typename Handle>
struct LessEqualByHandleHandle : 
  public std::binary_function<Handle,Handle,bool> {
private:
  typedef std::binary_function<Handle,Handle,bool> Base;
public:
  //! The result type.
  typedef typename Base::result_type result_type;
  //! The first argument type.
  typedef typename Base::first_argument_type first_argument_type;
  //! The second argument type.
  typedef typename Base::second_argument_type second_argument_type;

  //! Return true if the first object is less than or equal to the second.
  result_type
  operator()(const first_argument_type x, const second_argument_type y) const { 
    return **x <= **y; 
  }
};

//! Return an \c LessEqualByHandleHandle struct.
/*!
  \relates LessEqualByHandleHandle

  The template argument is required.
*/
template<typename Handle>
inline
LessEqualByHandleHandle<Handle>
constructLessEqualByHandleHandle() {
  return LessEqualByHandleHandle<Handle>();
}

// @}

END_NAMESPACE_ADS

#endif
