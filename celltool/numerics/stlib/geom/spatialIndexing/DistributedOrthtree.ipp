// -*- C++ -*-

#if !defined(__geom_spatialIndexing_DistributedOrthtree_ipp__)
#error This file is an implementation detail of the class DistributedOrthtree.
#endif

BEGIN_NAMESPACE_GEOM

//---------------------------------------------------------------------------
// Constructors etc.
//---------------------------------------------------------------------------

// Make an empty tree.
template<class _Orthtree>
inline
DistributedOrthtree<_Orthtree>::
DistributedOrthtree(const MPI::Intracomm& communicator,
		    const Point& lowerCorner, const Point& extents,
		    Split split,
		    Merge merge,
		    Refine refine,
		    Coarsen coarsen,
		    Action action) :
  Base(lowerCorner, extents, split, merge, refine, coarsen, action),
  _communicator(communicator.Dup()),
  _delimiters(_communicator.Get_size(), Code(0)) {
}

//---------------------------------------------------------------------------
// Operations on all nodes.
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Accessors.
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// File I/O.
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Free functions.
//---------------------------------------------------------------------------

END_NAMESPACE_GEOM

// End of file.
