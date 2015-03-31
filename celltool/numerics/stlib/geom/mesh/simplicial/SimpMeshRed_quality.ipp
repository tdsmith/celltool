// -*- C++ -*-

#if !defined(__geom_mesh_simplicial_SimpMeshRed_quality_ipp__)
#error This file is an implementation detail of the class SimpMeshRed.
#endif

//
// These member functions are pretty much copied from ISS_Quality.
//

BEGIN_NAMESPACE_GEOM

// Return the total content of the simplices in the mesh.
template < int N, int M, typename T,
	   template<class> class V,
	   template<class> class C,
	   template<class> class CR >
inline
typename SimpMeshRed<N,M,T,V,C,CR>::number_type
SimpMeshRed<N,M,T,V,C,CR>::
content() const
{
  simplex_type s;
  SimplexJac<N,number_type> sj;
  number_type c = 0;
  // Loop over the cells.
  for ( cell_const_iterator iter = cells_begin(); iter != cells_end(); 
	++iter ) {
    get_simplex( iter, s );
    sj.set_function( s );
    c += sj.content();
  }
  return c;
}

// Calculate content (hypervolume) statistics for the simplices in the mesh.
template < int N, int M, typename T,
	   template<class> class V,
	   template<class> class C,
	   template<class> class CR >
inline
void
SimpMeshRed<N,M,T,V,C,CR>::
content( number_type& min_content, 
	 number_type& max_content,
	 number_type& mean_content ) const
{
  const int num_simplices = cells_size();
  assert( num_simplices != 0 );
  number_type min_cont = std::numeric_limits<number_type>::max();
  number_type max_cont = -std::numeric_limits<number_type>::max();
  number_type sum_cont = 0;
  number_type x;

  simplex_type s;
  SimplexJac<N,number_type> sj;
  // Loop over the cells.
  for ( cell_const_iterator iter = cells_begin(); iter != cells_end(); 
	++iter ) {
    get_simplex( iter, s );
    sj.set_function( s );
    x = sj.content();
    if ( x < min_cont ) {
      min_cont = x;
    }
    if ( x > max_cont ) {
      max_cont = x;
    }
    sum_cont += x;
  }
  min_content = min_cont;
  max_content = max_cont;
  mean_content = sum_cont / num_simplices;
}


// Calculate determinant statistics for the simplices in the mesh.
template < int N, int M, typename T,
	   template<class> class V,
	   template<class> class C,
	   template<class> class CR >
inline
void
SimpMeshRed<N,M,T,V,C,CR>::
determinant( number_type& min_determinant, 
	     number_type& max_determinant,
	     number_type& mean_determinant ) const
{
  const int num_simplices = cells_size();
  assert( num_simplices != 0 );
  number_type min_det = std::numeric_limits<number_type>::max();
  number_type max_det = -std::numeric_limits<number_type>::max();
  number_type sum_det = 0;
  number_type x;

  simplex_type s;
  SimplexJac<N,number_type> sj;
  // Loop over the cells.
  for ( cell_const_iterator iter = cells_begin(); iter != cells_end(); 
	++iter ) {
    get_simplex( iter, s );
    sj.set_function( s );
    x = sj.determinant();
    if ( x < min_det ) {
      min_det = x;
    }
    if ( x > max_det ) {
      max_det = x;
    }
    sum_det += x;
  }
  min_determinant = min_det;
  max_determinant = max_det;
  mean_determinant = sum_det / num_simplices;
}


// Calculate modified mean ratio function statistics for the simplices 
// in the mesh.
template < int N, int M, typename T,
	   template<class> class V,
	   template<class> class C,
	   template<class> class CR >
inline
void
SimpMeshRed<N,M,T,V,C,CR>::
mod_mean_ratio( number_type& min_mod_mean_ratio, 
		number_type& max_mod_mean_ratio,
		number_type& mean_mod_mean_ratio ) const
{
  const int num_simplices = cells_size();
  assert( num_simplices != 0 );
  number_type min_mmr = std::numeric_limits<number_type>::max();
  number_type max_mmr = -std::numeric_limits<number_type>::max();
  number_type sum_mmr = 0;
  number_type x;

  simplex_type s;
  SimplexModMeanRatio<N,number_type> smmr;
  // Loop over the cells.
  for ( cell_const_iterator iter = cells_begin(); iter != cells_end(); 
	++iter ) {
    get_simplex( iter, s );
    smmr.set_function( s );
    x = smmr();
    if ( x < min_mmr ) {
      min_mmr = x;
    }
    if ( x > max_mmr ) {
      max_mmr = x;
    }
    sum_mmr += x;
  }
  min_mod_mean_ratio = min_mmr;
  max_mod_mean_ratio = max_mmr;
  mean_mod_mean_ratio = sum_mmr / num_simplices;
}


