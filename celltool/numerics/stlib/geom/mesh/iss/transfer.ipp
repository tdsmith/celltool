// -*- C++ -*-

#if !defined(__geom_mesh_iss_transfer_ipp__)
#error This file is an implementation detail.
#endif

BEGIN_NAMESPACE_GEOM

template<int N, int M, bool A, typename T, typename V, typename IS,
	 class PointArray, class IndexArray>
inline
void
transferIndices(const IndSimpSet<N,M,A,T,V,IS>& mesh,
		const PointArray& points, IndexArray* indices) {
  typedef IndSimpSet<N,M,A,T,V,IS> ISS;

  //
  // Sanity checks.
  //

  // The source mesh must not be trivial.
  assert(mesh.getVerticesSize() > 0);
  assert(mesh.getSimplicesSize() > 0);
  // Points and indices should be the same size.
  assert(points.size() == indices->size());

  //
  // Build the data structure for doing simplex queries.
  //

  ISS_SimplexQuery<ISS> meshQuery(mesh);

  // For each target vertex.
  for (int i = 0; i != points.size(); ++i) {
    // Find the relevant simplex in the source mesh.
    (*indices)[i] = meshQuery.computeMinimumDistanceIndex(points[i]);
  }
}



template<int N, int M, bool A, typename T, typename V, typename IS,
	 class SourceFieldArray, class PointArray, class TargetFieldArray>
inline
void
transfer(const IndSimpSet<N,M,A,T,V,IS>& mesh,
	 const SourceFieldArray& sourceFields,
	 const PointArray& points,
	 TargetFieldArray* targetFields) {
  typedef IndSimpSet<N,M,A,T,V,IS> ISS;
  typedef typename SourceFieldArray::value_type Field;

  //
  // Sanity checks.
  //

  // The source mesh must not be trivial.
  assert(mesh.getVerticesSize() > 0);
  assert(mesh.getSimplicesSize() > 0);
  // The fields are specified at the vertices.
  assert(mesh.getVerticesSize() == sourceFields.size());
  assert(points.size() == targetFields->size());

  // Build the data structure for doing simplex queries and interpolation.
  ISS_Interpolate<ISS,Field> meshInterp(mesh, sourceFields);

  // For each target vertex.
  for (int i = 0; i != points.size(); ++i) {
    (*targetFields)[i] = meshInterp(points[i]);
  }
}

END_NAMESPACE_GEOM

// End of file.
