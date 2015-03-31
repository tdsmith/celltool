// -*- C++ -*-

/*! 
  \file ads/algorithm/min_max.h
  \brief Contains min and max functions for more than two arguments.
*/

#if !defined(__ads_min_max_h__)
#define __ads_min_max_h__

#include "../defs.h"

// Include this to get std::min and std::max.
#include <algorithm>

BEGIN_NAMESPACE_ADS

//-----------------------------------------------------------------------------
/*! \defgroup algorithm_min_max Algorithm: Minimim and Maximum */
// @{

/*!
  \brief This does what you think it does.
  \param a A thing of arbitrary type.
  \param b Another thing of arbitrary type.
  \param c Yet another thing of arbitrary type.
  \return The least of the parameters.
*/
template<typename T>
inline 
const T&
min(const T& a, const T& b, const T& c) {
  return std::min(a, std::min(b, c));
}

/*!
  \brief This does what you think it does.
  \return The least of the parameters.
*/
template<typename T>
inline 
const T&
min(const T& a, const T& b, const T& c, const T& d) {
  return std::min(a, ads::min(b, c, d));
}

/*!
  \brief This does what you think it does.
  \return The least of the parameters.
*/
template<typename T>
inline 
const T&
min(const T& a, const T& b, const T& c, const T& d, const T& e) {
  return std::min(a, ads::min(b, c, d, e));
}

/*!
  \brief This does what you think it does.
  \param a A thing of arbitrary type.
  \param b Another thing of arbitrary type.
  \param c Yet another thing of arbitrary type.
  \return The greatest of the parameters.
*/
template<typename T>
inline 
const T&
max(const T& a, const T& b, const T& c) {
  return std::max(a, std::max(b, c));
}

/*!
  \brief This does what you think it does.
  \return The greatest of the parameters.
*/
template<typename T>
inline 
const T&
max(const T& a, const T& b, const T& c, const T& d) {
  return std::max(a, ads::max(b, c, d));
}

/*!
  \brief This does what you think it does.
  \return The greatest of the parameters.
*/
template<typename T>
inline 
const T&
max(const T& a, const T& b, const T& c, const T& d, const T& e) {
  return std::max(a, ads::max(b, c, d, e));
}

/*!
  \brief This does what you think it does.
  \param a A thing of arbitrary type.
  \param b Another thing of arbitrary type.
  \param c Yet another thing of arbitrary type.
  \param comp A comparison functor.
  \return The least of the parameters.
*/
template<typename T, typename Compare>
inline 
const T&
min(const T& a, const T& b, const T& c, Compare comp) {
  return std::min(a, std::min(b, c, comp), comp);
}

/*!
  \brief This does what you think it does.
  \c comp is a comparison functor.
  \return The least of the parameters.
*/
template<typename T, typename Compare>
inline 
const T&
min(const T& a, const T& b, const T& c, const T& d, Compare comp) {
  return std::min(a, ads::min(b, c, d, comp), comp);
}

/*!
  \brief This does what you think it does.
  \c comp is a comparison functor.
  \return The least of the parameters.
*/
template<typename T, typename Compare>
inline 
const T&
min(const T& a, const T& b, const T& c, const T& d, const T& e, 
    Compare comp) {
  return std::min(a, ads::min(b, c, d, e, comp), comp);
}

/*!
  \brief This does what you think it does.
  \param a A thing of arbitrary type.
  \param b Another thing of arbitrary type.
  \param c Yet another thing of arbitrary type.
  \param comp A comparison functor.
  \return The greatest of the parameters.
*/
template<typename T, typename Compare>
inline 
const T&
max(const T& a, const T& b, const T& c, Compare comp) {
  return std::max(a, std::max(b, c, comp), comp);
}

/*!
  \brief This does what you think it does.
  \c comp is a comparison functor.
  \return The greatest of the parameters.
*/
template<typename T, typename Compare>
inline 
const T&
max(const T& a, const T& b, const T& c, const T& d, Compare comp) {
  return std::max(a, ads::max(b, c, d, comp), comp);
}

/*!
  \brief This does what you think it does.
  \c comp is a comparison functor.
  \return The greatest of the parameters.
*/
template<typename T, typename Compare>
inline 
const T&
max(const T& a, const T& b, const T& c, const T& d, const T& e, 
    Compare comp) {
  return std::max(a, ads::max(b, c, d, e, comp), comp);
}

// @}

END_NAMESPACE_ADS

#endif