// Calculate modified condition number function statistics for the simplices 
// in the mesh.
template < int N, int M, typename T,
	   template<class> class V,
	   template<class> class C,
	   template<class> class CR >
inline
void
SimpMeshRed<N,M,T,V,C,CR>::
mod_cond_num( number_type& min_mod_cond_num, 
	      number_type& max_mod_cond_num,
	      number_type& mean_mod_cond_num ) const
{
  const int num_simplices = cells_size();
  assert( num_simplices != 0 );
  number_type min_mcn = std::numeric_limits<number_type>::max();
  number_type max_mcn = -std::numeric_limits<number_type>::max();
  number_type sum_mcn = 0;
  number_type x;

  simplex_type s;
  SimplexModCondNum<N,number_type> smcn;
  // Loop over the cells.
  for ( cell_const_iterator iter = cells_begin(); iter != cells_end(); 
	++iter ) {
    get_simplex( iter, s );
    smcn.set_function( s );
    x = smcn();
    if ( x < min_mcn ) {
      min_mcn = x;
    }
    if ( x > max_mcn ) {
      max_mcn = x;
    }
    sum_mcn += x;
  }
  min_mod_cond_num = min_mcn;
  max_mod_cond_num = max_mcn;
  mean_mod_cond_num = sum_mcn / num_simplices;
}

template < int N, int M, typename T,
	   template<class> class V,
	   template<class> class C,
	   template<class> class CR >
inline
void
SimpMeshRed<N,M,T,V,C,CR>::
quality( number_type& min_content, 
	 number_type& max_content,
	 number_type& mean_content,
	 number_type& min_determinant, 
	 number_type& max_determinant,
	 number_type& mean_determinant,
	 number_type& min_mod_mean_ratio,
	 number_type& max_mod_mean_ratio,
	 number_type& mean_mod_mean_ratio,
	 number_type& min_mod_cond_num, 
	 number_type& max_mod_cond_num,
	 number_type& mean_mod_cond_num ) const
{
  const int num_simplices = cells_size();
  assert( num_simplices != 0 );
  number_type min_cont = std::numeric_limits<number_type>::max();
  number_type max_cont = -std::numeric_limits<number_type>::max();
  number_type sum_cont = 0;
  number_type min_det = std::numeric_limits<number_type>::max();
  number_type max_det = -std::numeric_limits<number_type>::max();
  number_type sum_det = 0;
  number_type min_mmr = std::numeric_limits<number_type>::max();
  number_type max_mmr = -std::numeric_limits<number_type>::max();
  number_type sum_mmr = 0;
  number_type min_mcn = std::numeric_limits<number_type>::max();
  number_type max_mcn = -std::numeric_limits<number_type>::max();
  number_type sum_mcn = 0;
  number_type x;

  simplex_type s;
  SimplexModMeanRatio<N,number_type> smmr;
  SimplexModCondNum<N,number_type> smcn;
  // Loop over the cells.
  for ( cell_const_iterator iter = cells_begin(); iter != cells_end(); 
	++iter ) {
    get_simplex( iter, s );
    smmr.set_function( s );
    smcn.set_function( s );
    x = smmr.content();
    if ( x < min_cont ) {
      min_cont = x;
    }
    if ( x > max_cont ) {
      max_cont = x;
    }
    sum_cont += x;

    x = smmr.determinant();
    if ( x < min_det ) {
      min_det = x;
    }
    if ( x > max_det ) {
      max_det = x;
    }
    sum_det += x;

    x = smmr();
    if ( x < min_mmr ) {
      min_mmr = x;
    }
    if ( x > max_mmr ) {
      max_mmr = x;
    }
    sum_mmr += x;

    x = smcn();
    if ( x < min_mcn ) {
      min_mcn = x;
    }
    if ( x > max_mcn ) {
      max_mcn = x;
    }
    sum_mcn += x;
  }
  min_content = min_cont;
  max_content = max_cont;
  mean_content = sum_cont / num_simplices;
  min_determinant = min_det;
  max_determinant = max_det;
  mean_determinant = sum_det / num_simplices;
  min_mod_mean_ratio = min_mmr;
  max_mod_mean_ratio = max_mmr;
  mean_mod_mean_ratio = sum_mmr / num_simplices;
  min_mod_cond_num = min_mcn;
  max_mod_cond_num = max_mcn;
  mean_mod_cond_num = sum_mcn / num_simplices;
}

