// -*- C++ -*-

#if !defined(__numerical_random_PoissonGeneratorRanlib_ipp__)
#error This file is an implementation detail of PoissonGeneratorRanlib.
#endif

BEGIN_NAMESPACE_NUMERICAL

template<typename T,
	 class Uniform,
	 template<typename, class> class Normal>
inline
typename PoissonGeneratorRanlib<T, Uniform, Normal>::result_type
PoissonGeneratorRanlib<T, Uniform, Normal>::
operator()(const argument_type mu) {
  static Number a0 = -0.5;
  static Number a1 = 0.3333333;
  static Number a2 = -0.2500068;
  static Number a3 = 0.2000118;
  static Number a4 = -0.1661269;
  static Number a5 = 0.1421878;
  static Number a6 = -0.1384794;
  static Number a7 = 0.125006;
  static Number muold = 0.0;
  static Number muprev = 0.0;
  static Number fact[10] = {
    1.0,1.0,2.0,6.0,24.0,120.0,720.0,5040.0,40320.0,362880.0
  };
  static int ignpoi,j,k,kflag,l,m;
  static Number b1,b2,c,c0,c1,c2,c3,d,del,difmuk,e,fk,fx,fy,g,omega,p,p0,px,py,q,s,
    t,u,v,x,xx,pp[35];

  if(mu == muprev) goto S10;
  if(mu < 10.0) goto S120;
  /*
    C A S E  A. (RECALCULATION OF S,D,L IF MU HAS CHANGED)
  */
  muprev = mu;
  s = sqrt(mu);
  d = 6.0*mu*mu;
  /*
    THE POISSON PROBABILITIES PK EXCEED THE DISCRETE NORMAL
    PROBABILITIES FK WHENEVER K >= M(MU). L=IFIX(MU-1.1484)
    IS AN UPPER BOUND TO M(MU) FOR ALL MU >= 10 .
  */
  l = (int) (mu-1.1484);
 S10:
  /*
    STEP N. NORMAL SAMPLE - SNORM(IR) FOR STANDARD NORMAL DEVIATE
  */
  g = mu+s*(*_normalGenerator)();
  if(g < 0.0) goto S20;
  ignpoi = (int) (g);
  /*
    STEP I. IMMEDIATE ACCEPTANCE IF IGNPOI IS LARGE ENOUGH
  */
  if(ignpoi >= l) return ignpoi;
  /*
    STEP S. SQUEEZE ACCEPTANCE - SUNIF(IR) FOR (0,1)-SAMPLE U
  */
  fk = (Number)ignpoi;
  difmuk = mu-fk;
  u = transformDiscreteDeviateToContinuousDeviateClosed<Number>
    ((*_normalGenerator->getDiscreteUniformGenerator())());
  if(d*u >= difmuk*difmuk*difmuk) return ignpoi;
 S20:
  /*
    STEP P. PREPARATIONS FOR STEPS Q AND H.
    (RECALCULATIONS OF PARAMETERS IF NECESSARY)
    .3989423=(2*PI)**(-.5)  .416667E-1=1./24.  .1428571=1./7.
    THE QUANTITIES B1, B2, C3, C2, C1, C0 ARE FOR THE HERMITE
    APPROXIMATIONS TO THE DISCRETE NORMAL PROBABILITIES FK.
    C=.1069/MU GUARANTEES MAJORIZATION BY THE 'HAT'-FUNCTION.
  */
  if(mu == muold) goto S30;
  muold = mu;
  omega = 0.3989423/s;
  b1 = 4.166667E-2/mu;
  b2 = 0.3*b1*b1;
  c3 = 0.1428571*b1*b2;
  c2 = b2-15.0*c3;
  c1 = b1-6.0*b2+45.0*c3;
  c0 = 1.0-b1+3.0*b2-15.0*c3;
  c = 0.1069/mu;
 S30:
  if(g < 0.0) goto S50;
  /*
    'SUBROUTINE' F IS CALLED (KFLAG=0 FOR CORRECT RETURN)
  */
  kflag = 0;
  goto S70;
 S40:
  /*
    STEP Q. QUOTIENT ACCEPTANCE (RARE CASE)
  */
  if(fy-u*fy <= py*std::exp(px-fx)) return ignpoi;
 S50:
  /*
    STEP E. EXPONENTIAL SAMPLE - SEXPO(IR) FOR STANDARD EXPONENTIAL
    DEVIATE E AND SAMPLE T FROM THE LAPLACE 'HAT'
    (IF T <= -.6744 THEN PK < FK FOR ALL MU >= 10.)
  */
  e = sexpo();
  u = transformDiscreteDeviateToContinuousDeviateClosed<Number>
    ((*_normalGenerator->getDiscreteUniformGenerator())());
  u += (u-1.0);
  t = 1.8+fsign(e,u);
  if(t <= -0.6744) goto S50;
  ignpoi = (int) (mu+s*t);
  fk = (Number)ignpoi;
  difmuk = mu-fk;
  /*
    'SUBROUTINE' F IS CALLED (KFLAG=1 FOR CORRECT RETURN)
  */
  kflag = 1;
  goto S70;
 S60:
  /*
    STEP H. HAT ACCEPTANCE (E IS REPEATED ON REJECTION)
  */
  if(c*std::abs(u) > py*std::exp(px+e)-fy*std::exp(fx+e)) goto S50;
  return ignpoi;
 S70:
  /*
    STEP F. 'SUBROUTINE' F. CALCULATION OF PX,PY,FX,FY.
    CASE IGNPOI .LT. 10 USES FACTORIALS FROM TABLE FACT
  */
  if(ignpoi >= 10) goto S80;
  px = -mu;
  py = pow(mu,Number(ignpoi))/ *(fact+ignpoi);
  goto S110;
 S80:
  /*
    CASE IGNPOI .GE. 10 USES POLYNOMIAL APPROXIMATION
    A0-A7 FOR ACCURACY WHEN ADVISABLE
    .8333333E-1=1./12.  .3989423=(2*PI)**(-.5)
  */
  del = 8.333333E-2/fk;
  del -= (4.8*del*del*del);
  v = difmuk/fk;
  if(std::abs(v) <= 0.25) goto S90;
  px = fk*std::log(1.0+v)-difmuk-del;
  goto S100;
 S90:
  px = fk*v*v*(((((((a7*v+a6)*v+a5)*v+a4)*v+a3)*v+a2)*v+a1)*v+a0)-del;
 S100:
  py = 0.3989423/sqrt(fk);
 S110:
  x = (0.5-difmuk)/s;
  xx = x*x;
  fx = -0.5*xx;
  fy = omega*(((c3*xx+c2)*xx+c1)*xx+c0);
  if(kflag <= 0) goto S40;
  goto S60;
 S120:
  /*
    C A S E  B. (START NEW TABLE AND CALCULATE P0 IF NECESSARY)
  */
  muprev = 0.0;
  if(mu == muold) goto S130;
  muold = mu;
  m = std::max(1, int(mu));
  l = 0;
  p = std::exp(-mu);
  q = p0 = p;
 S130:
  /*
    STEP U. UNIFORM SAMPLE FOR INVERSION METHOD
  */
  u = transformDiscreteDeviateToContinuousDeviateClosed<Number>
    ((*_normalGenerator->getDiscreteUniformGenerator())());
  ignpoi = 0;
  if(u <= p0) return ignpoi;
  /*
    STEP T. TABLE COMPARISON UNTIL THE END PP(L) OF THE
    PP-TABLE OF CUMULATIVE POISSON PROBABILITIES
    (0.458=PP(9) FOR MU=10)
  */
  if(l == 0) goto S150;
  j = 1;
  if(u > 0.458) j = std::min(l,m);
  for(k=j; k<=l; k++) {
    if(u <= *(pp+k-1)) goto S180;
  }
  if(l == 35) goto S130;
 S150:
  /*
    STEP C. CREATION OF NEW POISSON PROBABILITIES P
    AND THEIR CUMULATIVES Q=PP(K)
  */
  l += 1;
  for(k=l; k<=35; k++) {
    p = p*mu/Number(k);
    q += p;
    *(pp+k-1) = q;
    if(u <= q) goto S170;
  }
  l = 35;
  goto S130;
 S170:
  l = k;
 S180:
  ignpoi = k;
  return ignpoi;
}


