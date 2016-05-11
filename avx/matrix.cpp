#include "matrix.h"

using namespace std;

//-----------------------------------------------------------------------//
//-----------------------------------------------------------------------//

void freeMatrix ( double ** array, uint64_t rows )
{
   for ( uint64_t i=0; i < rows; i++ ) delete [] array[i];
   delete [] array;
}

//-----------------------------------------------------------------------//
//-----------------------------------------------------------------------//

ostream & operator << ( ostream & os, const CSizeException & in )
{
	os << "Invalid matrix size " 
	   << in.r1 << "x" << in.c1
	   << " " << in.op << " "
	   << in.r2 << "x" << in.c2;
	return os;
}

//-----------------------------------------------------------------------//
//-----------------------------------------------------------------------//

ostream & operator << ( ostream & os, const CIndexException & in )
{
	os << "Invalid index ["
	   << in.r
	   << "]["
	   << in.c
	   << "]";
	return os;
}

//-----------------------------------------------------------------------//
//-----------------------------------------------------------------------//

ostream & operator << ( ostream & os, const CSubviewAssigmentException & in )
{
	os << "Assigning matrice with different size to subview is not allowed";
	return os;
}

//-----------------------------------------------------------------------//
//-----------------------------------------------------------------------//

//Constructor
CMatrix::CMatrix ( uint64_t r, uint64_t c, bool ctrl )
{	
	isSubMatrice = false;
	offrows = offcols = 0;

	cols = c;
	rows = r;
	rowsAllocated = rows;
	m_Matrix = new double* [rows];

	for (uint64_t i = 0; i < rows; ++i)
		m_Matrix[i] = new double [cols];	
	if (ctrl == false)
	{
		for (uint64_t i = 0; i < rows; ++i)
			for (uint64_t j = 0; j < cols; ++j)
			{
				m_Matrix[i][j] = 0;
			}
	}
}
//Copy constructor
CMatrix::CMatrix ( const CMatrix & right, bool sw)
{
	isSubMatrice = false;
	offcols = offrows = 0;
	rowsAllocated = rows;

	if (sw == true){
		double exponent = log2(right.getWidth() > right.getHeight() ? right.getWidth() : right.getHeight() );
		exponent = round(exponent);
		cols = rows = pow(2, exponent);
	}else{
		cols = right.getWidth();
		rows = right.getHeight();
	}

	m_Matrix = new double* [rows];
	for (uint64_t i = 0; i < rows; ++i)
		m_Matrix[i] = new double [cols];
	
	for (uint64_t i = 0; i < right.rows; ++i)
		for (uint64_t j = 0; j < right.cols; ++j){
			m_Matrix[i][j] = right.m_Matrix[i][j];
		}
	
}

//Constructs submatrices of right without copying its content (subview)
CMatrix::CMatrix (const CMatrix & right, uint64_t fromRows, uint64_t fromCols, uint64_t toRows, uint64_t toCols)
{
	isSubMatrice = true;
	offrows = fromRows;
	offcols = fromCols;
	this->cols = toCols;
	this->rows = toRows;
	m_Matrix = right.m_Matrix;
	rowsAllocated = right.rowsAllocated;
}

CMatrix::~CMatrix ()
{
	if (isSubMatrice == false){
		for (uint64_t i = 0; i < rows; ++i){
			delete [] m_Matrix[i];
		}
		delete [] m_Matrix;
	}
}

CMatrix & CMatrix::operator = ( const CMatrix & right )
{
	if (this == &right) return *this;

	if (getWidth() != right.getWidth() || getHeight() != right.getHeight()){
		if (isSubMatrice) throw CSubviewAssigmentException();
		freeMatrix( m_Matrix, rows );
		cols = right.cols;
		rows = right.rows;
		m_Matrix = new double* [rows];
	
		for (uint64_t i = 0; i < rows; ++i)
			m_Matrix[i] = new double [cols];
	}
	for (uint64_t i = 0; i < getHeight(); ++i)
		for (uint64_t j = 0; j < getWidth(); ++j)
		{
			m_Matrix[i+offrows][j+offcols] = right.m_Matrix[i+right.offrows][j+right.offcols];
		}
	return *this;
}

CMatrix CMatrix::operator + ( const CMatrix & right ) const
{
	if (getWidth() != right.getWidth() || getHeight() != right.getHeight())
		throw CSizeException ( getHeight(), getWidth(), right.getHeight(), right.getWidth(), '+' );
	CMatrix tmp ( getHeight(), getWidth(), true );
	for (uint64_t i = 0; i < tmp.rows; ++i)
		for (uint64_t j = 0; j < tmp.cols; ++j)
		{
			tmp.m_Matrix[i][j] = m_Matrix[i+offrows][j+offcols] + right.m_Matrix[i+right.offrows][j+right.offcols]; 
		}
	return tmp;
}

