// -*- C++ -*-

#if !defined(__geom_mesh_iss_quality_ipp__)
#error This file is an implementation detail.
#endif

BEGIN_NAMESPACE_GEOM


// Calculate the adjacency counts for the simplices in the mesh.
template<int N, int M, bool A, typename T, typename V, typename IS>
inline
void
countAdjacencies(const IndSimpSetIncAdj<N,M,A,T,V,IS>& iss,
		 ads::FixedArray<M+2,int>* counts) {
  *counts = 0;
  // For each simplex.
  for (int n = 0; n != iss.getSimplicesSize(); ++n) {
    ++(*counts)[iss.getAdjacentSize(n)];
  }
}



// Compute edge length statistics.
// With a bucket of simplices, one can compute the minimum and maximum, but
// not the mean.
template<int M, typename SimpInIter, typename T>
inline
void
computeEdgeLengthStatistics(SimpInIter simplicesBeginning, 
			    SimpInIter simplicesEnd,
			    T* minimumLength,
			    T* maximumLength) {
  *minimumLength = std::numeric_limits<T>::max();
  *maximumLength = -std::numeric_limits<T>::max();
  T d;
  // For each simplex.
  for (; simplicesBeginning != simplicesEnd; ++simplicesBeginning) {
    // For each edge (pair of vertices).
    for (int i = 0; i != M; ++i) {
      for (int j = i + 1; j != M + 1; ++j) {
	d = geom::computeDistance((*simplicesBeginning)[i], 
				  (*simplicesBeginning)[j]);
	if (d < *minimumLength) {
	  *minimumLength = d;
	}
	if (d > *maximumLength) {
	  *maximumLength = d;
	}
      }
    }
  }
}



// Compute edge length statistics.
template<int M, typename VertRAIter, typename ISInIter, typename T>
void
computeEdgeLengthStatistics(VertRAIter vertices,
			    ISInIter indexedSimplicesBeginning, 
			    ISInIter indexedSimplicesEnd,
			    T* minimumLength,
			    T* maximumLength) {
  *minimumLength = std::numeric_limits<T>::max();
  *maximumLength = -std::numeric_limits<T>::max();
  T d;
  // For each simplex.
  for (; indexedSimplicesBeginning != indexedSimplicesEnd; 
       ++indexedSimplicesBeginning) {
    // For each edge (pair of vertices).
    for (int i = 0; i != M; ++i) {
      for (int j = i + 1; j != M + 1; ++j) {
	d = geom::computeDistance(vertices[(*indexedSimplicesBeginning)[i]], 
				  vertices[(*indexedSimplicesBeginning)[j]]);
	if (d < *minimumLength) {
	  *minimumLength = d;
	}
	if (d > *maximumLength) {
	  *maximumLength = d;
	}
      }
    }
  }
}



// Compute edge length statistics.
// CONTINUE: Add a edge iterator to IndSimpSetIncAdj.  Then I could easily
// implement this for M = 3.
template<int N, bool A, typename T, typename V, typename IS>
inline
void
computeEdgeLengthStatistics(const IndSimpSetIncAdj<N,2,A,T,V,IS>& mesh,
			    T* minimumLength,
			    T* maximumLength,
			    T* meanLength) {
  typedef typename IndSimpSetIncAdj<N,2,A,T,V,IS>::FaceIterator FaceIterator;

  const int M = 2;

  // Initialize the statistics.
  *minimumLength = std::numeric_limits<T>::max();
  *maximumLength = -std::numeric_limits<T>::max();
  *meanLength = 0;

  int numberOfEdges = 0;
  int simplexIndex, localIndex, vertexIndex1, vertexIndex2;
  T length;
  // For each face (edge).
  const FaceIterator iEnd = mesh.getFacesEnd();
  for (FaceIterator i = mesh.getFacesBeginning(); i != iEnd; ++i) {
    ++numberOfEdges;
    simplexIndex = i->first;
    localIndex = i->second;
    vertexIndex1 = mesh.getIndexedSimplex(simplexIndex)[(localIndex + 1) % 
							(M + 1)];
    vertexIndex2 = mesh.getIndexedSimplex(simplexIndex)[(localIndex + 2) % 
							(M + 1)];
    length = geom::computeDistance(mesh.getVertex(vertexIndex1),
				   mesh.getVertex(vertexIndex2));
    if (length < *minimumLength) {
      *minimumLength = length;
    }
    if (length > *maximumLength) {
      *maximumLength = length;
    }
    *meanLength += length;
  }
  if (numberOfEdges != 0) {
    *meanLength /= numberOfEdges;
  }
}



