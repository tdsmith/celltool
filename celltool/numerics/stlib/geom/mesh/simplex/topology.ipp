// -*- C++ -*-

#if !defined(__geom_mesh_simplex_topology_ipp__)
#error This file is an implementation detail.
#endif

BEGIN_NAMESPACE_GEOM

// Compute the other indices of the simplex.
inline
void
computeOtherIndices(int i, int j, int* a, int* b) {
  if (i > j) {
    std::swap(i, j);
  }
  assert(0 <= i && i <= 3 && 0 <= j && j <= 3 && i < j);
  *a = 0;
  if (*a == i) {
    ++*a;
  }
  if (*a == j) {
    ++*a;
  }
  *b = *a + 1;
  if (*b == i) {
    ++*b;
  }
  if (*b == j) {
    ++*b;
  }
  assert(*a != i && *a != j && *b != i && *b != j && *a < *b);
}


// Compute the other index of the simplex.
int
computeOtherIndex(int i, int j, int k) {
  assert(0 <= i && i <= 3 && 
	 0 <= j && j <= 3 &&
	 0 <= k && k <= 3 &&
	 i != j && i != k && j != k);
  if (i != 0 && j != 0 && k != 0) {
    return 0;
  }
  if (i != 1 && j != 1 && k != 1) {
    return 1;
  }
  if (i != 2 && j != 2 && k != 2) {
    return 2;
  }
  return 3;
}

END_NAMESPACE_GEOM

// End of file.
