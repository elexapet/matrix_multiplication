
#define STRASS_SIZE_LIMIT 512

//-----------------------------------------------------------------------//
//-----------------------------------------------------------------------//

CMatrix strassenMult(const CMatrix & left, const CMatrix & right)
{
	//Use standard multiplication when matrixes are small enough.
	if (left.getWidth() < STRASS_SIZE_LIMIT || right.getWidth() < STRASS_SIZE_LIMIT)
		return left*right;

	//padd matrixes to 2^N x 2^N size
	//todo

	return strassenRecursion(left, right);
)

CMatrix strassenRecursion(const CMatrix & left, const CMatrix & right){
	//Use standard multiplication when matrixes are small enough.
	if (left.getWidth() < STRASS_SIZE_LIMIT || right.getWidth() < STRASS_SIZE_LIMIT)
		return left*right;

	CMatrix *m1,*m2,*m3,*m4,*m5,*m6,*m7;
	CMatrix result(left.rows, right.cols, true);

	//todo fix generation to produce submatrix not only from 0,0 to x,y
	m1 = &strassenRecursion((CMatrix(left,1,1) + CMatrix(left,2,2),(CMatrix(right,1,1) + CMatrix(right,2,2)))
	m2 = &strassenRecursion((CMatrix(left,2,1) + CMatrix(left,2,2), CMatrix(right,1,1))
	m3 = &strassenRecursion(CMatrix(left,1,1), (CMatrix(right,1,2) - CMatrix(right,2,2)))
	m4 = &strassenRecursion(CMatrix(left,2,2) , (CMatrix(right,2,1) - CMatrix(right,1,1)))
	m5 = &strassenRecursion((CMatrix(left,1,1) + CMatrix(left,1,2)), CMatrix(right,2,2))
	m6 = &strassenRecursion((CMatrix(left,2,1) - CMatrix(left,1,1)), (CMatrix(right,1,1) + CMatrix(right,1,2)))
	m7 = &strassenRecursion((CMatrix(left,1,2) - CMatrix(left,2,2)), (CMatrix(right,2,1) + CMatrix(right,2,2)))

	CMatrix(result, 1, 1) = *m1 + *m4 - *m5 + *m7;
	CMatrix(result, 1, 2) = *m3 + *m5;
	CMatrix(result, 2, 1) = *m2 + *m4;
	CMatrix(result, 2, 2) = *m1 - *m2 + *m3 + *m6;

	return C;
)

//-----------------------------------------------------------------------//
//-----------------------------------------------------------------------//