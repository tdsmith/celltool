// -*- C++ -*-

#include "cpt_c.h"
#include "cpt.h"

#include <iostream>

static cpt::State<3,double> state3;
static cpt::State<2,double> state2;

//---------------------------------------------------------------------------
//                       Initialize the parameters.
//---------------------------------------------------------------------------

void 
cptSetParameters3(const double* domain, const double maximumDistance) {
  state3.setParameters(domain, maximumDistance);
}

void 
cptSetParameters2(const double* domain, 
		  const double maximumDistance,
		  const bool localClipping,
		  const int globalClipping,
		  const int decimationFactor) {
  state2.setParameters(domain, maximumDistance, localClipping, 
		       globalClipping, decimationFactor);
}

//---------------------------------------------------------------------------
//                       Set the lattice.
//---------------------------------------------------------------------------

void 
cptSetLattice3(const int* extents, const double* domain) {
  state3.setLattice(extents, domain);
}

void 
cptSetLattice2(const int* extents, const double* domain) {
  state2.setLattice(extents, domain);
}


//---------------------------------------------------------------------------
//                             Add grids.
//---------------------------------------------------------------------------

void 
cptInsertGrid3(const int* indexLowerBounds, 
	       const int* indexUpperBounds,
	       double* distance,
	       double* gradientOfDistance,
	       double* closestPoint,
	       int* closestFace) {
  state3.insertGrid(indexLowerBounds, indexUpperBounds, distance, 
		    gradientOfDistance, closestPoint, closestFace);
}

void 
cptInsertGrid2(const int* indexLowerBounds, 
	       const int* indexUpperBounds,
	       double* distance,
	       double* gradientOfDistance,
	       double* closestPoint,
	       int* closestFace) {
  state2.insertGrid(indexLowerBounds, indexUpperBounds, distance, 
		    gradientOfDistance, closestPoint, closestFace);
}

//---------------------------------------------------------------------------
//                          Clear the grids.
//---------------------------------------------------------------------------

void 
cptClearGrids3() {
  state3.clearGrids();
}

void 
cptClearGrids2() {
  state2.clearGrids();
}

//---------------------------------------------------------------------------
//                       Initialize the b-rep.
//---------------------------------------------------------------------------

void 
cptSetBRepWithNoClipping3(const int verticesSize,
			  const double* vertices,
			  const int facesSize,
			  const int* faces) {
  state3.setBRepWithNoClipping(verticesSize, vertices, facesSize, faces);
}

void 
cptSetBRepWithNoClipping2(const int verticesSize,
			  const double* vertices,
			  const int facesSize,
			  const int* faces) {
  state2.setBRepWithNoClipping(verticesSize, vertices, facesSize, faces);
}

//---------------------------------------------------------------------------
//                       Initialize the b-rep.
//    Clip the mesh to use only points that affect the cartesian domain.
//---------------------------------------------------------------------------

void 
cptSetBRep3(const int verticesSize,
	    const double* vertices,
	    const int facesSize,
	    const int* faces) {
  state3.setBRep(verticesSize, vertices, facesSize, faces);
}

void 
cptSetBRep2(const int verticesSize,
	    const double* vertices,
	    const int facesSize,
	    const int* faces) {
  state2.setBRep(verticesSize, vertices, facesSize, faces);
}

//---------------------------------------------------------------------------
//                      Closest point_type Transform.
//---------------------------------------------------------------------------

void 
cptComputeClosestPointTransform3() {
  state3.computeClosestPointTransform();
}

void 
cptComputeClosestPointTransformUnsigned3() {
  state3.computeClosestPointTransformUnsigned();
}

void 
cptComputeClosestPointTransformUsingBBox3() {
  state3.computeClosestPointTransformUsingBBox();
}

void 
cptComputeClosestPointTransformUnsignedUsingBBox3() {
  state3.computeClosestPointTransformUnsignedUsingBBox();
}


void 
cptComputeClosestPointTransformUsingBruteForce3() {
  state3.computeClosestPointTransformUsingBruteForce();
}

void 
cptComputeClosestPointTransformUnsignedUsingBruteForce3() {
  state3.computeClosestPointTransformUnsignedUsingBruteForce();
}

void 
cptComputeClosestPointTransform2() {
  state2.computeClosestPointTransform();
}

void 
cptComputeClosestPointTransformUnsigned2() {
  state2.computeClosestPointTransformUnsigned();
}


void 
cptComputeClosestPointTransformUsingBBox2() {
  state2.computeClosestPointTransformUsingBBox();
}

void 
cptComputeClosestPointTransformUnsignedUsingBBox2() {
  state2.computeClosestPointTransformUnsignedUsingBBox();
}


void 
cptComputeClosestPointTransformUsingBruteForce2() {
  state2.computeClosestPointTransformUsingBruteForce();
}

void 
cptComputeClosestPointTransformUnsignedUsingBruteForce2() {
  state2.computeClosestPointTransformUnsignedUsingBruteForce();
}


//---------------------------------------------------------------------------
//    Flood fill the distance.
//---------------------------------------------------------------------------

void 
cptFloodFillAtBoundary3(const double farAway) {
  return state3.floodFillAtBoundary(farAway);
}

void 
cptFloodFillDetermineSign3(const double farAway) {
  return state3.floodFillDetermineSign(farAway);
}

void 
cptFloodFillUnsigned3(const double farAway) {
  return state3.floodFillUnsigned(farAway);
}

void 
cptFloodFillAtBoundary2(const double farAway) {
  return state2.floodFillAtBoundary(farAway);
}

void 
cptFloodFillDetermineSign2(const double farAway) {
  return state2.floodFillDetermineSign(farAway);
}

void 
cptFloodFillUnsigned2(const double farAway) {
  return state2.floodFillUnsigned(farAway);
}

//---------------------------------------------------------------------------
//                   Check if the grids are valid.
//---------------------------------------------------------------------------

int 
cptAreGridsValid3() {
  return state3.areGridsValid();
}

int 
cptAreGridsValidUnsigned3() {
  return state3.areGridsValidUnsigned();
}

int 
cptAreGridsValid2() {
  return state2.areGridsValid();
}

int 
cptAreGridsValidUnsigned2() {
  return state2.areGridsValidUnsigned();
}

//---------------------------------------------------------------------------
//        Write information about the state of the variables.
//---------------------------------------------------------------------------

void 
cptDisplayInformation3() {
  return state3.displayInformation(std::cout);
}

void 
cptDisplayInformation2() {
  return state2.displayInformation(std::cout);
}
