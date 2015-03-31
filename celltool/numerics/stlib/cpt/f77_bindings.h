// -*- C++ -*-
//----------------------------------------------------------------------
//
//                         Michael Aivazis
//                (C) 2000-2004 All Rights Reserved
//
//----------------------------------------------------------------------

// This file takes care of the names that FORTRAN programs see.
// The build procedure defined NEEDS_F77_TRANSLATION for every platform
// that has a non-standard naming convention and then specifies what 
// translation to perform.

#if !defined(NEEDS_F77_TRANSLATION)


#define cptSetParameters3F cptSetParameters3F_
#define cptSetLattice3F cptSetLattice3F_
#define cptInsertGrid3F cptInsertGrid3F_
#define cptClearGrids3F cptClearGrids3F_
#define cptSetBRepWithNoClipping3F cptSetBRepWithNoClipping3F_
#define cptSetBRep3F cptSetBRep3F_
#define cptComputeClosestPointTransform3F cptComputeClosestPointTransform3F_
#define cptComputeClosestPointTransformUnsigned3F cptComputeClosestPointTransformUnsigned3F_
#define cptComputeClosestPointTransformUsingBBox3F cptComputeClosestPointTransformUsingBBox3F_
#define cptComputeClosestPointTransformUnsignedUsingBBox3F cptComputeClosestPointTransformUnsignedUsingBBox3F_
#define cptComputeClosestPointTransformUsingBruteForce3F cptComputeClosestPointTransformUsingBruteForce3F_
#define cptComputeClosestPointTransformUnsignedUsingBruteForce3F cptComputeClosestPointTransformUnsignedUsingBruteForce3F_
#define cptFloodFillAtBoundary3F cptFloodFillAtBoundary3F_
#define cptFloodFillDetermineSign3F cptFloodFillDetermineSign3F_
#define cptFloodFillUnsigned3F cptFloodFillUnsigned3F_
#define cptAreGridsValid3F cptAreGridsValid3F_
#define cptAreGridsValidUnsigned3F cptAreGridsValidUnsigned3F_
#define cptDisplayInformation3F cptDisplayInformation3F_

#define cptSetParameters2F cptSetParameters2F_
#define cptSetLattice2F cptSetLattice2F_
#define cptInsertGrid2F cptInsertGrid2F_
#define cptClearGrids2F cptClearGrids2F_
#define cptSetBRepWithNoClipping2F cptSetBRepWithNoClipping2F_
#define cptSetBRep2F cptSetBRep2F_
#define cptComputeClosestPointTransform2F cptComputeClosestPointTransform2F_
#define cptComputeClosestPointTransformUnsigned2F cptComputeClosestPointTransformUnsigned2F_
#define cptComputeClosestPointTransformUsingBBox2F cptComputeClosestPointTransformUsingBBox2F_
#define cptComputeClosestPointTransformUnsignedUsingBBox2F cptComputeClosestPointTransformUnsignedUsingBBox2F_
#define cptComputeClosestPointTransformUsingBruteForce2F cptComputeClosestPointTransformUsingBruteForce2F_
#define cptComputeClosestPointTransformUnsignedUsingBruteForce2F cptComputeClosestPointTransformUnsignedUsingBruteForce2F_
#define cptFloodFillAtBoundary2F cptFloodFillAtBoundary2F_
#define cptFloodFillDetermineSign2F cptFloodFillDetermineSign2F_
#define cptFloodFillUnsigned2F cptFloodFillUnsigned2F_
#define cptAreGridsValid2F cptAreGridsValid2F_
#define cptAreGridsValidUnsigned2F cptAreGridsValidUnsigned2F_
#define cptDisplayInformation2F cptDisplayInformation2F_


#else // NEEDSF77_TRANSLATION

#if defined(F77EXTERNS_NOTRAILINGBAR)

/* 
   This is how these symbols are defined here. No translation is necessary
*/

#elif defined(F77EXTERNS_LOWERCASE_TRAILINGBAR)


#define cptSetParameters3F         cptSetParameters3F_
#define cptInsertGrid3F                      cptInsertGrid3F_
#define cptClearGrids3F                   cptClearGrids3F_
#define cptSetBRepWithNoClipping3F        cptSetBRepWithNoClipping3F_
#define cptSetBRep3F               cptSetBRep3F_
#define cptComputeClosestPointTransform3F       cptComputeClosestPointTransform3F_
#define cptComputeClosestPointTransformUnsigned3F cptComputeClosestPointTransformUnsigned3F_
#define cptFloodFillAtBoundary3F         cptFloodFillAtBoundary3F_
#define cptFloodFillDetermineSign3F      cptFloodFillDetermineSign3F_
#define cptFloodFillUnsigned3F           cptFloodFillUnsigned3F_
#define cptAreGridsValid3F               cptAreGridsValid3F_
#define cptAreGridsValidUnsigned3F      cptAreGridsValidUnsigned3F_
#define cptDisplayInformation3F           cptDisplayInformation3F_

