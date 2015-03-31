// -*- C++ -*-

#if !defined(__numerical_random_PoissonGeneratorInversionRejectionPatchworkWinrand_ipp__)
#error This file is an implementation detail of PoissonGeneratorInversionRejectionPatchworkWinrand.
#endif

BEGIN_NAMESPACE_NUMERICAL


template<typename T, class Uniform>
inline
typename PoissonGeneratorInversionRejectionPatchworkWinrand<T, Uniform>::result_type
PoissonGeneratorInversionRejectionPatchworkWinrand<T, Uniform>::
operator()(const argument_type my) {
  static Number my_old = -1.0;
  //Number t,g,my_k;

  //Number gx,gy,px,py,e,x,xx,delta,v;
  //int sign;

  static Number p,q,p0,pp[36];
  static int ll,m;
  Number u;
  int k,i;

  if (my < 10.0)
    {   /* CASE B: Inversion- start new table and calculate p0 */
      if (my != my_old)
	{
	  my_old = my;
	  m = (my > 1.0)? my : 1.0;
	  ll = 0;
	  p0 = q = p = exp(-my);
	}
      for(;;)
	{
	  u = transformDiscreteDeviateToContinuousDeviateClosed<Number>
	    ((*_discreteUniformGenerator)()); /* Step U. Uniform sample */
	  k = 0;
	  if (u <= p0) return(k);
	  if (ll != 0)               /* Step T. Table comparison */
	    {
	      i = (u > 0.458)? std::min(ll,m) : 1;
	      for (k = i; k <=ll; k++) if (u <= pp[k]) return(k);
	      if (ll == 35) continue;
	    }
	  for (k = ll +1; k <= 35; k++)  /* Step C. Creation of new prob. */
	    {
	      p *= my/(Number)k;
	      q += p;
	      pp[k] = q;
	      if (u <= q)
		{
		  ll = k;
		  return(k);
		}
	    }
	  ll = 35;
	}
    }     /* end my < 10 */
  else    /* CASE A: acceptance complement */
    {
      static Number        my_last = -1.0;
      static int      m,  k2, k4, k1, k5;
      static Number        dl, dr, r1, r2, r4, r5, ll, lr, l_my, c_pm,
	f1, f2, f4, f5, p1, p2, p3, p4, p5, p6;
      int             Dk, X, Y;
      Number               Ds, U, V, W;

      if (my != my_last)
	{                               /* set-up           */
	  my_last = my;

	  /* approximate deviation of reflection points k2, k4 from my - 1/2      */
	  Ds = sqrt(my + 0.25);

	  /* mode m, reflection points k2 and k4, and points k1 and k5, which     */
	  /* delimit the centre region of h(x)                                    */
	  m  = (int) my;
	  k2 = (int) ceil(my - 0.5 - Ds);
	  k4 = (int)     (my - 0.5 + Ds);
	  k1 = k2 + k2 - m + 1;
	  k5 = k4 + k4 - m;

	  /* range width of the critical left and right centre region             */
	  dl = (Number) (k2 - k1);
	  dr = (Number) (k5 - k4);

	  /* recurrence constants r(k) = p(k)/p(k-1) at k = k1, k2, k4+1, k5+1    */
	  r1 = my / (Number) k1;
	  r2 = my / (Number) k2;
	  r4 = my / (Number)(k4 + 1);
	  r5 = my / (Number)(k5 + 1);

	  /* reciprocal values of the scale parameters of expon. tail envelopes   */
	  ll =  log(r1);                                   /* expon. tail left */
	  lr = -log(r5);                                   /* expon. tail right*/

	  /* Poisson constants, necessary for computing function values f(k)      */
	  l_my = log(my);
	  c_pm = m * l_my - flogfak(m);

	  /* function values f(k) = p(k)/p(m) at k = k2, k4, k1, k5               */
	  f2 = f(k2, l_my, c_pm);
	  f4 = f(k4, l_my, c_pm);
	  f1 = f(k1, l_my, c_pm);
	  f5 = f(k5, l_my, c_pm);

	  /* area of the two centre and the two exponential tail regions          */
	  /* area of the two immediate acceptance regions between k2, k4          */
	  p1 = f2 * (dl + 1.0);                            /* immed. left      */
	  p2 = f2 * dl         + p1;                       /* centre left      */
	  p3 = f4 * (dr + 1.0) + p2;                       /* immed. right     */
	  p4 = f4 * dr         + p3;                       /* centre right     */
	  p5 = f1 / ll         + p4;                       /* expon. tail left */
	  p6 = f5 / lr         + p5;                       /* expon. tail right*/
	}

      for (;;)
	{
	  /* generate uniform number U -- U(0, p6)                                */
	  /* case distinction corresponding to U                                  */
	  if ((U = transformDiscreteDeviateToContinuousDeviateOpen<Number>
	       ((*_discreteUniformGenerator)()) * p6) < p2)
	    {     /* centre left      */

	      /* immediate acceptance region R2 = [k2, m) *[0, f2),  X = k2, ... m -1 */
	      if ((V = U - p1) < 0.0)  return(k2 + (int)(U/f2));
	      /* immediate acceptance region R1 = [k1, k2)*[0, f1),  X = k1, ... k2-1 */
	      if ((W = V / dl) < f1 )  return(k1 + (int)(V/f1));

	      /* computation of candidate X < k2, and its counterpart Y > k2          */
	      /* either squeeze-acceptance of X or acceptance-rejection of Y          */
	      Dk = (int)(dl * transformDiscreteDeviateToContinuousDeviateOpen<Number>
			 ((*_discreteUniformGenerator)())) + 1;
	      if (W <= f2 - Dk * (f2 - f2/r2))
		{             /* quick accept of  */
		  return(k2 - Dk);                             /* X = k2 - Dk      */
		}
	      if ((V = f2 + f2 - W) < 1.0)
		{                 /* quick reject of Y*/
		  Y = k2 + Dk;
		  if (V <= f2 + Dk * (1.0 - f2)/(dl + 1.0))
		    {  /* quick accept of  */
		      return(Y);                                 /* Y = k2 + Dk      */
		    }
		  if (V <= f(Y, l_my, c_pm))  return(Y);       /* final accept of Y*/
		}
	      X = k2 - Dk;
	    }
	  else if (U < p4)
	    {                               /* centre right     */
	      /*  immediate acceptance region R3 = [m, k4+1)*[0, f4), X = m, ... k4    */
	      if ((V = U - p3) < 0.0)  return(k4 - (int)((U - p2)/f4));
	      /* immediate acceptance region R4 = [k4+1, k5+1)*[0, f5)                */
	      if ((W = V / dr) < f5 )  return(k5 - (int)(V/f5));

	      /* computation of candidate X > k4, and its counterpart Y < k4          */
	      /* either squeeze-acceptance of X or acceptance-rejection of Y          */
	      Dk = (int)(dr * transformDiscreteDeviateToContinuousDeviateOpen<Number>
			 ((*_discreteUniformGenerator)())) + 1;
	      if (W <= f4 - Dk * (f4 - f4*r4))
		{             /* quick accept of  */
		  return(k4 + Dk);                             /* X = k4 + Dk      */
		}
	      if ((V = f4 + f4 - W) < 1.0)
		{                 /* quick reject of Y*/
		  Y = k4 - Dk;
		  if (V <= f4 + Dk * (1.0 - f4)/ dr)
		    {         /* quick accept of  */
		      return(Y);                                 /* Y = k4 - Dk      */
		    }
		  if (V <= f(Y, l_my, c_pm))  return(Y);       /* final accept of Y*/
		}
	      X = k4 + Dk;
	    }
	  else
	    {
	      W = transformDiscreteDeviateToContinuousDeviateOpen<Number>
	       ((*_discreteUniformGenerator)());
	      if (U < p5)
		{                                  /* expon. tail left */
		  Dk = (int)(1.0 - log(W)/ll);
		  if ((X = k1 - Dk) < 0)  continue;           /* 0 <= X <= k1 - 1 */
		  W *= (U - p4) * ll;                          /* W -- U(0, h(x))  */
		  if (W <= f1 - Dk * (f1 - f1/r1))  return(X); /* quick accept of X*/
		}
	      else
		{                                         /* expon. tail right*/
		  Dk = (int)(1.0 - log(W)/lr);
		  X  = k5 + Dk;                                /* X >= k5 + 1      */
		  W *= (U - p5) * lr;                          /* W -- U(0, h(x))  */
		  if (W <= f5 - Dk * (f5 - f5*r5))  return(X); /* quick accept of X*/
		}
	    }

	  /* acceptance-rejection test of candidate X from the original area      */
	  /* test, whether  W <= f(k),    with  W = U*h(x)  and  U -- U(0, 1)     */
	  /* log f(X) = (X - m)*log(my) - log X! + log m!                         */
	  if (log(W) <= X * l_my - flogfak(X) - c_pm)  return(X);
	}
    } /* end of my >= 10 */
}


END_NAMESPACE_NUMERICAL

// End of file.
