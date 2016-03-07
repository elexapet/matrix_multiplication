#include "matrix.cpp"

#define STRASS_SIZE_LIMIT 16

//-----------------------------------------------------------------------//
//-----------------------------------------------------------------------//

CMatrix strassenMult(const CMatrix & left, const CMatrix & right)
{
	if (left.getWidth() != left.getHeight() || right.getHeight() != right.getWidth()){
		cout << "Matrices are not square.";
		throw CSizeException ( left.getHeight(), left.getWidth(), right.getHeight(), right.getWidth(), '*' );
	}
	//Use standard multiplication when matrices are small enough.
	if (left.getWidth() < STRASS_SIZE_LIMIT || right.getWidth() < STRASS_SIZE_LIMIT)
		return left*right;

	//padd matrices to 2^N x 2^N size
	//todo

	return strassenRec(left, right);
}

CMatrix strassenRec(const CMatrix & left, const CMatrix & right){
	//Use standard multiplication when matrices are small enough.
	if (left.getWidth() < STRASS_SIZE_LIMIT || right.getWidth() < STRASS_SIZE_LIMIT)
		return left*right;

	//result placeholder
	CMatrix result(left.getHeight(), left.getWidth(), true);

	uint64_t halfr = result.getHeight()/2;
	uint64_t halfc = result.getWidth()/2;
	uint64_t rows = result.getHeight();
	uint64_t cols = result.getWidth();

	//subviews of left & right
	CMatrix a11(left,0,0,halfr,halfc);
	CMatrix a12(left,0,halfc,halfr,cols);
	CMatrix a21(left,halfr,0,rows,halfc);
	CMatrix a22(left,halfr,halfc,rows,cols);
	CMatrix b11(right,0,0,halfr,halfc);
	CMatrix b12(right,0,halfc,halfr,cols);
	CMatrix b21(right,halfr,0,rows,halfc);
	CMatrix b22(right,halfr,halfc,rows,cols);

	//new submatrices m(i)
	CMatrix const & m1 = strassenRec(a11+a22,b11+b22);
	//cout << m1 << "\n";
	CMatrix const & m2 = strassenRec(a21+a22,b11);
	//cout << m2<< "\n";
	CMatrix const & m3 = strassenRec(a11,b12-b22);
	//cout << m3<< "\n";
	CMatrix const & m4 = strassenRec(a22,b21-b11);
	//cout << m4<< "\n";
	CMatrix const & m5 = strassenRec(a11+a12,b22);
	//cout << m5<< "\n";
	CMatrix const & m6 = strassenRec(a21-a11,b11+b12);
	//cout << m6<< "\n";
	CMatrix const & m7 = strassenRec(a12-a22,b21+b22);
	//cout << m7<< "\n";

	//express result with m(i) matrices
	CMatrix(result, 0,0, 		halfr,halfc) = m1 + m4 - m5 + m7;
	CMatrix(result, 0,halfc, 	halfr,cols) = m3 + m5;
	CMatrix(result, halfr,0, 	rows,halfc) = m2 + m4;
	CMatrix(result, halfr,halfc,rows,cols) = m1 - m2 + m3 + m6;

	return result;
}

//-----------------------------------------------------------------------//
//-----------------------------------------------------------------------//