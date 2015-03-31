// -*- C++ -*-

/*! 
  \file numerical/random/uniform/DiscreteUniformGeneratorMt19937.h
  \brief Uniform random deviates using the Mersenne Twister algorithm from Matsumoto and Nishimura.
*/

// License from Matsumoto and Nishimura.
/* 
   A C-program for MT19937, with initialization improved 2002/1/26.
   Coded by Takuji Nishimura and Makoto Matsumoto.

   Before using, initialize the state by using init_genrand(seed)  
   or init_by_array(init_key, key_length).

   Copyright (C) 1997 - 2002, Makoto Matsumoto and Takuji Nishimura,
   All rights reserved.                          
   Copyright (C) 2005, Mutsuo Saito,
   All rights reserved.                          

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:

     1. Redistributions of source code must retain the above copyright
        notice, this list of conditions and the following disclaimer.

     2. Redistributions in binary form must reproduce the above copyright
        notice, this list of conditions and the following disclaimer in the
        documentation and/or other materials provided with the distribution.

     3. The names of its contributors may not be used to endorse or promote 
        products derived from this software without specific prior written 
        permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


   Any feedback is very welcome.
   http://www.math.sci.hiroshima-u.ac.jp/~m-mat/MT/emt.html
   email: m-mat @ math.sci.hiroshima-u.ac.jp (remove space)
*/

#if !defined(__numerical_DiscreteUniformGeneratorMt19937_h__)
#define __numerical_DiscreteUniformGeneratorMt19937_h__

#include "../../defs.h"

// If we are debugging the whole numerical package.
#if defined(DEBUG_numerical) && !defined(DEBUG_DiscreteUniformGeneratorMt19937)
#define DEBUG_DiscreteUniformGeneratorMt19937
#endif

BEGIN_NAMESPACE_NUMERICAL


namespace mt19937mn {
  //! Period parameter.
  const int N = 624;
  //! Period parameter.
  const int M = 397;
  //! Constant vector a.
  const unsigned MATRIX_A = 0x9908b0dfUL;
  //! Most significant w-r bits.
  const unsigned UPPER_MASK = 0x80000000UL;
  //! Least significant r bits.
  const unsigned LOWER_MASK = 0x7fffffffUL;
}

//! Uniform random deviates using the Mersenne Twister algorithm from Matsumoto and Nishimura.
/*!
  \param T The number type.  By default it is double.

  For documentation go to the 
  \ref numerical_random_uniform "uniform deviates page".

  See http://www.math.sci.hiroshima-u.ac.jp/~m-mat/MT/emt.html
  for information on the Mersenne twister algorithm.
*/
class DiscreteUniformGeneratorMt19937 {
private:

  // the array for the state vector
  unsigned mt[mt19937mn::N]; 
  // mag01[x] = x * MATRIX_A  for x=0,1
  unsigned mag01[2];
  // mti==mt19937mn::N+1 means mt[mt19937mn::N] is not initialized
  int mti; 

public:

  //! The argument type.
  typedef void argument_type;
  //! The result type.
  typedef unsigned result_type;


  //! Construct and seed.
  /*! If no seed is specified, it is seeded with 0. */
  explicit
  DiscreteUniformGeneratorMt19937(const unsigned seedValue = 0) :
    mti(mt19937mn::N + 1) {
    mag01[0] = 0x0UL;
    mag01[1] = mt19937mn::MATRIX_A;
    seed(seedValue);
  }

  //! Copy constructor.
  DiscreteUniformGeneratorMt19937
  (const DiscreteUniformGeneratorMt19937& other) :
    mti(other.mti) {
    mag01[0] = 0x0UL;
    mag01[1] = mt19937mn::MATRIX_A;
    for (int i = 0; i != mt19937mn::N; ++i) {
      mt[i] = other.mt[i];
    }
  }

  //! Assignment operator.
  DiscreteUniformGeneratorMt19937&
  operator=(const DiscreteUniformGeneratorMt19937& other) {
    if (this != &other) {
      mti = other.mti;
      for (int i = 0; i != mt19937mn::N; ++i) {
	mt[i] = other.mt[i];
      }
    }
    return *this;
  }

  //! Destructor.
  ~DiscreteUniformGeneratorMt19937() 
  {}

  //! Seed this random number generator.
  void
  seed(const unsigned s);

  //! Generate a state vector.
  /*!
    \param s A seed.
    \param state The state vector of length 624.
    \return A new seed.
  */
  static
  unsigned
  generateState(const unsigned s, unsigned state[]);

  //! Set the state vector.
  void
  setState(const unsigned state[]) {
    for (int i = 0; i != mt19937mn::N; ++i) {
      mt[i] = state[i];
    }
  }

  //! Set the specified element of the state vector.
  void
  setState(const int i, const unsigned state) {
    mt[i] = state;
  }

  //! Get the specified element of the state vector.
  unsigned
  getState(const int i) const {
    return mt[i];
  }

  //! Get the state vector.
  void
  getState(unsigned state[]) const {
    for (int i = 0; i != mt19937mn::N; ++i) {
      state[i] = mt[i];
    }
  }

  //! Return a uniform random deviate.
  result_type
  operator()();
};


END_NAMESPACE_NUMERICAL

#define __numerical_random_DiscreteUniformGeneratorMt19937_ipp__
#include "DiscreteUniformGeneratorMt19937.ipp"
#undef __numerical_random_DiscreteUniformGeneratorMt19937_ipp__

#endif
