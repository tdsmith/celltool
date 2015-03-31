// -*- C++ -*-

#if !defined(__geom_mesh_iss_cellAttributes_ipp__)
#error This file is an implementation detail.
#endif

BEGIN_NAMESPACE_GEOM


//----------------------------------------------------------------------------
// Mean ratio.
//----------------------------------------------------------------------------


// Calculate mean ratio function for each simplex in the mesh.
template<int M, typename T, typename VertRAIter, typename ISInIter,
	 typename OutputIterator>
inline
void
computeMeanRatio(VertRAIter vertices,
		 ISInIter indexedSimplicesBeginning, 
		 ISInIter indexedSimplicesEnd,
		 OutputIterator output) {
  typedef typename std::iterator_traits<VertRAIter>::value_type Vertex;
  typedef Simplex<M,Vertex> Simplex;

  Simplex s;
  SimplexMeanRatio<M,T> functor;
  // Loop over the simplices.
  for (; indexedSimplicesBeginning != indexedSimplicesEnd; 
       ++indexedSimplicesBeginning) {
    s.build(vertices, *indexedSimplicesBeginning);
    functor.setFunction(s);
    *output = 1.0 / functor();
    ++output;
  }
}



// Calculate mean ratio function for each simplex in the mesh.
template<int M, typename T, typename SimpInIter, typename OutputIterator>
inline
void
computeMeanRatio(SimpInIter simplicesBeginning, 
		 SimpInIter simplicesEnd,
		 OutputIterator output) {
  SimplexMeanRatio<M,T> functor;
  // Loop over the simplices.
  for (; simplicesBeginning != simplicesEnd; ++simplicesBeginning) {
    functor.setFunction(*simplicesBeginning);
    *output = 1.0 / functor();
    ++output;
  }
}



// Calculate mean ratio function for each simplex in the mesh.
template<int N, int M, bool A, typename T, typename V, typename IS,
	 typename OutputIterator>
inline
void
computeMeanRatio(const IndSimpSet<N,M,A,T,V,IS>& iss,
		 OutputIterator output) {
  computeMeanRatio<M,T>(iss.getVerticesBeginning(), 
			iss.getIndexedSimplicesBeginning(), 
			iss.getIndexedSimplicesEnd(),
			output);
}


//----------------------------------------------------------------------------
// Modified mean ratio.
//----------------------------------------------------------------------------


// Calculate modified mean ratio function for each simplex in the mesh.
template<int M, typename T, typename VertRAIter, typename ISInIter,
	 typename OutputIterator>
inline
void
computeModifiedMeanRatio(VertRAIter vertices,
			 ISInIter indexedSimplicesBeginning, 
			 ISInIter indexedSimplicesEnd,
			 OutputIterator output) {
  typedef typename std::iterator_traits<VertRAIter>::value_type Vertex;
  typedef Simplex<M,Vertex> Simplex;

  Simplex s;
  SimplexModMeanRatio<M,T> functor;
  // Loop over the simplices.
  for (; indexedSimplicesBeginning != indexedSimplicesEnd; 
       ++indexedSimplicesBeginning) {
    s.build(vertices, *indexedSimplicesBeginning);
    functor.setFunction(s);
    *output = 1.0 / functor();
    ++output;
  }
}



// Calculate modified mean ratio function for each simplex in the mesh.
template<int M, typename T, typename SimpInIter, typename OutputIterator>
inline
void
computeModifiedMeanRatio(SimpInIter simplicesBeginning, 
			 SimpInIter simplicesEnd,
			 OutputIterator output) {
  SimplexModMeanRatio<M,T> functor;
  // Loop over the simplices.
  for (; simplicesBeginning != simplicesEnd; ++simplicesBeginning) {
    functor.setFunction(*simplicesBeginning);
    *output = 1.0 / functor();
    ++output;
  }
}



// Calculate modified mean ratio function for each simplex in the mesh.
template<int N, int M, bool A, typename T, typename V, typename IS,
	 typename OutputIterator>
inline
void
computeModifiedMeanRatio(const IndSimpSet<N,M,A,T,V,IS>& iss,
			 OutputIterator output) {
  computeModifiedMeanRatio<M,T>(iss.getVerticesBeginning(), 
				iss.getIndexedSimplicesBeginning(), 
				iss.getIndexedSimplicesEnd(),
				output);
}


//----------------------------------------------------------------------------
// Condition number.
//----------------------------------------------------------------------------


// Calculate condition number function for each simplex in the mesh.
template<int M, typename T, typename VertRAIter, typename ISInIter,
	 typename OutputIterator>
inline
void
computeConditionNumber(VertRAIter vertices,
		       ISInIter indexedSimplicesBeginning, 
		       ISInIter indexedSimplicesEnd,
		       OutputIterator output) {
  typedef typename std::iterator_traits<VertRAIter>::value_type Vertex;
  typedef Simplex<M,Vertex> Simplex;

  Simplex s;
  SimplexCondNum<M,T> functor;
  // Loop over the simplices.
  for (; indexedSimplicesBeginning != indexedSimplicesEnd; 
       ++indexedSimplicesBeginning) {
    s.build(vertices, *indexedSimplicesBeginning);
    functor.setFunction(s);
    *output = 1.0 / functor();
    ++output;
  }
}