// Return the minimum edge length.
template<int M, typename T, typename SimpInIter>
inline
T
computeMinimumEdgeLength(SimpInIter simplicesBeginning, 
			 SimpInIter simplicesEnd) {
  T minimumLength, maximumLength;
  computeEdgeLengthStatistics<M>(simplicesBeginning, simplicesEnd, 
				 &minimumLength, &maximumLength);
  return minimumLength;
}



// Return the maximum edge length.
template<int M, typename T, typename SimpInIter>
inline
T
computeMaximumEdgeLength(SimpInIter simplicesBeginning, 
			 SimpInIter simplicesEnd) {
  T minimumLength, maximumLength;
  computeEdgeLengthStatistics<M>(simplicesBeginning, simplicesEnd, 
				 &minimumLength, &maximumLength);
  return maximumLength;
}



// Return the total content of the simplices in the mesh.
template<int M, typename T, typename VertRAIter, typename ISInIter>
inline
T
computeContent(VertRAIter vertices,
	       ISInIter indexedSimplicesBeginning, 
	       ISInIter indexedSimplicesEnd) {
  typedef typename std::iterator_traits<VertRAIter>::value_type Vertex;
  typedef Simplex<M,Vertex> Simplex;
  
  Simplex s;
  SimplexJac<M,T> sj;
  T c = 0;
  // Loop over the simplices.
  for (; indexedSimplicesBeginning != indexedSimplicesEnd; 
       ++indexedSimplicesBeginning) {
    s.build(vertices, *indexedSimplicesBeginning);
    sj.setFunction(s);
    c += sj.computeContent();
  }
  return c;
}


// Return the total content of the simplices in the mesh.
template<int M, typename T, typename SimpInIter>
inline
T
computeContent(SimpInIter simplicesBeginning, SimpInIter simplicesEnd) {
  SimplexJac<M,T> sj;
  T c = 0;
  // Loop over the simplices.
  for (; simplicesBeginning != simplicesEnd; ++simplicesBeginning) {
    sj.setFunction(*simplicesBeginning);
    c += sj.computeContent();
  }
  return c;
}


// Return the total content of the simplices in the mesh.
template<int N, int M, bool A, typename T, typename V, typename IS>
inline
T
computeContent(const IndSimpSet<N,M,A,T,V,IS>& iss) {
  return computeContent<M,T>(iss.getVerticesBeginning(), 
			     iss.getIndexedSimplicesBeginning(),
			     iss.getIndexedSimplicesEnd());
}



// Calculate content (hypervolume) statistics for the simplices in the mesh.
template<int M, typename T, typename VertRAIter, typename ISInIter>
inline
void
computeContentStatistics(VertRAIter vertices,
			 ISInIter indexedSimplicesBeginning, 
			 ISInIter indexedSimplicesEnd,
			 T* minContent, 
			 T* maxContent,
			 T* meanContent) {
  typedef typename std::iterator_traits<VertRAIter>::value_type Vertex;
  typedef Simplex<M,Vertex> Simplex;

  T minCont = std::numeric_limits<T>::max();
  T maxCont = -std::numeric_limits<T>::max();
  T sumCont = 0;
  T x;

  Simplex s;
  SimplexJac<M,T> sj;
  // Loop over the simplices.
  int numSimplices = 0;
  for (; indexedSimplicesBeginning != indexedSimplicesEnd; 
       ++indexedSimplicesBeginning, ++numSimplices) {
    s.build(vertices, *indexedSimplicesBeginning);
    sj.setFunction(s);
    x = sj.computeContent();
    if (x < minCont) {
      minCont = x;
    }
    if (x > maxCont) {
      maxCont = x;
    }
    sumCont += x;
  }

  if (numSimplices == 0) {
    *minContent = 0;
    *maxContent = 0;
    *meanContent = 0;
  }
  else {
    *minContent = minCont;
    *maxContent = maxCont;
    *meanContent = sumCont / numSimplices;
  }
}



// Calculate content (hypervolume) statistics for the simplices in the mesh.
template<int M, typename T, typename SimpInIter>
inline
void
computeContentStatistics(SimpInIter simplicesBeginning, 
			 SimpInIter simplicesEnd,
			 T* minContent,
			 T* maxContent,
			 T* meanContent) {
  T minCont = std::numeric_limits<T>::max();
  T maxCont = -std::numeric_limits<T>::max();
  T sumCont = 0;
  T x;

  SimplexJac<M,T> sj;
  // Loop over the simplices.
  int numSimplices = 0;
  for (; simplicesBeginning != simplicesEnd; 
       ++simplicesBeginning, ++numSimplices) {
    sj.setFunction(*simplicesBeginning);
    x = sj.computeContent();
    if (x < minCont) {
      minCont = x;
    }
    if (x > maxCont) {
      maxCont = x;
    }
    sumCont += x;
  }

  if (numSimplices == 0) {
    *minContent = 0;
    *maxContent = 0;
    *meanContent = 0;
  }
  else {
    *minContent = minCont;
    *maxContent = maxCont;
    *meanContent = sumCont / numSimplices;
  }
}



