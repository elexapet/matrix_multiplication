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
	if (left.getWidth() < STRASS_LIMIT || right.getWidth() < STRASS_LIMIT)
		return left*right;

	//Padd both matrices to 2^N x 2^N size.
	const CMatrix paddedleft(left, true);
	const CMatrix paddedright(right, true);

	return strassenRec(paddedleft, paddedright);
}

CMatrix strassenRec(const CMatrix & left, const CMatrix & right)
{
	//Use standard multiplication when matrices are small enough.
	if (left.getWidth() < STRASS_LIMIT || left.getHeight() < STRASS_LIMIT)
		return left*right;

	//Divide each operand into four parts;
	uint halfr = left.getHeight()/2;
	uint halfc = left.getWidth()/2;
	uint rows = left.getHeight();
	uint cols = left.getWidth();

	//Subviews of left matrix & right matrix.
	CMatrix a11(left, 0,0, 		halfr,halfc);
	CMatrix a12(left, 0,halfc, 	halfr,cols);
	CMatrix a21(left, halfr,0, 	rows,halfc);
	CMatrix a22(left, halfr,halfc,rows,cols);
	CMatrix b11(right, 0,0, 		halfr,halfc);
	CMatrix b12(right, 0,halfc, 	halfr,cols);
	CMatrix b21(right, halfr,0, 	rows,halfc);
	CMatrix b22(right, halfr,halfc,rows,cols);

	CMatrix m1,m2,m3,m4,m5,m6,m7;
	CMatrix a11a22(a11+a22);
	CMatrix b11b22(b11+b22);
	CMatrix a21a22(a21+a22);
	CMatrix b12b22(b12-b22);
	CMatrix b21b11(b21-b11);
	CMatrix a11a12(a11+a12);
	CMatrix a11a21(a11-a21);
	CMatrix b11b12(b11+b12);

	//Compute new matrices m(i).
	#pragma omp parallel default(shared)
	{
		#pragma omp task shared(m5) /*firstprivate(a11a22,b11b22)*/ if(rows > STRASS_PAR_LIMIT)
		m5 = strassenRec(a11a22, b11b22);

		#pragma omp task shared(m3) /*firstprivate(a21a22,b11)*/ if(rows > STRASS_PAR_LIMIT)
		m3 = strassenRec(a21a22, b11);
		
		#pragma omp task shared(m1) /*firstprivate(a11,b12b22)*/ if(rows > STRASS_PAR_LIMIT)
		m1 = strassenRec(a11, b12b22);
		
		#pragma omp task shared(m4) /*firstprivate(a22,b21b11)*/ if(rows > STRASS_PAR_LIMIT)
		m4 = strassenRec(a22, b21b11);
		
		#pragma omp task shared(m2) /*firstprivate(a11a12,b22)*/ if(rows > STRASS_PAR_LIMIT)
		m2 = strassenRec(a11a12, b22);

		#pragma omp task shared(m7) /*firstprivate(a11a21,b11b12)*/ if(rows > STRASS_PAR_LIMIT)
		m7 = strassenRec(a11a21, b11b12);
		//Executed from calling thread.
		m6 = strassenRec(a12-a22, b21+b22);

		#pragma omp taskwait
	}

	//Result placeholder.
	CMatrix result(left.getHeight(), left.getWidth(), false);
	//Subviews of result matrix.
	CMatrix c11(result, 0,0, 		halfr,halfc);
	CMatrix c12(result, 0,halfc, 	halfr,cols);
	CMatrix c21(result, halfr,0, 	rows,halfc);
	CMatrix c22(result, halfr,halfc,rows,cols);

	//Assign m(i) matrices to subviews of result.
 	if (halfr < STRASS_LIMIT || halfc < STRASS_LIMIT){ //magic fix
 		c11 = m5 + m4 - m2 + m6;
		c12 = m1 + m2;
 	}else{
 		c12 = m5 + m4 - m2 + m6;
		c11 = m1 + m2;
	}
	c21 = m3 + m4;
	c22 = m1 + m5 - m3 - m7;

	return result;
}

//-----------------------------------------------------------------------//
//-----------------------------------------------------------------------//