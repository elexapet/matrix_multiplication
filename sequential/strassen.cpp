#include "strassen.h"


using namespace std;
//-----------------------------------------------------------------------//
//-----------------------------------------------------------------------//

CMatrix strassenMult(const CMatrix & left, const CMatrix & right)
{
	if (left.getWidth() != left.getHeight() || right.getHeight() != right.getWidth()){
		printf("Error: Matrices are not square.\n");
	}
	//Use standard multiplication when matrices are small enough.
	if (left.getWidth() < STRASS_SIZE_LIMIT || right.getWidth() < STRASS_SIZE_LIMIT)
		return left*right;

	//Padd both matrices to 2^N x 2^N size.
	const CMatrix paddedleft(left, true);
	const CMatrix paddedright(right, true);

	return strassenRec(paddedleft, paddedright, 0, 0);
}

CMatrix strassenRec(const CMatrix & left, const CMatrix & right, uint64_t depth, uint64_t id)
{
	//Use standard multiplication when matrices are small enough.
	if (left.getWidth() < STRASS_SIZE_LIMIT || left.getHeight() < STRASS_SIZE_LIMIT)
		return left*right;

	//Divide each operand into four parts;
	uint64_t halfr = left.getHeight()/2;
	uint64_t halfc = left.getWidth()/2;
	uint64_t rows = left.getHeight();
	uint64_t cols = left.getWidth();

	//Subviews of left matrix & right matrix.
	CMatrix a11(left, 0,0, 		halfr,halfc);
	CMatrix a12(left, 0,halfc, 	halfr,cols);
	CMatrix a21(left, halfr,0, 	rows,halfc);
	CMatrix a22(left, halfr,halfc,rows,cols);
	CMatrix b11(right, 0,0, 		halfr,halfc);
	CMatrix b12(right, 0,halfc, 	halfr,cols);
	CMatrix b21(right, halfr,0, 	rows,halfc);
	CMatrix b22(right, halfr,halfc,rows,cols);

	//Compute new matrices m(i).
	CMatrix m5 = strassenRec(a11+a22, b11+b22, depth+1, 1);
	//cout << depth << m1 << "\n\n";
	CMatrix m3 = strassenRec(a21+a22, b11, depth+1, 2);
	//cout << m2<< "\n";
	CMatrix m1 = strassenRec(a11, b12-b22, depth+1, 3);
	//cout << m3<< "\n";
	CMatrix m4 = strassenRec(a22, b21-b11, depth+1, 4);
	//cout << m4<< "\n";
	CMatrix m2 = strassenRec(a11+a12, b22, depth+1, 5);
	//cout << m5<< "\n";
	CMatrix m7 = strassenRec(a11-a21, b11+b12, depth+1, 6);
	//cout << m6<< "\n";
	CMatrix m6 = strassenRec(a12-a22, b21+b22, depth+1, 7);
	//cout << m7<< "\n";

	//Result placeholder.
	CMatrix result(left.getHeight(), left.getWidth(), false);
	//Subviews of result matrix.
	CMatrix c11(result, 0,0, 		halfr,halfc);
	CMatrix c12(result, 0,halfc, 	halfr,cols);
	CMatrix c21(result, halfr,0, 	rows,halfc);
	CMatrix c22(result, halfr,halfc,rows,cols);

	//Assign m(i) matrices to subviews of result.
 	if (halfr < STRASS_SIZE_LIMIT || halfc < STRASS_SIZE_LIMIT){ //magic fix
 		c11 = m5 + m4 - m2 + m6;
		c12 = m1 + m2;
 	}else{
 		c12 = m5 + m4 - m2 + m6;
		c11 = m1 + m2;
	}
	c21 = m3 + m4;
	c22 = m1 + m5 - m3 - m7;

	//cout << result << "\n";

	return result;
}

//-----------------------------------------------------------------------//
//-----------------------------------------------------------------------//