// Calculate content (hypervolume) statistics for the simplices in the mesh.
template<int N, int M, bool A, typename T, typename V, typename IS>
inline
void
computeContentStatistics(const IndSimpSet<N,M,A,T,V,IS>& iss,
			 T* minContent, 
			 T* maxContent,
			 T* meanContent) {
  computeContentStatistics<M>(iss.getVerticesBeginning(), 
			      iss.getIndexedSimplicesBeginning(), 
			      iss.getIndexedSimplicesEnd(),
			      minContent, maxContent, meanContent);
}




// Calculate determinant statistics for the simplices in the mesh.
template<int M, typename T, typename VertRAIter, typename ISInIter>
inline
void
computeDeterminantStatistics(VertRAIter vertices,
			     ISInIter indexedSimplicesBeginning, 
			     ISInIter indexedSimplicesEnd,
			     T* minDeterminant, 
			     T* maxDeterminant,
			     T* meanDeterminant) {
  typedef typename std::iterator_traits<VertRAIter>::value_type Vertex;
  typedef Simplex<M,Vertex> Simplex;

  T minDet = std::numeric_limits<T>::max();
  T maxDet = -std::numeric_limits<T>::max();
  T sumDet = 0;
  T x;

  Simplex s;
  SimplexJac<M,T> sj;
  // Loop over the simplices.
  int numSimplices = 0;
  for (; indexedSimplicesBeginning != indexedSimplicesEnd; 
       ++indexedSimplicesBeginning, ++numSimplices) {
    s.build(vertices, *indexedSimplicesBeginning);
    sj.setFunction(s);
    x = sj.getDeterminant();
    if (x < minDet) {
      minDet = x;
    }
    if (x > maxDet) {
      maxDet = x;
    }
    sumDet += x;
  }

  if (numSimplices == 0) {
    *minDeterminant = 0;
    *maxDeterminant = 0;
    *meanDeterminant = 0;
  }
  else {
    *minDeterminant = minDet;
    *maxDeterminant = maxDet;
    *meanDeterminant = sumDet / numSimplices;
  }
}



// Calculate determinant statistics for the simplices in the mesh.
template<int M, typename T, typename SimpInIter>
inline
void
computeDeterminantStatistics(SimpInIter simplicesBeginning, 
			     SimpInIter simplicesEnd,
			     T* minDeterminant, 
			     T* maxDeterminant,
			     T* meanDeterminant) {
  T minDet = std::numeric_limits<T>::max();
  T maxDet = -std::numeric_limits<T>::max();
  T sumDet = 0;
  T x;

  SimplexJac<M,T> sj;
  // Loop over the simplices.
  int numSimplices = 0;
  for (; simplicesBeginning != simplicesEnd; 
       ++simplicesBeginning, ++numSimplices) {
    sj.setFunction(*simplicesBeginning);
    x = sj.getDeterminant();
    if (x < minDet) {
      minDet = x;
    }
    if (x > maxDet) {
      maxDet = x;
    }
    sumDet += x;
  }

  if (numSimplices == 0) {
    *minDeterminant = 0;
    *maxDeterminant = 0;
    *meanDeterminant = 0;
  }
  else {
    *minDeterminant = minDet;
    *maxDeterminant = maxDet;
    *meanDeterminant = sumDet / numSimplices;
  }
}



// Calculate determinant statistics for the simplices in the mesh.
template<int N, int M, bool A, typename T, typename V, typename IS>
inline
void
computeDeterminantStatistics(const IndSimpSet<N,M,A,T,V,IS>& iss,
			     T* minDeterminant, 
			     T* maxDeterminant,
			     T* meanDeterminant) {
  computeDeterminantStatistics<M>(iss.getVerticesBeginning(), 
				  iss.getIndexedSimplicesBeginning(), 
				  iss.getIndexedSimplicesEnd(),
				  minDeterminant, maxDeterminant, 
				  meanDeterminant);
}






