#include "matrix.h"
#include <immintrin.h>

#define ROUND_DOWN(a,n) (a - (a % n))

CMatrix CMatrix::operator * ( const CMatrix & right ) const
{
	if ( getWidth() != right.getHeight() )
		throw CSizeException ( getHeight(), getWidth(), right.getHeight(), right.getWidth(), '*' );

	CMatrix tmp ( getHeight(), right.getWidth(), true );

	for (uint64_t i = 0; i < getHeight(); ++i)
		for (uint64_t j = 0; j < right.getWidth(); ++j)
		{
			tmp.m_Matrix[i][j] = 0;
			uint64_t k = 0;
			for (; k < ROUND_DOWN(getWidth(),4); k+=4)
			{
				__m256d v1 = _mm256_loadu_pd(&m_Matrix[i+offrows][k+offcols]);
				__m256d v2 = _mm256_loadu_pd(&right.m_Matrix[k+right.offrows][j+right.offcols]);
				__m256d v3 = _mm256_mul_pd(v1,v2);
				tmp.m_Matrix[i][j] += ((double*)(&v3))[0] + ((double*)(&v3))[1] + ((double*)(&v3))[2] + ((double*)(&v3))[3]; 
			}
			for (; k < getWidth(); ++k)
			{
				tmp.m_Matrix[i][j] += m_Matrix[i+offrows][k+offcols] * right.m_Matrix[k+right.offrows][j+right.offcols];
			}
		}
	return tmp;	
}