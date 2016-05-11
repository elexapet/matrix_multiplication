#include "matrix.h"
#include <omp.h>

CMatrix CMatrix::operator * ( const CMatrix & right ) const
{
	if ( getWidth() != right.getHeight() )
		throw CSizeException ( getHeight(), getWidth(), right.getHeight(), right.getWidth(), '*' );

	CMatrix product ( getHeight(), right.getWidth(), true );


	uint i, j, k;
	uint leftHeight = getHeight();
	uint leftWidth = getWidth();
	uint leftOffrows = offrows;
	uint leftOffcols = offcols;
	uint rightWidth = right.getWidth();
	uint rightOffrows = right.offrows;
	uint rightOffcols = right.offcols;
	double tmp_res;
	
	#pragma omp parallel for\
		collapse(2) schedule(static, 1)\
		default(shared) private(k, tmp_res)
	for (i = 0; i < leftHeight; ++i){
		for (j = 0; j < rightWidth; ++j)
		{
			tmp_res = 0;
			#pragma omp simd reduction(+:tmp_res)
			for (k = 0; k < leftWidth; ++k)
			{
				tmp_res += m_Matrix[i+leftOffrows][k+leftOffcols] * right.m_Matrix[k+rightOffrows][j+rightOffcols];
			}
			product.m_Matrix[i][j] = tmp_res;
		}
	}
	return product;	
}