// Calculate modified mean ratio function statistics for the simplices in the mesh.
template<int M, typename T, typename VertRAIter, typename ISInIter>
inline
void
computeModifiedMeanRatioStatistics(VertRAIter vertices,
				   ISInIter indexedSimplicesBeginning, 
				   ISInIter indexedSimplicesEnd,
				   T* minModMeanRatio, 
				   T* maxModMeanRatio,
				   T* meanModMeanRatio) {
  typedef typename std::iterator_traits<VertRAIter>::value_type Vertex;
  typedef Simplex<M,Vertex> Simplex;

  T minMmr = std::numeric_limits<T>::max();
  T maxMmr = -std::numeric_limits<T>::max();
  T sumMmr = 0;
  T x;

  Simplex s;
  SimplexModMeanRatio<M,T> smmr;
  // Loop over the simplices.
  int numSimplices = 0;
  for (; indexedSimplicesBeginning != indexedSimplicesEnd; 
       ++indexedSimplicesBeginning, ++numSimplices) {
    s.build(vertices, *indexedSimplicesBeginning);
    smmr.setFunction(s);
    x = smmr();
    if (x < minMmr) {
      minMmr = x;
    }
    if (x > maxMmr) {
      maxMmr = x;
    }
    sumMmr += x;
  }

  if (numSimplices == 0) {
    *minModMeanRatio = 0;
    *maxModMeanRatio = 0;
    *meanModMeanRatio = 0;
  }
  else {
    *minModMeanRatio = minMmr;
    *maxModMeanRatio = maxMmr;
    *meanModMeanRatio = sumMmr / numSimplices;
  }
}


// Calculate modified mean ratio function statistics for the simplices in the mesh.
template<int M, typename T, typename SimpInIter>
inline
void
computeModifiedMeanRatioStatistics(SimpInIter simplicesBeginning, 
				   SimpInIter simplicesEnd,
				   T* minModMeanRatio, 
				   T* maxModMeanRatio,
				   T* meanModMeanRatio) {
  T minMmr = std::numeric_limits<T>::max();
  T maxMmr = -std::numeric_limits<T>::max();
  T sumMmr = 0;
  T x;

  SimplexModMeanRatio<M,T> smmr;
  // Loop over the simplices.
  int numSimplices = 0;
  for (; simplicesBeginning != simplicesEnd; 
       ++simplicesBeginning, ++numSimplices) {
    smmr.setFunction(*simplicesBeginning);
    x = smmr();
    if (x < minMmr) {
      minMmr = x;
    }
    if (x > maxMmr) {
      maxMmr = x;
    }
    sumMmr += x;
  }

  if (numSimplices == 0) {
    *minModMeanRatio = 0;
    *maxModMeanRatio = 0;
    *meanModMeanRatio = 0;
  }
  else {
    *minModMeanRatio = minMmr;
    *maxModMeanRatio = maxMmr;
    *meanModMeanRatio = sumMmr / numSimplices;
  }
}


// Calculate modified mean ratio function statistics for the simplices in the mesh.
template<int N, int M, bool A, typename T, typename V, typename IS>
inline
void
computeModifiedMeanRatioStatistics(const IndSimpSet<N,M,A,T,V,IS>& iss,
				   T* minModMeanRatio, 
				   T* maxModMeanRatio,
				   T* meanModMeanRatio) {
  computeModifiedMeanRatioStatistics<M>(iss.getVerticesBeginning(), 
					iss.getIndexedSimplicesBeginning(), 
					iss.getIndexedSimplicesEnd(),
					minModMeanRatio, maxModMeanRatio, 
					meanModMeanRatio);
}







// Calculate modified condition number function statistics for the simplices in the mesh.
template<int M, typename T, typename VertRAIter, typename ISInIter>
inline
void
computeModifiedConditionNumberStatistics(VertRAIter vertices,
					 ISInIter indexedSimplicesBeginning, 
					 ISInIter indexedSimplicesEnd,
					 T* minModCondNum, 
					 T* maxModCondNum,
					 T* meanModCondNum) {
  typedef typename std::iterator_traits<VertRAIter>::value_type Vertex;
  typedef Simplex<M,Vertex> Simplex;

  T minMcn = std::numeric_limits<T>::max();
  T maxMcn = -std::numeric_limits<T>::max();
  T sumMcn = 0;
  T x;

  Simplex s;
  SimplexModMeanRatio<M,T> smcn;
  // Loop over the simplices.
  int numSimplices = 0;
  for (; indexedSimplicesBeginning != indexedSimplicesEnd; ++indexedSimplicesBeginning, ++numSimplices) {
    s.build(vertices, *indexedSimplicesBeginning);
    smcn.setFunction(s);
    x = smcn();
    if (x < minMcn) {
      minMcn = x;
    }
    if (x > maxMcn) {
      maxMcn = x;
    }
    sumMcn += x;
  }

  if (numSimplices == 0) {
    *minModCondNum = 0;
    *maxModCondNum = 0;
    *meanModCondNum = 0;
  }
  else {
    *minModCondNum = minMcn;
    *maxModCondNum = maxMcn;
    *meanModCondNum = sumMcn / numSimplices;
  }
}


