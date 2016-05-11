#pragma once

#include <iostream>
#include <fstream>
#include <iomanip>
#include <cmath>
#include <omp.h>


#define uint unsigned long long

//-----------------------------------------------------------------------//
//-----------------------------------------------------------------------//

class Proxy {
	public:
	    Proxy( double * in, const uint r, const uint & rowsAllocated, const uint & cmax,
	    		 const uint & sr,  const uint & sc);
	    double & operator [] ( const uint c ) const;
	private:
	    double * m_Array;
	    uint r, cmax, rowsAllocated, sr, sc;
};

//-----------------------------------------------------------------------//
//-----------------------------------------------------------------------//

class CMatrix
{
private:
	uint offrows; //Offset for submatrices rows beginning
	uint offcols; //Offset for submatrices columns beginning
	uint rows; //Allocated rows
	uint cols; //Allocated columns
	uint rowsAllocated; //Used for matrice parsing
	double **__restrict__ m_Matrix; //2D array
	bool isSubMatrice; //Submatrice mark
public:
	CMatrix ( uint r = 0, uint c = 0, bool ctrl = false );
	//Copy constructor
	CMatrix ( const CMatrix & right, bool sw = false );
	//Constructs submatrices of right without copying its content (subview)
	CMatrix (const CMatrix & right, uint fromRows, uint fromCols, uint toRows, uint toCols);
	~CMatrix ();
	CMatrix & operator = ( const CMatrix & right );
	CMatrix operator + ( const CMatrix & right ) const;
	CMatrix operator - ( const CMatrix & right ) const;
	CMatrix operator - ( void ) const;
	CMatrix operator * ( const CMatrix & right ) const;
	CMatrix operator * ( const double num ) const;
	Proxy operator [] ( const uint r ) const;
	uint getWidth() const;
	uint getHeight() const;
	//Our friends
	friend std::ostream & operator << ( std::ostream & os, const CMatrix & in );
	friend std::istream & operator >> ( std::istream & is, CMatrix & out );
	//friend CMatrix strassenMult(const CMatrix & left, const CMatrix & right);
	//friend CMatrix strassenRec(const CMatrix & left, const CMatrix & right, uint depth, uint id);
};

//-----------------------------------------------------------------------//
//-----------------------------------------------------------------------//

class CSizeException
{
private:
	const char op;
	const uint r1, c1, r2, c2;
public:
	CSizeException	( const uint & a1, const uint & a2, const uint & b1,
					  const uint & b2, const char zn) : op (zn), r1(a1), c1(a2), r2(b1), c2(b2) {}
	friend std::ostream & operator << ( std::ostream & os, const CSizeException & in );
};

//-----------------------------------------------------------------------//
//-----------------------------------------------------------------------//

class CIndexException
{
private:
	const uint r, c;
public:
	CIndexException ( const uint & a1, const uint & a2 ) : r(a1), c(a2) {}
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