#define cptSetParameters2F         cptSetParameters2F_
#define cptInsertGrid2F                      cptInsertGrid2F_
#define cptClearGrids2F                   cptClearGrids2F_
#define cptSetBRepWithNoClipping2F        cptSetBRepWithNoClipping2F_
#define cptSetBRep2F               cptSetBRep2F_
#define cptComputeClosestPointTransform2F       cptComputeClosestPointTransform2F_
#define cptComputeClosestPointTransformUnsigned2F cptComputeClosestPointTransformUnsigned2F_
#define cptFloodFillAtBoundary2F           cptFloodFillAtBoundary2F_
#define cptFloodFillDetermineSign2F        cptFloodFillcptFloodFill2F2F_
#define cptFloodFillUnsigned2F           cptFloodFillUnsigned2F_
#define cptAreGridsValid2F               cptAreGridsValid2F_
#define cptAreGridsValidUnsigned2F      cptAreGridsValidUnsigned2F_
#define cptDisplayInformation2F           cptDisplayInformation2F_


#elif defined(F77EXTERNS_UPPERCASE_NOTRAILINGBAR)


#define cptSetParameters3F         CPTSETPARAMETERS3F
#define cptInsertGrid3F                      CPTINSERTGRID3F
#define cptClearGrids3F                   CPTCLEARGRIDS3F
#define cptSetBRepWithNoClipping3F        CPTSETBREPWITHNOCLIPPING3F
#define cptSetBRep3F               CPTSETBREP3F
#define cptComputeClosestPointTransform3F       CPTCLOSESTPOINTTRANSFORM3F
#define cptComputeClosestPointTransformUnsigned3F CPTCLOSESTPOINTTRANSFORMUNSIGNED3F
#define cptFloodFillAtBoundary3F          CPTFLOODFILLATBOUNDARY3F
#define cptFloodFillDetermineSign3F       CPTFLOODFILLDETERMINESIGN3F
#define cptFloodFillUnsigned3F           CPTFLOODFILLUNSIGNED3F
#define cptAreGridsValid3F               CPTAREGRIDSVALID3F
#define cptAreGridsValidUnsigned3F      CPTAREGRIDSVALIDUNSIGNED3F
#define cptDisplayInformation3F           CPTDISPLAYINFORMATION3F

#define cptSetParameters2F         CPTSETPARAMETERS2F
#define cptInsertGrid2F                      CPTINSERTGRID2F
#define cptClearGrids2F                   CPTCLEARGRIDS2F
#define cptSetBRepWithNoClipping2F        CPTSETBREPWITHNOCLIPPING2F
#define cptSetBRep2F               CPTSETBREP2F
#define cptComputeClosestPointTransform2F       CPTCLOSESTPOINTTRANSFORM2F
#define cptComputeClosestPointTransformUnsigned2F CPTCLOSESTPOINTTRANSFORMUNSIGNED2F
#define cptFloodFillAtBoundary2F          CPTFLOODFILLATBOUNDARY2F
#define cptFloodFillDetermineSign2F       CPTFLOODFILLDETERMINESIGN2F
#define cptFloodFillUnsigned2F           CPTFLOODFILLUNSIGNED2F
#define cptAreGridsValid2F               CPTAREGRIDSVALID2F
#define cptAreGridsValidUnsigned2F      CPTAREGRIDSVALIDUNSIGNED2F
#define cptDisplayInformation2F           CPTDISPLAYINFORMATION2F


#elif defined(F77EXTERNS_COMPAQF90)


#define cptSetParameters3F         cptSetParameters3F__
#define cptInsertGrid3F                      cptInsertGrid3F__
#define cptClearGrids3F                   cptClearGrids3F__
#define cptSetBRepWithNoClipping3F        cptSetBRepWithNoClipping3F__
#define cptSetBRep3F               cptSetBRep3F__
#define cptComputeClosestPointTransform3F       cptComputeClosestPointTransform3F__
#define cptComputeClosestPointTransformUnsigned3F cptComputeClosestPointTransformUnsigned3F__
#define cptFloodFillAtBoundary3F          cptFloodFillAtBoundary3F__
#define cptFloodFillDetermineSign3F       cptFloodFillDetermineSign3F__
#define cptFloodFillUnsigned3F           cptFloodFillUnsigned3F__
#define cptAreGridsValid3F               cptAreGridsValid3F__
#define cptAreGridsValidUnsigned3F      cptAreGridsValidUnsigned3F__
#define cptDisplayInformation3F           cptDisplayInformation3F__

#define cptSetParameters2F         cptSetParameters2F__
#define cptInsertGrid2F                      cptInsertGrid2F__
#define cptClearGrids2F                   cptClearGrids2F__
#define cptSetBRepWithNoClipping2F        cptSetBRepWithNoClipping2F__
#define cptSetBRep2F               cptSetBRep2F__
#define cptComputeClosestPointTransform2F       cptComputeClosestPointTransform2F__
#define cptComputeClosestPointTransformUnsigned2F cptComputeClosestPointTransformUnsigned2F__
#define cptFloodFillAtBoundary2F          cptFloodFillAtBoundary2F__
#define cptFloodFillDetermineSign2F       cptFloodFillDetermineSign2F__
#define cptFloodFillUnsigned2F           cptFloodFillUnsigned2F__
#define cptAreGridsValid2F               cptAreGridsValid2F__
#define cptAreGridsValidUnsigned2F      cptAreGridsValidUnsigned2F__
#define cptDisplayInformation2F           cptDisplayInformation2F__


#else

#error Uknown translation for FORTRAN external symbols

#endif

#endif // NEEDSF77_TRANSLATION