// Calculate modified condition number function statistics for the simplices in the mesh.
template<int M, typename T, typename SimpInIter>
inline
void
computeModifiedConditionNumberStatistics(SimpInIter simplicesBeginning, 
					 SimpInIter simplicesEnd,
					 T* minModCondNum, 
					 T* maxModCondNum,
					 T* meanModCondNum) {
  T minMcn = std::numeric_limits<T>::max();
  T maxMcn = -std::numeric_limits<T>::max();
  T sumMcn = 0;
  T x;

  SimplexModMeanRatio<M,T> smcn;
  // Loop over the simplices.
  int numSimplices = 0;
  for (; simplicesBeginning != simplicesEnd; 
       ++simplicesBeginning, ++numSimplices) {
    smcn.setFunction(*simplicesBeginning);
    x = smcn();
    if (x < minMcn) {
      minMcn = x;
    }
    if (x > maxMcn) {
      maxMcn = x;
    }
    sumMcn += x;
  }

  if (numSimplices == 0) {
    *minModCondNum = 0;
    *maxModCondNum = 0;
    *meanModCondNum = 0;
  }
  else {
    *minModCondNum = minMcn;
    *maxModCondNum = maxMcn;
    *meanModCondNum = sumMcn / numSimplices;
  }
}


// Calculate modified condition number function statistics for the simplices in the mesh.
template<int N, int M, bool A, typename T, typename V, typename IS>
inline
void
computeModifiedConditionNumberStatistics(const IndSimpSet<N,M,A,T,V,IS>& iss,
					 T* minModCondNum, 
					 T* maxModCondNum,
					 T* meanModCondNum) {
  computeModifiedConditionNumberStatistics<M>
    (iss.getVerticesBeginning(), 
     iss.getIndexedSimplicesBeginning(), 
     iss.getIndexedSimplicesEnd(),
     minModCondNum, maxModCondNum, meanModCondNum);
}





// Calculate quality statistics for the simplices in the mesh.
template<int M, typename T, typename VertRAIter, typename ISInIter>
inline
void
computeQualityStatistics(VertRAIter vertices,
			 ISInIter indexedSimplicesBeginning, 
			 ISInIter indexedSimplicesEnd,
			 T* minContent, 
			 T* maxContent,
			 T* meanContent,
			 T* minDeterminant, 
			 T* maxDeterminant,
			 T* meanDeterminant,
			 T* minModMeanRatio,
			 T* maxModMeanRatio,
			 T* meanModMeanRatio,
			 T* minModCondNum, 
			 T* maxModCondNum,
			 T* meanModCondNum) {
  typedef typename std::iterator_traits<VertRAIter>::value_type Vertex;
  typedef Simplex<M,Vertex> Simplex;

  T minCont = std::numeric_limits<T>::max();
  T maxCont = -std::numeric_limits<T>::max();
  T sumCont = 0;
  T minDet = std::numeric_limits<T>::max();
  T maxDet = -std::numeric_limits<T>::max();
  T sumDet = 0;
  T minMmr = std::numeric_limits<T>::max();
  T maxMmr = -std::numeric_limits<T>::max();
  T sumMmr = 0;
  T minMcn = std::numeric_limits<T>::max();
  T maxMcn = -std::numeric_limits<T>::max();
  T sumMcn = 0;
  T x;

  Simplex s;
  SimplexModMeanRatio<M,T> smmr;
  SimplexModCondNum<M,T> smcn;
  // Loop over the simplices.
  int numSimplices = 0;
  for (; indexedSimplicesBeginning != indexedSimplicesEnd; 
       ++indexedSimplicesBeginning, ++numSimplices) {
    s.build(vertices, *indexedSimplicesBeginning);
    smmr.setFunction(s);
    smcn.setFunction(s);
    x = smmr.computeContent();
    if (x < minCont) {
      minCont = x;
    }
    if (x > maxCont) {
      maxCont = x;
    }
    sumCont += x;

    x = smmr.getDeterminant();
    if (x < minDet) {
      minDet = x;
    }
    if (x > maxDet) {
      maxDet = x;
    }
    sumDet += x;

    x = 1.0 / smmr();
    if (x < minMmr) {
      minMmr = x;
    }
    if (x > maxMmr) {
      maxMmr = x;
    }
    sumMmr += x;

    x = 1.0 / smcn();
    if (x < minMcn) {
      minMcn = x;
    }
    if (x > maxMcn) {
      maxMcn = x;
    }
    sumMcn += x;
  }

  if (numSimplices == 0) {
    *minContent = 0;
    *maxContent = 0;
    *meanContent = 0;
    *minDeterminant = 0;
    *maxDeterminant = 0;
    *meanDeterminant = 0;
    *minModMeanRatio = 0;
    *maxModMeanRatio = 0;
    *meanModMeanRatio = 0;
    *minModCondNum = 0;
    *maxModCondNum = 0;
    *meanModCondNum = 0;
  }
  else {
    *minContent = minCont;
    *maxContent = maxCont;
    *meanContent = sumCont / numSimplices;
    *minDeterminant = minDet;
    *maxDeterminant = maxDet;
    *meanDeterminant = sumDet / numSimplices;
    *minModMeanRatio = minMmr;
    *maxModMeanRatio = maxMmr;
    *meanModMeanRatio = sumMmr / numSimplices;
    *minModCondNum = minMcn;
    *maxModCondNum = maxMcn;
    *meanModCondNum = sumMcn / numSimplices;
  }
}


