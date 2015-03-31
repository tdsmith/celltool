// -*- C++ -*-

#if !defined(__FunctionWithQuadraticPenalty_ipp__)
#error This file is an implementation detail of the class FunctionWithQuadraticPenalty.
#endif

BEGIN_NAMESPACE_NUMERICAL

//
// Constructors
//

template <int N, class F, class C, typename T, typename P>
inline
FunctionWithQuadraticPenalty<N,F,C,T,P>::
FunctionWithQuadraticPenalty(const function_type& function,
			     const constraint_type& constraint,
			     const number_type penalty_parameter,
			     const number_type reduction_factor) :
  base_type(),
  _function(function),
  _constraint(constraint),
  _penalty_parameter(penalty_parameter),
  _reduction_factor(reduction_factor) {
  assert(_penalty_parameter > 0);
  assert(0 < _reduction_factor && _reduction_factor < 1);
  }

template <int N, class F, class C, typename T, typename P>
inline
FunctionWithQuadraticPenalty<N,F,C,T,P>::
FunctionWithQuadraticPenalty(const  FunctionWithQuadraticPenalty& x) :
  _function(x._function),
  _constraint(x._constraint),
  _penalty_parameter(x._penalty_parameter),
  _reduction_factor(x._reduction_factor)
{}

//
// Functor.
//


template <int N, class F, class C, typename T, typename P>
inline
typename FunctionWithQuadraticPenalty<N,F,C,T,P>::result_type
FunctionWithQuadraticPenalty<N,F,C,T,P>::
operator()(const argument_type& x) const {
  const number_type c = _constraint(x);
  return _function(x) + 0.5 / _penalty_parameter * c * c;
}

template <int N, class F, class C, typename T, typename P>
inline
void
FunctionWithQuadraticPenalty<N,F,C,T,P>::
gradient(const argument_type& x, argument_type& gradient) const {
  // The gradient of the objective function.
  _function.gradient(x, gradient);
  // Add the constraint.
  const number_type c = _constraint(x);
  argument_type gc;
  _constraint.gradient(x, gc);
  gc *= c / _penalty_parameter;
  gradient += gc;
}

END_NAMESPACE_NUMERICAL

// End of file.