// CONTINUE: Replace this with a standard exponential deviate.  This is 
// only accurate to single precision.
/*
**********************************************************************
                                                                      
                                                                      
     (STANDARD-)  E X P O N E N T I A L   DISTRIBUTION                
                                                                      
                                                                      
**********************************************************************
**********************************************************************
                                                                      
     FOR DETAILS SEE:                                                 
                                                                      
               AHRENS, J.H. AND DIETER, U.                            
               COMPUTER METHODS FOR SAMPLING FROM THE                 
               EXPONENTIAL AND NORMAL DISTRIBUTIONS.                  
               COMM. ACM, 15,10 (OCT. 1972), 873 - 882.               
                                                                      
     ALL STATEMENT NUMBERS CORRESPOND TO THE STEPS OF ALGORITHM       
     'SA' IN THE ABOVE PAPER (SLIGHTLY MODIFIED IMPLEMENTATION)       
                                                                      
     Modified by Barry W. Brown, Feb 3, 1988 to use RANF instead of   
     SUNIF.  The argument IR thus goes away.                          
                                                                      
**********************************************************************
     Q(N) = SUM(ALOG(2.0)**K/K!)    K=1,..,N ,      THE HIGHEST N
     (HERE 8) IS DETERMINED BY Q(N)=1.0 WITHIN STANDARD PRECISION
*/
template<typename T,
	 class Uniform,
	 template<typename, class> class Normal>
inline
typename PoissonGeneratorRanlib<T, Uniform, Normal>::Number
PoissonGeneratorRanlib<T, Uniform, Normal>::
sexpo() {
  static Number q[8] = {
    0.6931472,0.9333737,0.9888778,0.9984959,0.9998293,0.9999833,0.9999986,1.0
  };
  static int i;
  static Number sexpo,a,u,ustar,umin;
  static Number *q1 = q;
  a = 0.0;
  u = transformDiscreteDeviateToContinuousDeviateClosed<Number>
    ((*_normalGenerator->getDiscreteUniformGenerator())());
  goto S30;
 S20:
  a += *q1;
 S30:
  u += u;
  if(u <= 1.0) goto S20;
  u -= 1.0;
  if(u > *q1) goto S60;
  sexpo = a+u;
  return sexpo;
 S60:
  i = 1;
  ustar = transformDiscreteDeviateToContinuousDeviateClosed<Number>
    ((*_normalGenerator->getDiscreteUniformGenerator())());
  umin = ustar;
 S70:
  ustar = transformDiscreteDeviateToContinuousDeviateClosed<Number>
    ((*_normalGenerator->getDiscreteUniformGenerator())());
  if(ustar < umin) umin = ustar;
  i += 1;
  if(u > *(q+i-1)) goto S70;
  sexpo = a+umin**q1;
  return sexpo;
}


END_NAMESPACE_NUMERICAL

// End of file.