// Calculate quality statistics for the simplices in the mesh.
template<int M, typename T, typename SimpInIter>
inline
void
computeQualityStatistics(SimpInIter simplicesBeginning, 
			 SimpInIter simplicesEnd,
			 T* minContent, 
			 T* maxContent,
			 T* meanContent,
			 T* minDeterminant, 
			 T* maxDeterminant,
			 T* meanDeterminant,
			 T* minModMeanRatio,
			 T* maxModMeanRatio,
			 T* meanModMeanRatio,
			 T* minModCondNum, 
			 T* maxModCondNum,
			 T* meanModCondNum) {
  typedef typename std::iterator_traits<SimpInIter>::value_type Value;
  typedef typename Loki::TypeTraits<Value>::UnqualifiedType Simplex;

  T minCont = std::numeric_limits<T>::max();
  T maxCont = -std::numeric_limits<T>::max();
  T sumCont = 0;
  T minDet = std::numeric_limits<T>::max();
  T maxDet = -std::numeric_limits<T>::max();
  T sumDet = 0;
  T minMmr = std::numeric_limits<T>::max();
  T maxMmr = -std::numeric_limits<T>::max();
  T sumMmr = 0;
  T minMcn = std::numeric_limits<T>::max();
  T maxMcn = -std::numeric_limits<T>::max();
  T sumMcn = 0;
  T x;

  Simplex s;
  SimplexModMeanRatio<M,T> smmr;
  SimplexModCondNum<M,T> smcn;
  // Loop over the simplices.
  int numSimplices = 0;
  for (; simplicesBeginning != simplicesEnd; ++simplicesBeginning, ++numSimplices) {
    s = *simplicesBeginning;
    smmr.setFunction(s);
    smcn.setFunction(s);
    x = smmr.computeContent();
    if (x < minCont) {
      minCont = x;
    }
    if (x > maxCont) {
      maxCont = x;
    }
    sumCont += x;

    x = smmr.getDeterminant();
    if (x < minDet) {
      minDet = x;
    }
    if (x > maxDet) {
      maxDet = x;
    }
    sumDet += x;

    x = 1.0 / smmr();
    if (x < minMmr) {
      minMmr = x;
    }
    if (x > maxMmr) {
      maxMmr = x;
    }
    sumMmr += x;

    x = 1.0 / smcn();
    if (x < minMcn) {
      minMcn = x;
    }
    if (x > maxMcn) {
      maxMcn = x;
    }
    sumMcn += x;
  }

  if (numSimplices == 0) {
    *minContent = 0;
    *maxContent = 0;
    *meanContent = 0;
    *minDeterminant = 0;
    *maxDeterminant = 0;
    *meanDeterminant = 0;
    *minModMeanRatio = 0;
    *maxModMeanRatio = 0;
    *meanModMeanRatio = 0;
    *minModCondNum = 0;
    *maxModCondNum = 0;
    *meanModCondNum = 0;
  }
  else {
    *minContent = minCont;
    *maxContent = maxCont;
    *meanContent = sumCont / numSimplices;
    *minDeterminant = minDet;
    *maxDeterminant = maxDet;
    *meanDeterminant = sumDet / numSimplices;
    *minModMeanRatio = minMmr;
    *maxModMeanRatio = maxMmr;
    *meanModMeanRatio = sumMmr / numSimplices;
    *minModCondNum = minMcn;
    *maxModCondNum = maxMcn;
    *meanModCondNum = sumMcn / numSimplices;
  }
}


