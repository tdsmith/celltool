// -*- C++ -*-

#include "cpt_f.h"
#include "cpt_c.h"

//---------------------------------------------------------------------------
//                          Set the parameters.
//---------------------------------------------------------------------------

void 
cptSetParameters3F(double* domain, double *maximumDistance) {
  cptSetParameters3(domain, *maximumDistance);
}

void 
cptSetParameters2F(double* domain, 
			       double* maximumDistance,
			       int* localClipping,
			       int* globalClipping,
			       int* decimationFactor) {
  cptSetParameters2(domain, *maximumDistance, *localClipping,
			*globalClipping, *decimationFactor);
}

//---------------------------------------------------------------------------
//                       Set the lattice.
//---------------------------------------------------------------------------

void 
cptSetLattice3F(int* extents, double* domain) {
  cptSetLattice3(extents, domain);
}

void 
cptSetLattice2F(int* extents, double* domain) {
  cptSetLattice2(extents, domain);
}

//---------------------------------------------------------------------------
//                               Add a grid.
//---------------------------------------------------------------------------

void 
cptInsertGrid3F(int* indexLowerBounds, 
		  int* indexUpperBounds,
		  double* distance,
		  int* computeGradientDistance,
		  double* gradientDistance,
		  int* computeClosestPoint,
		  double* closestPoint,
		  int* computeClosestFace,
		  int* closestFace) {
  //
  // If the gradient of the distance, the closest point or closest face 
  // is not being computed, pass 0 for those arrays.
  //
  if (*computeGradientDistance == 0) {
    gradientDistance = 0;
  }
  if (*computeClosestPoint == 0) {
    closestPoint = 0;
  }
  if (*computeClosestFace == 0) {
    closestFace = 0;
  }

  // Call the C function.
  cptInsertGrid3(indexLowerBounds, indexUpperBounds, distance, 
		  gradientDistance, closestPoint, closestFace);
}

void 
cptInsertGrid2F(int* indexLowerBounds, 
		  int* indexUpperBounds,
		  double* distance,
		  int* computeGradientDistance,
		  double* gradientDistance,
		  int* computeClosestPoint,
		  double* closestPoint,
		  int* computeClosestFace,
		  int* closestFace) {
  //
  // If the gradient of the distance, the closest point or closest face 
  // is not being computed, pass 0 for those arrays.
  //
  if (*computeGradientDistance == 0) {
    gradientDistance = 0;
  }
  if (*computeClosestPoint == 0) {
    closestPoint = 0;
  }
  if (*computeClosestFace == 0) {
    closestFace = 0;
  }

  // Call the C function.
  cptInsertGrid2(indexLowerBounds, indexUpperBounds, distance, 
		  gradientDistance, closestPoint, closestFace);
}

//---------------------------------------------------------------------------
//                            Clear the grids.
//---------------------------------------------------------------------------

void 
cptClearGrids3F() {
  // Call the C function.
  cptClearGrids3();
}

void 
cptClearGrids2F() {
  // Call the C function.
  cptClearGrids2();
}

//---------------------------------------------------------------------------
//                             Set the b-rep.
//---------------------------------------------------------------------------

void 
cptSetBRepWithNoClipping3F(int *verticesSize,
				const double* vertices,
				int *facesSize,
				const int* faces) {
  cptSetBRepWithNoClipping3(*verticesSize, vertices, *facesSize, faces);
}

void 
cptSetBRepWithNoClipping2F(int *verticesSize,
				const double* vertices,
				int *facesSize,
				const int* faces) {
  cptSetBRepWithNoClipping2(*verticesSize, vertices, *facesSize, faces);
}

//---------------------------------------------------------------------------
//                       Initialize the b-rep.
//    Clip the mesh to use only points that affect the cartesian domain.
//---------------------------------------------------------------------------

void 
cptSetBRep3F(int *verticesSize,
			 const double* vertices,
			 int *facesSize,
			 const int* faces) {
  cptSetBRep3(*verticesSize, vertices, *facesSize, faces);
}

void 
cptSetBRep2F(int *verticesSize,
			 const double* vertices,
			 int *facesSize,
			 const int* faces) {
  cptSetBRep2(*verticesSize, vertices, *facesSize, faces);
}

//---------------------------------------------------------------------------
//                      Closest Point Transform.
//---------------------------------------------------------------------------

void 
cptComputeClosestPointTransform3F() {
  cptComputeClosestPointTransform3();
}

void 
cptComputeClosestPointTransformUnsigned3F() {
  cptComputeClosestPointTransformUnsigned3();
}

void 
cptComputeClosestPointTransformUsingBBox3F() {
  cptComputeClosestPointTransformUsingBBox3();
}

void 
cptComputeClosestPointTransformUnsignedUsingBBox3F() {
  cptComputeClosestPointTransformUnsignedUsingBBox3();
}


void 
cptComputeClosestPointTransformUsingBruteForce3F() {
  cptComputeClosestPointTransformUsingBruteForce3();
}

void 
cptComputeClosestPointTransformUnsignedUsingBruteForce3F() {
  cptComputeClosestPointTransformUnsignedUsingBruteForce3();
}


void 
cptComputeClosestPointTransform2F() {
  cptComputeClosestPointTransform2();
}

void 
cptComputeClosestPointTransformUnsigned2F() {
  cptComputeClosestPointTransformUnsigned2();
}


void 
cptComputeClosestPointTransformUsingBBox2F() {
  cptComputeClosestPointTransformUsingBBox2();
}

void 
cptComputeClosestPointTransformUnsignedUsingBBox2F() {
  cptComputeClosestPointTransformUnsignedUsingBBox2();
}


void 
cptComputeClosestPointTransformUsingBruteForce2F() {
  cptComputeClosestPointTransformUsingBruteForce2();
}

void 
cptComputeClosestPointTransformUnsignedUsingBruteForce2F() {
  cptComputeClosestPointTransformUnsignedUsingBruteForce2();
}


//---------------------------------------------------------------------------
//    Flood fill the distance.
//---------------------------------------------------------------------------

void 
cptFloodFillAtBoundary3F(double* farAway) {
  return cptFloodFillAtBoundary3(*farAway);
}

void 
cptFloodFillDetermineSign3F(double* farAway) {
  return cptFloodFillDetermineSign3(*farAway);
}

void 
cptFloodFillUnsigned3F(double* farAway) {
  return cptFloodFillUnsigned3(*farAway);
}

void 
cptFloodFillAtBoundary2F(double* farAway) {
  return cptFloodFillAtBoundary2(*farAway);
}

void 
cptFloodFillDetermineSign2F(double* farAway) {
  return cptFloodFillDetermineSign2(*farAway);
}

void 
cptFloodFillUnsigned2F(double* farAway) {
  return cptFloodFillUnsigned2(*farAway);
}

//---------------------------------------------------------------------------
//                   Check if the grids are valid.
//---------------------------------------------------------------------------

int 
cptAreGridsValid3F() {
  return cptAreGridsValid3();
}

int 
cptAreGridsValidUnsigned3F() {
  return cptAreGridsValidUnsigned3();
}

int 
cptAreGridsValid2F() {
  return cptAreGridsValid2();
}

int 
cptAreGridsValidUnsigned2F() {
  return cptAreGridsValidUnsigned2();
}

//---------------------------------------------------------------------------
//        Write information about the state of the variables.
//---------------------------------------------------------------------------

void 
cptDisplayInformation3F() {
  cptDisplayInformation3();
}

void 
cptDisplayInformation2F() {
  cptDisplayInformation2();
}