CMatrix CMatrix::operator - ( const CMatrix & right ) const
{
	if (getWidth() != right.getWidth() || getHeight() != right.getHeight())
		throw CSizeException ( getHeight(), getWidth(), right.getHeight(), right.getWidth(), '-' );
	CMatrix tmp ( getHeight(), getWidth(), true );
	for (uint64_t i = 0; i < tmp.rows; ++i)
		for (uint64_t j = 0; j < tmp.cols; ++j)
		{
			tmp.m_Matrix[i][j] = m_Matrix[i+offrows][j+offcols] - right.m_Matrix[i+right.offrows][j+right.offcols]; 
		}
	return tmp;
}

CMatrix CMatrix::operator - ( void ) const
{
	CMatrix tmp ( getHeight(), getWidth(), true );
	for (uint64_t i = 0; i < tmp.rows; ++i)
		for (uint64_t j = 0; j < tmp.cols; ++j)
		{
			tmp.m_Matrix[i][j] = -m_Matrix[i+offrows][j+offcols];
		}
	return tmp;		
}


CMatrix CMatrix::operator * ( const double num ) const
{
	CMatrix tmp ( getHeight(), getWidth(), true );
	for (uint64_t i = 0; i < tmp.rows; ++i)
		for (uint64_t j = 0; j < tmp.cols; ++j)
		{
			tmp.m_Matrix[i][j] = num * m_Matrix[i+offrows][j+offcols];
		}
	return tmp;
}


Proxy::Proxy( double* in, const uint64_t r, const uint64_t & rowsAllocated, const uint64_t & cmax,
		 const uint64_t & sr,  const uint64_t & sc) 
		: m_Array(in), r(r), cmax(cmax), rowsAllocated(rowsAllocated), sr(sr), sc(sc) {}

double & Proxy::operator [] ( const uint64_t c ) const
{
    if ( c < sc || c >= cmax || r < sr || r >= rowsAllocated ) throw CIndexException ( r, c );
	return m_Array[c];
}

Proxy CMatrix::operator [] ( const uint64_t r ) const
{
    return Proxy( m_Matrix[r], r, rows, cols, offrows, offcols );
}

uint64_t CMatrix::getWidth() const
{
	return cols - offcols;
}

uint64_t CMatrix::getHeight() const
{
	return rows - offrows;
}

//-----------------------------------------------------------------------//
//-----------------------------------------------------------------------//

ostream & operator << ( ostream & os, const CMatrix & in )
{
	os << "{";
	for (uint64_t i = in.offrows; i < in.rows; ++i)
	{
		if ( i > in.offrows ) os << ", \n";
		os << "{";
		for (uint64_t j = in.offcols; j < in.cols; ++j)
		{
			if ( j > in.offcols ) os << ", ";
			os << in.m_Matrix[i][j];
		}
		os << "}";
	}	
	os << "}";
	return os;
}

//-----------------------------------------------------------------------//
//-----------------------------------------------------------------------//

istream & operator >> ( istream & is, CMatrix & out )
{
	uint64_t unpaired = 0, r = 0, c, refc = 0, rowsAllocated = 64, cmax = 64;
	double num;
	char zn;
	uint64_t msize;

	is >> msize;

	freeMatrix ( out.m_Matrix, out.rows );
	out.cols = msize;
	out.rows = msize;
	out.m_Matrix = new double* [out.rows];
	for (uint64_t i = 0; i < out.rows; ++i)
	out.m_Matrix[i] = new double [out.cols];
	
	is >> zn;
	if ( zn != '{' ) { is.setstate (ios::failbit); return is; }
	do
	{
		c = 0;
		is >> zn;
		if ( zn == '{' )
		{
			unpaired++;
			do
			{
				is >> num;
				if ( !is ) return is;
				out.m_Matrix[r][c] = num;
				c++;
				is >> zn;
			} while ( zn == ',' );
			if ( zn == '}' )
			{
				if ( !refc ) refc = c;
				else if ( refc != c )break;
				unpaired--;
			} 
			else break;
		}
		else break;
		r++;
	} while ( is.get() == ',' );
	if ( unpaired ) { is.setstate (ios::failbit); return is; }
	is >> zn;
	if ( zn != '}' )  { is.setstate (ios::failbit); return is; }

	out.rowsAllocated = rowsAllocated;

	return is;
}

//-----------------------------------------------------------------------//
//-----------------------------------------------------------------------//