// Calculate quality statistics for the simplices in the mesh.
template<int N, int M, bool A, typename T, typename V, typename IS>
inline
void
computeQualityStatistics(const IndSimpSet<N,M,A,T,V,IS>& iss,
			 T* minContent, 
			 T* maxContent,
			 T* meanContent,
			 T* minDeterminant, 
			 T* maxDeterminant,
			 T* meanDeterminant,
			 T* minModMeanRatio,
			 T* maxModMeanRatio,
			 T* meanModMeanRatio,
			 T* minModCondNum, 
			 T* maxModCondNum,
			 T* meanModCondNum) {
  computeQualityStatistics<M>(iss.getVerticesBeginning(), 
			      iss.getIndexedSimplicesBeginning(), 
			      iss.getIndexedSimplicesEnd(),
			      minContent, 
			      maxContent,
			      meanContent,
			      minDeterminant, 
			      maxDeterminant,
			      meanDeterminant,
			      minModMeanRatio,
			      maxModMeanRatio,
			      meanModMeanRatio,
			      minModCondNum, 
			      maxModCondNum,
			      meanModCondNum);
}







// Print quality statistics for the simplices in the mesh.
template<int N, int M, typename T, typename VertRAIter, typename ISForwardIter>
inline
void
printQualityStatistics(std::ostream& out,
		       VertRAIter verticesBeginning, VertRAIter verticesEnd,
		       ISForwardIter indexedSimplicesBeginning, 
		       ISForwardIter indexedSimplicesEnd) {
  typedef typename std::iterator_traits<VertRAIter>::value_type Vertex;
  typedef Simplex<M,Vertex> Simplex;

  T minContent, maxContent, meanContent,
    minDeterminant, maxDeterminant, meanDeterminant,
    minModMeanRatio, maxModMeanRatio, meanModMeanRatio,
    minModCondNum, maxModCondNum, meanModCondNum;

  computeQualityStatistics<M>
    (verticesBeginning, indexedSimplicesBeginning, indexedSimplicesEnd,
     &minContent, &maxContent, &meanContent,
     &minDeterminant, &maxDeterminant, &meanDeterminant,
     &minModMeanRatio, &maxModMeanRatio, &meanModMeanRatio,
     &minModCondNum, &maxModCondNum, &meanModCondNum);

  const int numSimplices = std::distance(indexedSimplicesBeginning,
					  indexedSimplicesEnd);
  const T content = meanContent * numSimplices;

  // Count the number of simplices with a positive determinant.
  Simplex s;
  SimplexJac<M,T> simplexJacobian;
  int numSimplicesWithPositiveDeterminant = 0;
  // Loop over the simplices.
  for (ISForwardIter i = indexedSimplicesBeginning; i != indexedSimplicesEnd; 
       ++i) {
    s.build(verticesBeginning, *i);
    simplexJacobian.setFunction(s);
    if (simplexJacobian.getDeterminant() > 0.0) {
      ++numSimplicesWithPositiveDeterminant;
    }
  }

  // Compute a bounding box around the mesh.
  geom::BBox<N,T> boundingBox;
  boundingBox.bound(verticesBeginning, verticesEnd);

  // Compute the edge length statistics.
  T minimumLength, maximumLength;
  computeEdgeLengthStatistics<M>(verticesBeginning,
				 indexedSimplicesBeginning, 
				 indexedSimplicesEnd,
				 &minimumLength, &maximumLength);

  out << "Space dimension = " << N << "\n"
      << "Simplex dimension = " << M << "\n"
      << "Bounding box = " << boundingBox << "\n"
      << "Number of vertices = " 
      << int(std::distance(verticesBeginning, verticesEnd)) << "\n"
      << "Number of simplices = " << numSimplices << "\n"
      << "Number of simplices with positive volume = " 
      << numSimplicesWithPositiveDeterminant << "\n"
      << "content = " << content
      << " min = " << minContent
      << " max = " << maxContent
      << " mean = " << meanContent << "\n"
      << "determinant:"
      << " min = " << minDeterminant
      << " max = " << maxDeterminant
      << " mean = " << meanDeterminant << "\n"
      << "mod mean ratio:"
      << " min = " << minModMeanRatio
      << " max = " << maxModMeanRatio
      << " mean = " << meanModMeanRatio << "\n"
      << "mod cond num:"
      << " min = " << minModCondNum
      << " max = " << maxModCondNum
      << " mean = " << meanModCondNum << "\n"
      << "edge lengths:"
      << " min = " << minimumLength
      << " max = " << maximumLength << "\n";
}






