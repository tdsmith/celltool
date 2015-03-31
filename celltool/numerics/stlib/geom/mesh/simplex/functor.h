// -*- C++ -*-

/*! 
  \file geom/mesh/simplex/functor.h
  \brief Various functors for simplices.
*/

#if !defined(__geom_mesh_simplex_functor_h__)
#define __geom_mesh_simplex_functor_h__

#include "SimplexJac.h"


BEGIN_NAMESPACE_GEOM

//---------------------------------------------------------------------------
// Determinant.
//---------------------------------------------------------------------------

//! Functor for the determinant of the Jacobian matrix.
template<int N, int M = N, typename T = double>
class SimplexDeterminant : 
  public std::unary_function<geom::Simplex<M, ads::FixedArray<N,T> >, T> {
private:

  //
  // Private types.
  //

  typedef std::unary_function<geom::Simplex<M, ads::FixedArray<N,T> >, T> Base;

public:

  //
  // Public types.
  //

  //! The argument type.
  typedef typename Base::argument_type argument_type;
  //! The result type.
  typedef typename Base::result_type result_type;

private:

  //
  // Member data.
  //

  //! The simplex Jacobian data structure.
  mutable SimplexJac<N,T> _simplexJacobian;

  //
  // Constructors.
  //

public:

  // Since the member data is mutable, the constructor, copy constructor,
  // and assignment operator are trivial.

  //! Default constructor.
  SimplexDeterminant()
  {}
  
  //! Copy constructor.
  SimplexDeterminant(const SimplexDeterminant& x)
  {}

  //! Assignment operator.
  SimplexDeterminant&
  operator=(const SimplexDeterminant& other) {
    return *this;
  }

  //! Destructor.
  ~SimplexDeterminant()
  {}

  //
  // Functor.
  //

  //! Return the determinant.
  result_type
  operator()(const argument_type& x) const {
    _simplexJacobian.setFunction(x);
    return _simplexJacobian.getDeterminant();
  }

};


//! Convenience function for constructing a \c SimplexDeterminant.
template<int N, int M, typename T>
inline
SimplexDeterminant<N,M,T>
simplexDeterminant() {
  return SimplexDeterminant<N,M,T>();
}




//---------------------------------------------------------------------------
// Content.
//---------------------------------------------------------------------------

//! Functor for the content of a simplex.
template<int N, int M = N, typename T = double>
class SimplexContent : 
  public std::unary_function<geom::Simplex<M, ads::FixedArray<N,T> >, T> {
private:

  //
  // Private types.
  //

  typedef std::unary_function<geom::Simplex<M, ads::FixedArray<N,T> >, T> Base;

public:

  //
  // Public types.
  //

  //! The argument type.
  typedef typename Base::argument_type argument_type;
  //! The result type.
  typedef typename Base::result_type result_type;

private:

  //
  // Member data.
  //

  //! The simplex Jacobian data structure.
  mutable SimplexJac<N,T> _simplexJacobian;

  //
  // Constructors.
  //

public:

  // Since the member data is mutable, the constructor, copy constructor,
  // and assignment operator are trivial.

  //! Default constructor.
  SimplexContent()
  {}
  
  //! Copy constructor.
  SimplexContent(const SimplexContent& x)
  {}

  //! Assignment operator.
  SimplexContent&
  operator=(const SimplexContent& other) {
    return *this;
  }

  //! Destructor.
  ~SimplexContent()
  {}

  //
  // Functor.
  //

  //! Return the content.
  result_type
  operator()(const argument_type& x) const {
    _simplexJacobian.setFunction(x);
    return _simplexJacobian.computeContent();
  }

};


//! Convenience function for constructing a \c SimplexContent.
template<int N, int M, typename T>
inline
SimplexContent<N,M,T>
simplexContent() {
  return SimplexContent<N,M,T>();
}




//---------------------------------------------------------------------------
// Minimum edge length.
//---------------------------------------------------------------------------

//! Functor for the minimum edge length of a simplex.
template<int N, int M = N, typename T = double>
class SimplexMinimumEdgeLength : 
  public std::unary_function<geom::Simplex<M, ads::FixedArray<N,T> >, T> {
private:

  //
  // Private types.
  //

  typedef std::unary_function<geom::Simplex<M, ads::FixedArray<N,T> >, T> Base;

public:

  //
  // Public types.
  //

  //! The argument type.
  typedef typename Base::argument_type argument_type;
  //! The result type.
  typedef typename Base::result_type result_type;

  //
  // Constructors.
  //

public:

  // Since there is no member data, the default constructor, destructor, 
  // copy constructor, and assignment operator are sufficient.

  //
  // Functor.
  //

  //! Return the minimum edge length.
  result_type
  operator()(const argument_type& x) const {
    result_type d, minimum = std::numeric_limits<result_type>::max();
    // For each edge (pair of vertices).
    for (int i = 0; i != M; ++i) {
      for (int j = i + 1; j != M + 1; ++j) {
	d = geom::computeDistance(x[i], x[j]);
	if (d < minimum) {
	  minimum = d;
	}
      }
    }
    return minimum;
  }

};


//! Convenience function for constructing a \c SimplexMinimumEdgeLength.
template<int N, int M, typename T>
inline
SimplexMinimumEdgeLength<N,M,T>
simplexMinimumEdgeLength() {
  return SimplexMinimumEdgeLength<N,M,T>();
}




//---------------------------------------------------------------------------
// Maximum edge length.
//---------------------------------------------------------------------------

//! Functor for the minimum edge length of a simplex.
template<int N, int M = N, typename T = double>
class SimplexMaximumEdgeLength : 
  public std::unary_function<geom::Simplex<M, ads::FixedArray<N,T> >, T> {
private:

  //
  // Private types.
  //

  typedef std::unary_function<geom::Simplex<M, ads::FixedArray<N,T> >, T> Base;

public:

  //
  // Public types.
  //

  //! The argument type.
  typedef typename Base::argument_type argument_type;
  //! The result type.
  typedef typename Base::result_type result_type;

  //
  // Constructors.
  //

public:

  // Since there is no member data, the default constructor, destructor, 
  // copy constructor, and assignment operator are sufficient.

  //
  // Functor.
  //

  //! Return the maximum edge length.
  result_type
  operator()(const argument_type& x) const {
    result_type d, maximum = -std::numeric_limits<result_type>::max();
    // For each edge (pair of vertices).
    for (int i = 0; i != M; ++i) {
      for (int j = i + 1; j != M + 1; ++j) {
	d = geom::computeDistance(x[i], x[j]);
	if (d > maximum) {
	  maximum = d;
	}
      }
    }
    return maximum;
  }

};


//! Convenience function for constructing a \c SimplexMaximumEdgeLength.
template<int N, int M, typename T>
inline
SimplexMaximumEdgeLength<N,M,T>
simplexMaximumEdgeLength() {
  return SimplexMaximumEdgeLength<N,M,T>();
}

END_NAMESPACE_GEOM

#define __geom_mesh_simplex_functor_ipp__
#include "functor.ipp"
#undef __geom_mesh_simplex_functor_ipp__

#endif
