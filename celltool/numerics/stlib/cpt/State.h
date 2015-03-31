// -*- C++ -*-

/*! 
  \file State.h
  \brief Class for controlling the state of the CPT.
*/

#if !defined(__cpt_State_h__)
#define __cpt_State_h__

// Local
#include "StateBase.h"

BEGIN_NAMESPACE_CPT

// Hold the state for a closest point transform.
template<int N, typename T = double>
class State;

END_NAMESPACE_CPT

#define __cpt_State1_ipp__
#include "State1.ipp"
#undef __cpt_State1_ipp__

#define __cpt_State2_ipp__
#include "State2.ipp"
#undef __cpt_State2_ipp__

#define __cpt_State3_ipp__
#include "State3.ipp"
#undef __cpt_State3_ipp__

#endif