// Calculate condition number function for each simplex in the mesh.
template<int M, typename T, typename SimpInIter, typename OutputIterator>
inline
void
computeConditionNumber(SimpInIter simplicesBeginning, 
		       SimpInIter simplicesEnd,
		       OutputIterator output) {

  SimplexCondNum<M,T> functor;
  // Loop over the simplices.
  for (; simplicesBeginning != simplicesEnd; ++simplicesBeginning) {
    functor.setFunction(*simplicesBeginning);
    *output = 1.0 / functor();
    ++output;
  }
}



// Calculate condition number function for each simplex in the mesh.
template<int N, int M, bool A, typename T, typename V, typename IS,
	 typename OutputIterator>
inline
void
computeConditionNumber(const IndSimpSet<N,M,A,T,V,IS>& iss,
		       OutputIterator output) {
  computeConditionNumber<M,T>(iss.getVerticesBeginning(), 
			      iss.getIndexedSimplicesBeginning(), 
			iss.getIndexedSimplicesEnd(),
			output);
}


//----------------------------------------------------------------------------
// Modified condition number.
//----------------------------------------------------------------------------


// Calculate modified condition number function for each simplex in the mesh.
template<int M, typename T, typename VertRAIter, typename ISInIter,
	 typename OutputIterator>
inline
void
computeModifiedConditionNumber(VertRAIter vertices,
			       ISInIter indexedSimplicesBeginning, 
			       ISInIter indexedSimplicesEnd,
			       OutputIterator output) {
  typedef typename std::iterator_traits<VertRAIter>::value_type Vertex;
  typedef Simplex<M,Vertex> Simplex;

  Simplex s;
  SimplexModCondNum<M,T> functor;
  // Loop over the simplices.
  for (; indexedSimplicesBeginning != indexedSimplicesEnd; 
       ++indexedSimplicesBeginning) {
    s.build(vertices, *indexedSimplicesBeginning);
    functor.setFunction(s);
    *output = 1.0 / functor();
    ++output;
  }
}



// Calculate modified condition number function for each simplex in the mesh.
template<int M, typename T, typename SimpInIter, typename OutputIterator>
inline
void
computeModifiedConditionNumber(SimpInIter simplicesBeginning, 
			       SimpInIter simplicesEnd,
			       OutputIterator output) {
  SimplexModCondNum<M,T> functor;
  // Loop over the simplices.
  for (; simplicesBeginning != simplicesEnd; ++simplicesBeginning) {
    functor.setFunction(*simplicesBeginning);
    *output = 1.0 / functor();
    ++output;
  }
}



// Calculate modified condition number function for each simplex in the mesh.
template<int N, int M, bool A, typename T, typename V, typename IS,
	 typename OutputIterator>
inline
void
computeModifiedConditionNumber(const IndSimpSet<N,M,A,T,V,IS>& iss,
			       OutputIterator output) {
  computeModifiedConditionNumber<M,T>(iss.getVerticesBeginning(), 
				iss.getIndexedSimplicesBeginning(), 
				iss.getIndexedSimplicesEnd(),
				output);
}


//----------------------------------------------------------------------------
// Content.
//----------------------------------------------------------------------------


// Calculate the content for each simplex in the mesh.
template<int M, typename T, typename VertRAIter, typename ISInIter,
	 typename OutputIterator>
inline
void
computeContent(VertRAIter vertices,
		 ISInIter indexedSimplicesBeginning, 
		 ISInIter indexedSimplicesEnd,
		 OutputIterator output) {
  typedef typename std::iterator_traits<VertRAIter>::value_type Vertex;
  typedef Simplex<M,Vertex> Simplex;

  Simplex s;
  SimplexJac<M,T> simplex;
  // Loop over the simplices.
  for (; indexedSimplicesBeginning != indexedSimplicesEnd; 
       ++indexedSimplicesBeginning) {
    s.build(vertices, *indexedSimplicesBeginning);
    simplex.setFunction(s);
    *output = simplex.computeContent();
    ++output;
  }
}



// Calculate the content for each simplex in the mesh.
template<int M, typename T, typename SimpInIter, typename OutputIterator>
inline
void
computeContent(SimpInIter simplicesBeginning, 
		 SimpInIter simplicesEnd,
		 OutputIterator output) {
  SimplexJac<M,T> simplex;
  // Loop over the simplices.
  for (; simplicesBeginning != simplicesEnd; ++simplicesBeginning) {
    simplex.setFunction(*simplicesBeginning);
    *output = simplex.computeContent();
    ++output;
  }
}



// Calculate the content for each simplex in the mesh.
template<int N, int M, bool A, typename T, typename V, typename IS,
	 typename OutputIterator>
inline
void
computeContent(const IndSimpSet<N,M,A,T,V,IS>& iss,
		 OutputIterator output) {
  computeContent<M,T>(iss.getVerticesBeginning(), 
		      iss.getIndexedSimplicesBeginning(), 
		      iss.getIndexedSimplicesEnd(),
		      output);
}

END_NAMESPACE_GEOM

// End of file.