// Print quality statistics for the simplices in the mesh.
template<int N, int M, typename T, typename SimplexForwardIterator>
inline
void
printQualityStatistics(std::ostream& out,
		       SimplexForwardIterator simplicesBeginning, 
		       SimplexForwardIterator simplicesEnd) {
  typedef typename std::iterator_traits<SimplexForwardIterator>::value_type 
    Value;
  typedef typename Loki::TypeTraits<Value>::UnqualifiedType Simplex;

  T minContent, maxContent, meanContent,
    minDeterminant, maxDeterminant, meanDeterminant,
    minModMeanRatio, maxModMeanRatio, meanModMeanRatio,
    minModCondNum, maxModCondNum, meanModCondNum;

  computeQualityStatistics<M>
    (simplicesBeginning, simplicesEnd,
     &minContent, &maxContent, &meanContent,
     &minDeterminant, &maxDeterminant, &meanDeterminant,
     &minModMeanRatio, &maxModMeanRatio, &meanModMeanRatio,
     &minModCondNum, &maxModCondNum, &meanModCondNum);

  const int numSimplices = std::distance(simplicesBeginning, simplicesEnd);
  const T content = meanContent * numSimplices;

  // Count the number of simplices with a positive determinant.
  // Compute a bounding box around the mesh.
  Simplex s;
  SimplexJac<M,T> simplexJacobian;
  int numSimplicesWithPositiveDeterminant = 0;
  geom::BBox<N,T> boundingBox;
  if (numSimplices != 0) {
    boundingBox.setLowerCorner((*simplicesBeginning)[0]);
    boundingBox.setUpperCorner((*simplicesBeginning)[0]);
  }
  // Loop over the simplices.
  for (SimplexForwardIterator i = simplicesBeginning; i != simplicesEnd; ++i) {
    simplexJacobian.setFunction(*i);
    // Check the sign of the determinant.
    if (simplexJacobian.getDeterminant() > 0.0) {
      ++numSimplicesWithPositiveDeterminant;
    }
    // Add the simplex vertices to the bounding box.
    for (int m = 0; m != M + 1; ++m) {
      boundingBox.add((*i)[m]);
    }
  }

  // Compute the edge length statistics.
  T minimumLength, maximumLength;
  computeEdgeLengthStatistics<M>(simplicesBeginning, simplicesEnd,
				 &minimumLength, &maximumLength);

  out << "Space dimension = " << N << "\n"
      << "Simplex dimension = " << M << "\n"
      << "Bounding box = " << boundingBox << "\n"
    // << "Number of vertices = " 
    // << int(std::distance(verticesBeginning, verticesEnd)) << "\n"
      << "Number of simplices = " << numSimplices << "\n"
      << "Number of simplices with positive volume = " 
      << numSimplicesWithPositiveDeterminant << "\n"
      << "content = " << content
      << " min = " << minContent
      << " max = " << maxContent
      << " mean = " << meanContent << "\n"
      << "determinant:"
      << " min = " << minDeterminant
      << " max = " << maxDeterminant
      << " mean = " << meanDeterminant << "\n"
      << "mod mean ratio:"
      << " min = " << minModMeanRatio
      << " max = " << maxModMeanRatio
      << " mean = " << meanModMeanRatio << "\n"
      << "mod cond num:"
      << " min = " << minModCondNum
      << " max = " << maxModCondNum
      << " mean = " << meanModCondNum << "\n"
      << "edge lengths:"
      << " min = " << minimumLength
      << " max = " << maximumLength << "\n";
}




// Print quality statistics for the simplices in the mesh.
template<int N, int M, bool A, typename T, typename V, typename IS>
inline
void
printQualityStatistics(std::ostream& out,
		       const IndSimpSet<N,M,A,T,V,IS>& iss) {
  printQualityStatistics<N,M,T>(out, iss.getVerticesBeginning(), 
				iss.getVerticesEnd(), 
				iss.getIndexedSimplicesBeginning(), 
				iss.getIndexedSimplicesEnd());
}






// Print information about the mesh.
template<int N, int M, bool A, typename T, typename V, typename IS>
inline
void
printInformation(std::ostream& out, const IndSimpSet<N,M,A,T,V,IS>& x) {
  // A bounding box for the mesh.
  BBox<N,T> box;
  box.bound(x.getVerticesBeginning(), x.getVerticesEnd());

  out << "Space dimension = " << N << "\n"
      << "Simplex dimension = " << M << "\n"
      << "Bounding box = " << box << "\n"
      << "Number of vertices = " << x.getVerticesSize() << "\n"
      << "Number of simplices = " << x.getSimplicesSize() << "\n";
}


// Print information about the mesh.
template<int N, int M, bool A, typename T, typename V, typename IS>
inline
void
printInformation(std::ostream& out,
		 const IndSimpSetIncAdj<N,M,A,T,V,IS>& x) {
  // Print information that does not depend on the incidences and adjacencies.
  printInformation(out, static_cast<const IndSimpSet<N,M,A,T,V,IS>&>(x));
  
  {
    ads::FixedArray<M+2,int> counts;
    countAdjacencies(x, &counts);
    out << "Adjacency counts = " << counts << "\n";
  }
  out << "Number of components = " << countComponents(x) << "\n";
}

END_NAMESPACE_GEOM

// End of file.
