// -*- C++ -*-

/*! 
  \file select.h
  \brief Selecting elements from a pair.
*/

#if !defined(__ads_select_h__)
#define __ads_select_h__

#include "../defs.h"

#include <functional>

BEGIN_NAMESPACE_ADS

//-----------------------------------------------------------------------------
/*! \defgroup functor_select Functor: Select 

  The SGI extensions provide functors for selecting the first and second
  elements of a \c std::pair.  This functionality is implemented here to 
  avoid dependence on the extensions.

  The SelectElement functor selects the n_th element of a sequence.
*/
// @{

//! Functor for selecting the first element of a pair.
template<class Pair>
struct Select1st : 
  public std::unary_function<Pair, typename Pair::first_type> {
  //! Return a reference to the first element.
  typename Pair::first_type& 
  operator()(Pair& x) const {
    return x.first;
  }

  //! Return a const reference to the first element.
  const typename Pair::first_type& 
  operator()(const Pair& x) const {
    return x.first;
  }
};

//! Functor for selecting the second element of a pair.
template<class Pair>
struct Select2nd : 
  public std::unary_function<Pair, typename Pair::second_type> {
  //! Return a reference to the second element.
  typename Pair::second_type& 
  operator()(Pair& x) const {
    return x.second;
  }

  //! Return a const reference to the second element.
  const typename Pair::second_type& 
  operator()(const Pair& x) const {
    return x.second;
  }
};

//! Convenience function for constructing a \c Select1st.
template<class Pair>
inline
Select1st<Pair>
select_1st() {
  return Select1st<Pair>();
}

//! Convenience function for constructing a \c Select2nd.
template<class Pair>
inline
Select2nd<Pair>
select_2nd() {
  return Select2nd<Pair>();
}



//! Functor for selecting an element of a sequence.
template<class Sequence, int N>
struct SelectElement : 
  public std::unary_function<Sequence, typename Sequence::value_type> {
  //! The base type.
  typedef std::unary_function<Sequence, typename Sequence::value_type>
  Base;
  //! The argument type.
  typedef typename Base::argument_type argument_type;
  //! The result type.
  typedef typename Base::result_type result_type;

  //! Return a reference to the N_th element.
  result_type& 
  operator()(argument_type& x) const {
    return x[N];
  }

  //! Return a const reference to the N_th element.
  const result_type& 
  operator()(const argument_type& x) const {
    return x[N];
  }
};

//! Convenience function for constructing a \c SelectElement.
template<class Sequence, int N>
inline
SelectElement<Sequence,N>
select_element() {
  return SelectElement<Sequence,N>();
}

// @}

END_NAMESPACE_ADS

#endif
