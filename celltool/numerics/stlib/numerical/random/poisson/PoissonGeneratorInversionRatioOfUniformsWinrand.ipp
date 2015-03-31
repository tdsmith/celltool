// -*- C++ -*-

#if !defined(__numerical_random_PoissonGeneratorInversionRatioOfUniformsWinrand_ipp__)
#error This file is an implementation detail of PoissonGeneratorInversionRatioOfUniformsWinrand.
#endif

BEGIN_NAMESPACE_NUMERICAL


template<typename T, class Uniform>
inline
typename PoissonGeneratorInversionRatioOfUniformsWinrand<T, Uniform>::result_type
PoissonGeneratorInversionRatioOfUniformsWinrand<T, Uniform>::
operator()(const argument_type my) {
#define       C0      9.18938533204672742e-01       /* constants for */
#define       C1      8.33333333333333333e-02       /* Stirling's    */
#define       C3     -2.77777777777777778e-03       /* approximation */
#define       C5      7.93650793650793651e-04       /* of ln(k!)     */
#define       C7     -5.95238095238095238e-04

  static int m, b;

  static Number logfak[30L] = {
    0.00000000000000000,   0.00000000000000000,   0.69314718055994531,
    1.79175946922805500,   3.17805383034794562,   4.78749174278204599,
    6.57925121201010100,   8.52516136106541430,  10.60460290274525023,
    12.80182748008146961,  15.10441257307551530,  17.50230784587388584,
    19.98721449566188615,  22.55216385312342289,  25.19122118273868150,
    27.89927138384089157,  30.67186010608067280,  33.50507345013688888,
    36.39544520803305358,  39.33988418719949404,  42.33561646075348503,
    45.38013889847690803,  48.47118135183522388,  51.60667556776437357,
    54.78472939811231919,  58.00360522298051994,  61.26170176100200198,
    64.55753862700633106,  67.88974313718153498,  71.25703896716800901
  };                  /* ln(k!) for k=0,1,2,...,29 */
  static Number my_prev = -1.0,p0,a,h,g,q;
  int k;
  Number u,x,lf,ry,ry2,yk,ym,fk,fm,r;

  if (my_prev != my)                                  /* Set-up */
    {
      my_prev = my;
      if (my<5.5)
	{
	  p0=std::exp(-my);                            /* Set-up for Inversion */
	  b = int(my+10.0*(std::sqrt(my)+1.0));   /* safety-bound */
	}
      else
	{
	  a = my + 0.5;                      /* Set-up for Ratio of Uniforms */
	  r = std::sqrt(a + a);
	  b = int(std::floor(a + 7.0 * (r + 1.5)));    /* safety-bound */
	  m = int(my);
	  g  = std::log(my);
	  ym = m;
	  yk = std::floor(a - r);
	  x = (a - yk - 1.0) / (a - yk);
	  if (my * x * x > yk + 1) yk++;
	  if (yk > 29)                      /* computing ln(k!) with    */
	    {                              /* Stirling's Approximation */
	      ry= 1.0/yk;
	      ry2=ry*ry;
	      fk = (yk + 0.5)*std::log(yk)-yk+C0+ry*(C1+ry2*(C3+ry2*(C5+ry2*C7)));
	    }
	  else
	    {
	      k = int(yk);                      /* ln(k!) out of table logfak */
	      fk = logfak[k];              /* applied for k<=29          */
	    }
	  if (ym > 29)                     /* ln(m!) analogous to ln(k!) */
	    {
	      ry= 1.0/ym;
	      ry2=ry*ry;
	      fm = (ym + 0.5)*std::log(ym)-ym+C0+ry*(C1+ry2*(C3+ry2*(C5+ry2*C7)));
	    }
	  else fm = logfak[m];
	  q = ym * g - fm;
	  h = (a - yk) * std::exp(0.5 * ((yk - ym) * g + fm - fk) + logfak[2L]);
	}
    }
  if (my<5.5)                              /* Inversion/Chop-down */
    {
      Number pk;

      pk=p0;
      k=0;
      u=transformDiscreteDeviateToContinuousDeviateClosed<Number>
	((*_discreteUniformGenerator)());
      while (u>pk)
	{
	  ++k;
	  if (k>b)
	    {
	      u=transformDiscreteDeviateToContinuousDeviateClosed<Number>
		((*_discreteUniformGenerator)());
	      k=0;
	      pk=p0;
	    }
	  else
	    {
	      u-=pk;
	      pk*=my/k;
	    }
	}
      return(k);
    }
  for(;;)                                 /* Ratio of Uniforms */
    {
      do
	{
	  u = transformDiscreteDeviateToContinuousDeviateOpen<Number>
	    ((*_discreteUniformGenerator)());
	  x = a + h * 
	    (transformDiscreteDeviateToContinuousDeviateClosed<Number>
	     ((*_discreteUniformGenerator)()) - 0.5) / u;
	}
      while ((x < 0) || (x >= b));      /* check, if x is valid candidate */
      k = int(x);
      yk = k;
      if (k > 29)
	{
	  ry=1.0/yk;                               /* lf - ln(f) */
	  lf = g * yk - (0.5 + yk) * std::log(yk) + yk - C0 - C1*ry - q; /* lower bound */
	  if (lf >= u * (4.0 - u) - 3.0) break;    /* quick acceptance */
	  ry2=ry*ry;
	  lf -=ry*ry2*(C3+ry2*(C5+ry2*C7));        /* more exact ln(f) */
	}
      else
	{
	  lf = yk * g - logfak[k] - q;            /* exact ln(f) using table */
	  if (lf >= u * (4.0 - u) - 3.0) break;   /* to compute ln(k!)       */
	}
      if (u * (u - lf) <= 1.0)               /* u*(u-lf)>1 - quick rejection */
	if (2.0 * std::log(u) <= lf) break;      /* final acceptance */
    }
  return k;
}


END_NAMESPACE_NUMERICAL

// End of file.