// Print quality statistics for the simplices in the mesh.
template < int N, int M, typename T,
	   template<class> class V,
	   template<class> class C,
	   template<class> class CR >
inline
void
SimpMeshRed<N,M,T,V,C,CR>::
print_quality( std::ostream& out ) const
{
  const int num_simplices = cells_size();
  assert( num_simplices != 0 );
  number_type content = 0;
  number_type min_content = std::numeric_limits<number_type>::max();
  number_type max_content = -std::numeric_limits<number_type>::max();
  number_type mean_content = 0;
  number_type min_determinant = std::numeric_limits<number_type>::max();
  number_type max_determinant = -std::numeric_limits<number_type>::max();
  number_type mean_determinant = 0;
  number_type min_mod_mean_ratio = std::numeric_limits<number_type>::max();
  number_type max_mod_mean_ratio = -std::numeric_limits<number_type>::max();
  number_type mean_mod_mean_ratio = 0;
  number_type l2_mod_mean_ratio = 0;
  number_type min_mod_cond_num = std::numeric_limits<number_type>::max();
  number_type max_mod_cond_num = -std::numeric_limits<number_type>::max();
  number_type mean_mod_cond_num = 0;
  number_type l2_mod_cond_num = 0;
  number_type x;
  int num_simplices_positive_determinant = 0;

  simplex_type s;
  SimplexModMeanRatio<N,number_type> smmr;
  SimplexModCondNum<N,number_type> smcn;
  // Loop over the cells.
  for ( cell_const_iterator iter = cells_begin(); iter != cells_end(); 
	++iter ) {
    get_simplex( iter, s );
    smmr.set_function( s );
    smcn.set_function( s );
    x = smmr.content();
    if ( x < min_content ) {
      min_content = x;
    }
    if ( x > max_content ) {
      max_content = x;
    }
    mean_content += x;

    x = smmr.determinant();
    if ( x < min_determinant ) {
      min_determinant = x;
    }
    if ( x > max_determinant ) {
      max_determinant = x;
    }
    mean_determinant += x;

    if ( x > 0.0 ) {
      ++num_simplices_positive_determinant;
    }

    x = 1.0 / smmr();
    if ( x < min_mod_mean_ratio ) {
      min_mod_mean_ratio = x;
    }
    if ( x > max_mod_mean_ratio ) {
      max_mod_mean_ratio = x;
    }
    mean_mod_mean_ratio += x;
    l2_mod_mean_ratio += x * x;

    x = 1.0 / smcn();
    if ( x < min_mod_cond_num ) {
      min_mod_cond_num = x;
    }
    if ( x > max_mod_cond_num ) {
      max_mod_cond_num = x;
    }
    mean_mod_cond_num += x;
    l2_mod_cond_num += x * x;
  }
  content = mean_content;
  mean_content /= num_simplices;
  mean_determinant /= num_simplices;
  mean_mod_mean_ratio /= num_simplices;
  mean_mod_cond_num /= num_simplices;
  l2_mod_mean_ratio = std::sqrt( l2_mod_mean_ratio / num_simplices );
  l2_mod_cond_num = std::sqrt( l2_mod_cond_num / num_simplices );

  out << "Space dimension = " << N << '\n'
      << "Bounding box = " << bbox() << '\n'
      << "Number of vertices = " << vertices_size() << '\n'
      << "Number of simplices = " << num_simplices << '\n'
      << "Number of simplices with positive volume = " 
      << num_simplices_positive_determinant << '\n'
      << "content = " << content
      << " min = " << min_content
      << " max = " << max_content
      << " mean = " << mean_content << '\n'
      << "determinant:"
      << " min = " << min_determinant
      << " max = " << max_determinant
      << " mean = " << mean_determinant << '\n'
      << "mod mean ratio:"
      << " min = " << min_mod_mean_ratio
      << " max = " << max_mod_mean_ratio
      << " mean = " << mean_mod_mean_ratio
      << " l2 = " << l2_mod_mean_ratio << '\n'
      << "mod cond num:"
      << " min = " << min_mod_cond_num
      << " max = " << max_mod_cond_num
      << " mean = " << mean_mod_cond_num
      << " l2 = " << l2_mod_cond_num << '\n';
}

END_NAMESPACE_GEOM

// End of file.
