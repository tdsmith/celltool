// -*- C++ -*-

#if !defined(__numerical_random_PoissonGeneratorAcceptanceComplementWinrand_ipp__)
#error This file is an implementation detail of PoissonGeneratorAcceptanceComplementWinrand.
#endif

BEGIN_NAMESPACE_NUMERICAL


template<typename T,
	 class Uniform,
	 template<typename, class> class Normal>
inline
typename PoissonGeneratorAcceptanceComplementWinrand<T, Uniform, Normal>::result_type
PoissonGeneratorAcceptanceComplementWinrand<T, Uniform, Normal>::
operator()(const argument_type my) {
  static Number my_prev = -1.0, my_alt = -1.0,
    a0 =-0.5000000002, a1 = 0.3333333343, a2 =-0.2499998565,
    a3 = 0.1999997049, a4 =-0.1666848753, a5 = 0.1428833286,
    a6 =-0.1241963125, a7 = 0.1101687109, a8 =-0.1142650302,
    a9 = 0.1055093006;
  static int fac[] = {
    1,1,2,6,24,120,720,5040,40320,362880
  };
  static int l;
  static Number s,d;
  Number t,g,my_k = 0;

  static Number omega,b1,b2,c0,c1,c2,c3,c;
  Number gx,gy,px,py,e,x,xx,delta,v;
  int sign;

  Number u = 0;
  int k = 0;

  /* CASE A: acceptance complement */
  if (my_prev != my)
    {
      my_prev = my;
      s = std::sqrt(my);
      d = 6.0 * my * my;
      l = int(my - 1.1484);
    }
  t = (*_normalGenerator)();        /* Step N. Normal sample */
  g = my + s * t;
  if (g >= 0.0)
    {
      k = int(g);
      if (k >= l) return(k);     /* Step I. Immediate acceptance */
      /* Step S. Squeeze acceptance */
      u = transformDiscreteDeviateToContinuousDeviateOpen<Number>
	((*_normalGenerator->getDiscreteUniformGenerator())());
      my_k = my - k;
      if (d * u >= my_k * my_k * my_k) return(k);
    }
  if (my_alt != my)    /* Step P. Preparations for steps Q and H */
    {
      my_alt = my;
      omega = 0.3989423 / s;
      b1 = 0.416666666667e-1 / my;
      b2 = 0.3 * b1 * b1;
      c3 = 0.1428571 * b1 * b2;
      c2 = b2 - 15.0 * c3;
      c1 = b1 - 6.0 * b2 + 45.0 * c3;
      c0 = 1.0 - b1 + 3.0 * b2 - 15.0 * c3;
      c = 0.1069 / my;
    }
  if (g >= 0.0)
    {
      /* FUNCTION F */
      if (k < 10)
	{
	  px = -my;
	  py = std::exp(k * std::log(my)) / fac[k];
	}
      else  /* k >= 10 */
	{
	  delta = 0.83333333333e-1 / k;
	  delta = delta - 4.8*delta*delta*delta*(1.0-1.0/(3.5*k*k));
	  v = my_k / Number(k);
	  if (std::abs(v) > 0.25)
	    {
	      px = k * std::log(1.0 + v) - my_k - delta;
	    }
	  else
	    {
	      px = k * v * v;
	      px *= ((((((((a9*v+a8)*v+a7)*v+a6)*v+a5)*v+
			a4)*v+a3)*v+a2)*v+a1)*v+a0;
	      px -= delta;
	    }
	  py = 0.3989422804 / std::sqrt(Number(k));
	}
      x = (0.5 - my_k) / s;
      xx = x * x;
      gx = -0.5 * xx;
      gy = omega * (((c3 * xx + c2) * xx + c1) * xx + c0);
      /* Step Q. Quotient acceptance */
      if (gy * (1.0 - u)  <= py * std::exp(px - gx)) return(k);
    }
  for(;;)
    {
      do
	{
	  /* Step E. Number exponential sample */
	  e = - std::log(transformDiscreteDeviateToContinuousDeviateOpen<Number>
			 ((*_normalGenerator->getDiscreteUniformGenerator())())); 
	  u = transformDiscreteDeviateToContinuousDeviateOpen<Number>
	    ((*_normalGenerator->getDiscreteUniformGenerator())());
	  u = u + u - 1.0;
	  sign = (u < 0.0)? -1 : 1;
	  t = 1.8 + e * sign;
	}
      while (t <= -0.6744);
      k = int(my + s * t);
      my_k = my-k;
      /* FUNCTION F */
      if (k < 10)
	{
	  px = -my;
	  py = std::exp(k * std::log(my)) / fac[k];
	}
      else  /* k >= 10 */
	{
	  delta = 0.83333333333e-1 / Number(k);
	  delta = delta - 4.8*delta*delta*delta*(1.0-1.0/(3.5*k*k));
	  v = my_k / Number(k);
	  if (std::abs(v) > 0.25)
	    {
	      px = k * std::log(1.0 + v) - my_k - delta;
	    }
	  else
	    {
	      px = k * v * v;
	      px *= ((((((((a9*v+a8)*v+a7)*v+a6)*v+a5)*v+
			a4)*v+a3)*v+a2)*v+a1)*v+a0;
	      px -= delta;
	    }
	  py = 0.3989422804 / std::sqrt(Number(k));
	}
      x = (0.5 - my_k) / s;
      xx = x * x;
      gx = -0.5 * xx;
      gy = omega * (((c3 * xx + c2) * xx + c1) * xx + c0);
      /* Step H. Hat acceptance */
      if (c * sign * u <= py * std::exp(px + e) - gy * std::exp(gx + e)) return(k);
    }
}


END_NAMESPACE_NUMERICAL

// End of file.
