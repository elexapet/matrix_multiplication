#include "matrix.h"

CMatrix CMatrix::operator * ( const CMatrix & right ) const
{
	if ( getWidth() != right.getHeight() )
		throw CSizeException ( getHeight(), getWidth(), right.getHeight(), right.getWidth(), '*' );

	CMatrix tmp ( getHeight(), right.getWidth(), false );
	
	for (uint64_t i = 0; i < getHeight(); ++i)
		for (uint64_t j = 0; j < right.getWidth(); ++j)
			for (uint64_t k = 0; k < getWidth(); ++k)
			{
				tmp.m_Matrix[i][j] += m_Matrix[i+offrows][k+offcols] * right.m_Matrix[k+right.offrows][j+right.offcols];
			}
	return tmp;	
}