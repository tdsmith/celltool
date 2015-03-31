// -*- C++ -*-

#include "performance.h"

BEGIN_NAMESPACE_CPT

#ifdef CPT_PERFORMANCE
namespace performance {
  
  int countFaceScanConverted = 0;
  int countEdgeScanConverted = 0;
  int countVertexScanConverted = 0;

  int countFaceDistancesComputed = 0;
  int countEdgeDistancesComputed = 0;
  int countVertexDistancesComputed = 0;

  int countFaceDistancesSet = 0;
  int countEdgeDistancesSet = 0;
  int countVertexDistancesSet = 0;

  double timeMakeFacePolyhedra = 0;
  double timeMakeEdgePolyhedra = 0;
  double timeMakeVertexPolyhedra = 0;

  double timeScanConvertFacePolyhedra = 0;
  double timeScanConvertEdgePolyhedra = 0;
  double timeScanConvertVertexPolyhedra = 0;

  double timeFaceCpt = 0;
  double timeEdgeCpt = 0;
  double timeVertexCpt = 0;

  void
  print(std::ostream& out) {
    double total;

    out << "countFaceScanConverted = " 
	<< countFaceScanConverted << "\n"
	<< "countEdgeScanConverted = " 
	<< countEdgeScanConverted << "\n"
	<< "countVertexScanConverted = " 
	<< countVertexScanConverted << "\n"
	<< "  Total scan converted = " 
	<< (countFaceScanConverted + 
	    countEdgeScanConverted + 
	    countVertexScanConverted) << "\n";


    out << "countFaceDistancesComputed = " 
	<< countFaceDistancesComputed << "\n"
	<< "countEdgeDistancesComputed = " 
	<< countEdgeDistancesComputed << "\n"
	<< "countVertexDistancesComputed = " 
	<< countVertexDistancesComputed << "\n"
	<< "  Total distances computed = " 
	<< (countFaceDistancesComputed +
	    countEdgeDistancesComputed +
	    countVertexDistancesComputed) << "\n";

    out << "countFaceDistancesSet = " 
	<< countFaceDistancesSet << "\n"
	<< "countEdgeDistancesSet = " 
	<< countEdgeDistancesSet << "\n"
	<< "countVertexDistancesSet = " 
	<< countVertexDistancesSet << "\n"
	<< "  Total distances set = " 
	<< (countFaceDistancesSet +
	    countEdgeDistancesSet +
	    countVertexDistancesSet) << "\n";

    total = timeMakeFacePolyhedra + timeMakeEdgePolyhedra +
      timeMakeVertexPolyhedra;
    if (total != 0) {
      out << "timeMakeFacePolyhedra = " 
	  << timeMakeFacePolyhedra << "\n"
	  << "timeMakeEdgePolyhedra = " 
	  << timeMakeEdgePolyhedra << "\n"
	  << "timeMakeVertexPolyhedra = " 
	  << timeMakeVertexPolyhedra << "\n"
	  << "  Total time to make the polyhedra = " 
	  << total << "\n";
    }

    total = timeScanConvertFacePolyhedra + 
      timeScanConvertEdgePolyhedra +
      timeScanConvertVertexPolyhedra;
    if (total != 0) {
      out << "timeScanConvertFacePolyhedra = " 
	  << timeScanConvertFacePolyhedra << "\n"
	  << "timeScanConvertEdgePolyhedra = " 
	  << timeScanConvertEdgePolyhedra << "\n"
	  << "timeScanConvertVertexPolyhedra = " 
	  << timeScanConvertVertexPolyhedra << "\n"
	  << "  Total time to scan convert the polyhedra = " 
	  << total << "\n";
    }

    out << "timeFaceCpt = " 
	<< timeFaceCpt << "\n"
	<< "timeEdgeCpt = " 
	<< timeEdgeCpt << "\n"
	<< "timeVertexCpt = " 
	<< timeVertexCpt << "\n"
	<< "  Total time to compute distances etc. = " 
	<< (timeFaceCpt +
	    timeEdgeCpt +
	    timeVertexCpt) << "\n";
  }

}
#endif

END_NAMESPACE_CPT
