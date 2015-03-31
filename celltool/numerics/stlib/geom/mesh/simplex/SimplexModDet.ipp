// -*- C++ -*-

#if !defined(__geom_SimplexModDet_ipp__)
#error This file is an implementation detail of the class SimplexModDet.
#endif

BEGIN_NAMESPACE_GEOM

//
// Mathematical Member Functions
//

template<typename T>
inline
T
SimplexModDet<T>::
getH(const Number determinant, const Number minDeterminant) {
  const Number d = getDelta(minDeterminant);
  // h is close to the determinant when it is positive.
  // h is small and positive when the determinant is negative.
  const Number h = (0.5 * (determinant + 
			   std::sqrt(determinant * determinant + 
				     4.0 * d * d)));
  assert(h > 0);
  return h;
}


//
// Static member data.
//

// CONTINUE 
// This may be too conservative.  
// However, if I use 10 times the machine precision 
// the modified eta function overflows for some inverted simplices.
template<>
SimplexModDet<double>::Number 
SimplexModDet<double>::
_epsilon(100.0 * std::numeric_limits<double>::epsilon());

template<>
SimplexModDet<float>::Number 
SimplexModDet<float>::
_epsilon(100.0f * std::numeric_limits<float>::epsilon());


END_NAMESPACE_GEOM

// End of file.
