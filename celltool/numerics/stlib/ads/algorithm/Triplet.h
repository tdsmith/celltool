// -*- C++ -*-

/*! 
  \file Triplet.h
  \brief The Triplet data structure.
*/

#if !defined(__ads_Triplet_h__)
#define __ads_Triplet_h__

#include "../defs.h"

BEGIN_NAMESPACE_ADS

//! Triplet holds three objects of arbitrary type.
template<typename T1, typename T2, typename T3>
struct Triplet {
  //! The first bound type
  typedef T1 first_type;
  //!  The second bound type
  typedef T2 second_type;   
  //!  The third bound type
  typedef T3 third_type;   

  //! A copy of the first object
  first_type first;
  //! A copy of the second object
  second_type second;
  //! A copy of the third object
  third_type third;

  //! Default constructor.
  /*! 
    Create the elements using their default constructors.
  */
  Triplet() : 
    first(),
    second(),
    third()
  {}
  
  //! Construct by copying the elements.
  Triplet(const first_type& a, const second_type& b, const third_type& c) : 
    first(a), 
    second(b),
    third(c)
  {}

  //! Construct from a triplet of different types.
  template<typename U1, typename U2, typename U3>
  Triplet(const Triplet<U1,U2,U3>& x) : 
    first(x.first), 
    second(x.second),
    third(x.third)
  {}
};

//! Two Triplets of the same type are equal iff their members are equal.
template<typename T1, typename T2, typename T3>
inline 
bool
operator==(const Triplet<T1,T2,T3>& x, const Triplet<T1,T2,T3>& y) { 
  return x.first == y.first && x.second == y.second && x.third == y.third; 
}

//! Treat the triplet as a composite number.
template<typename T1, typename T2, typename T3>
inline 
bool
operator<(const Triplet<T1,T2,T3>& x, const Triplet<T1,T2,T3>& y) { 
  return x.first < y.first || 
    (! (y.first < x.first) && x.second < y.second) || 
    (! (y.first < x.first) && ! (y.second < x.second) && 
      x.third < y.third);
}

//! Return true if the members are not all equal.
template<typename T1, typename T2, typename T3>
inline 
bool
operator!=(const Triplet<T1,T2,T3>& x, const Triplet<T1,T2,T3>& y) { 
  return !(x == y); 
}

//! Use \c operator< to find the result.
template<typename T1, typename T2, typename T3>
inline 
bool
operator>(const Triplet<T1,T2,T3>& x, const Triplet<T1,T2,T3>& y) { 
  return y < x; 
}

//! Use \c operator< to find the result.
template<typename T1, typename T2, typename T3>
inline 
bool
operator<=(const Triplet<T1,T2,T3>& x, const Triplet<T1,T2,T3>& y) { 
  return !(y < x); 
}

//! Use \c operator< to find the result.
template<typename T1, typename T2, typename T3>
inline 
bool
operator>=(const Triplet<T1,T2,T3>& x, const Triplet<T1,T2,T3>& y) { 
  return !(x < y); 
}

//! A convenience wrapper for creating a Triplet from three objects.
/*!
  \param x The first object.
  \param y The second object.
  \param z The third object.
  \return A newly-constructed Triplet<> object of the appropriate type.
 
*/
template<typename T1, typename T2, typename T3>
inline 
Triplet<T1,T2,T3>
makeTriplet(const T1& x, const T2& y, const T3& z) { 
  return Triplet<T1,T2,T3>(x, y, z); 
}

END_NAMESPACE_ADS

#endif
