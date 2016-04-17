#pragma once

#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdint>
#include <cmath>

//-----------------------------------------------------------------------//
//-----------------------------------------------------------------------//

class Proxy {
	public:
	    Proxy( double * in, const uint64_t r, const uint64_t & rowsAllocated, const uint64_t & cmax,
	    		 const uint64_t & sr,  const uint64_t & sc);
	    double & operator [] ( const uint64_t c ) const;
	private:
	    double * m_Array;
	    uint64_t r, cmax, rowsAllocated, sr, sc;
};

//-----------------------------------------------------------------------//
//-----------------------------------------------------------------------//

class CMatrix
{
private:
	uint64_t offrows; //Offset for submatrices rows beginning
	uint64_t offcols; //Offset for submatrices columns beginning
	uint64_t rows; //Allocated rows
	uint64_t cols; //Allocated columns
	uint64_t rowsAllocated; //Used for matrice parsing
	double ** m_Matrix; //2D array
	bool isSubMatrice; //Submatrice mark
public:
	CMatrix ( uint64_t r = 0, uint64_t c = 0, bool ctrl = false );
	//Copy constructor
	CMatrix ( const CMatrix & right, bool sw = false );
	//Constructs submatrices of right without copying its content (subview)
	CMatrix (const CMatrix & right, uint64_t fromRows, uint64_t fromCols, uint64_t toRows, uint64_t toCols);
	~CMatrix ();
	CMatrix & operator = ( const CMatrix & right );
	CMatrix operator + ( const CMatrix & right ) const;
	CMatrix operator - ( const CMatrix & right ) const;
	CMatrix operator - ( void ) const;
	CMatrix operator * ( const CMatrix & right ) const;
	CMatrix operator * ( const double num ) const;
	Proxy operator [] ( const uint64_t r ) const;
	uint64_t getWidth() const;
	uint64_t getHeight() const;
	//Our friends
	friend std::ostream & operator << ( std::ostream & os, const CMatrix & in );
	friend std::istream & operator >> ( std::istream & is, CMatrix & out );
	//friend CMatrix strassenMult(const CMatrix & left, const CMatrix & right);
	//friend CMatrix strassenRec(const CMatrix & left, const CMatrix & right, uint64_t depth, uint64_t id);
};

//-----------------------------------------------------------------------//
//-----------------------------------------------------------------------//

class CSizeException
{
private:
	const char op;
	const uint64_t r1, c1, r2, c2;
public:
	CSizeException	( const uint64_t & a1, const uint64_t & a2, const uint64_t & b1,
					  const uint64_t & b2, const char zn) : op (zn), r1(a1), c1(a2), r2(b1), c2(b2) {}
	friend std::ostream & operator << ( std::ostream & os, const CSizeException & in );
};

//-----------------------------------------------------------------------//
//-----------------------------------------------------------------------//

class CIndexException
{
private:
	const uint64_t r, c;
public:
	CIndexException ( const uint64_t & a1, const uint64_t & a2 ) : r(a1), c(a2) {}
	friend std::ostream & operator << ( std::ostream & os, const CIndexException & in );
};

//-----------------------------------------------------------------------//
//-----------------------------------------------------------------------//

class CSubviewAssigmentException
{
public:
	CSubviewAssigmentException ( ) {}
	friend std::ostream & operator << ( std::ostream & os, const CSubviewAssigmentException & in );
};

//-----------------------------------------------------------------------//
//-----------------------------------------------------------------------//