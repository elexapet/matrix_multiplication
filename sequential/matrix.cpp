#ifndef __PROGTEST__
#include <iostream>
#include <sstream>
#include <iomanip>
using namespace std;
#endif /* __PROGTEST__ */

//-----------------------------------------------------------------------//
//-----------------------------------------------------------------------//

void freeMatrix ( double ** array, int rows )
{
   for ( int i=0; i < rows; i++ ) delete [] array[i];
   delete [] array;
}

//-----------------------------------------------------------------------//
//-----------------------------------------------------------------------//

class CSizeException
{
private:
	const char op;
	const int r1, c1, r2, c2;
public:
	CSizeException	( const int & a1, const int & a2, const int & b1,
					  const int & b2, const char zn) : op (zn), r1(a1), c1(a2), r2(b1), c2(b2) {}
	friend ostream & operator << ( ostream & os, const CSizeException & in );
};

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

class CIndexException
{
private:
	const int r, c;
public:
	CIndexException ( const int & a1, const int & a2 ) : r(a1), c(a2) {}
	friend ostream & operator << ( ostream & os, const CIndexException & in );
};

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

class CMatrix
{
private:
	int rows;
	int rmax;
	int cols;
	double ** m_Matrix;
public:
	CMatrix ( int r = 0, int c = 0, bool ctrl = 0 )
	{	
		cols = c;
		m_Matrix = new double* [rows = r];
		for (int i = 0; i < rows; ++i)
			m_Matrix[i] = new double [cols];	
		if (!ctrl)
		{
			for (int i = 0; i < rows; ++i)
				for (int j = 0; j < cols; ++j)
				{
					m_Matrix[i][j] = 0;
				}
		}
	}

	CMatrix ( const CMatrix & right )
	{
		cols = right.cols;
		rows = right.rows;
		m_Matrix = new double* [rows];
		for (int i = 0; i < rows; ++i)
			m_Matrix[i] = new double [cols];
		for (int i = 0; i < rows; ++i)
			for (int j = 0; j < cols; ++j)
				{
					m_Matrix[i][j] = right.m_Matrix[i][j];
				}
	}
	~CMatrix ()
	{
		for ( int i = 0; i < rows; ++i )
			delete [] m_Matrix[i];
		delete [] m_Matrix;
	}

	CMatrix & operator = ( const CMatrix & right )
	{
		if ( this == &right ) return *this;
		freeMatrix ( m_Matrix, rows );
		cols = right.cols;
		rows = right.rows;
		m_Matrix = new double* [rows];
		for (int i = 0; i < rows; ++i)
			m_Matrix[i] = new double [cols];

		for (int i = 0; i < rows; ++i)
			for (int j = 0; j < cols; ++j)
				{
					m_Matrix[i][j] = right.m_Matrix[i][j];
				}
		return *this;
	}

	CMatrix operator + ( const CMatrix & right ) const
	{
		if ( rows != right.rows || cols != right.cols )
			throw CSizeException ( rows, cols, right.rows, right.cols, '+' );
		CMatrix tmp ( rows, cols, true );
		for (int i = 0; i < rows; ++i)
			for (int j = 0; j < cols; ++j)
			{
				tmp.m_Matrix[i][j] = m_Matrix[i][j] + right.m_Matrix[i][j]; 
			}
		return tmp;
	}

	CMatrix operator - ( const CMatrix & right ) const
	{
		if ( rows != right.rows || cols != right.cols )
			throw CSizeException ( rows, cols, right.rows, right.cols, '-' );
		CMatrix tmp ( rows, cols, true );
		for (int i = 0; i < rows; ++i)
			for (int j = 0; j < cols; ++j)
			{
				tmp.m_Matrix[i][j] = m_Matrix[i][j] - right.m_Matrix[i][j]; 
			}
		return tmp;	
	}

	CMatrix operator - ( void ) const
	{
		CMatrix tmp ( rows, cols, true );
		for (int i = 0; i < rows; ++i)
			for (int j = 0; j < cols; ++j)
			{
				tmp.m_Matrix[i][j] = -1 * m_Matrix[i][j];
			}
		return tmp;		
	}

	CMatrix operator * ( const CMatrix & right ) const
	{
		if ( cols != right.rows )
			throw CSizeException ( rows, cols, right.rows, right.cols, '*' );
		CMatrix tmp ( rows, right.cols );
		for (int i = 0; i < rows; ++i)
			for (int j = 0; j < right.cols; ++j)
				for (int k = 0; k < cols; ++k)
				{
					tmp.m_Matrix[i][j] += m_Matrix[i][k] * right.m_Matrix[k][j];
				}
		return tmp;	
	}

	CMatrix operator * ( const double num ) const
	{
		CMatrix tmp ( rows, cols, true );
		for (int i = 0; i < rows; ++i)
			for (int j = 0; j < cols; ++j)
			{
				tmp.m_Matrix[i][j] = num * m_Matrix[i][j];
			}
		return tmp;
	}

	class Proxy {
    public:
        Proxy( double* in, const int r, const int & rmax, const int & cmax ) 
				: m_Array(in), r(r), cmax(cmax), rmax(rmax) {}
        double & operator [] ( const int c ) const
        {
            if ( c < 0 || c >= cmax || r < 0 || r >= rmax ) throw CIndexException ( r, c );
			return m_Array[c];
        }
    private:
        double* m_Array;
        int r, cmax, rmax;
    };

    Proxy operator [] ( const int r ) const
    {
        return Proxy ( m_Matrix[r], r, rows, cols );
    }

	friend ostream & operator << ( ostream & os, const CMatrix & in );
	friend istream & operator >> ( istream & is, CMatrix & out );
};

//-----------------------------------------------------------------------//
//-----------------------------------------------------------------------//

ostream & operator << ( ostream & os, const CMatrix & in )
{
	os << "{";
	for (int i = 0; i < in.rows; ++i)
	{
		if ( i > 0 ) os << ", ";
		os << "{";
		for (int j = 0; j < in.cols; ++j)
		{
			if ( j > 0 ) os << ", ";
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
	int unpaired = 0, r = 0, c, refc = 0, rmax = 64, cmax = 64;
	CMatrix tmp ( rmax, cmax );
	double num;
	char zn;

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
				tmp.m_Matrix[r][c] = num;
				c++;
				if ( c >= cmax && !refc )
				{
					cmax *= 2;
					CMatrix reloc = tmp;
					tmp.~CMatrix();
					CMatrix tmp ( rmax, cmax );
					for (int i = 0; i < r; ++i)
						for (int j = 0; j < c; ++j)
						{
							tmp[i][j] = reloc[i][j];
						}
					tmp.~CMatrix();
				}
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
		if ( r >= rmax )
		{
			rmax *= 2;
			CMatrix reloc = tmp;
			tmp.~CMatrix();
			CMatrix tmp ( rmax, cmax );
			for (int i = 0; i < r; ++i)
				for (int j = 0; j < refc; ++j)
				{
					tmp[i][j] = reloc[i][j];
				}	
			reloc.~CMatrix();
		} 
	} while ( is.get() == ',' );
	if ( unpaired ) { is.setstate (ios::failbit); return is; }
	is >> zn;
	if ( zn != '}' )  { is.setstate (ios::failbit); return is; }

	freeMatrix ( out.m_Matrix, out.rows );
	out.cols = refc;
	out.rows = r;
	out.m_Matrix = new double* [out.rows];
	for (int i = 0; i < out.rows; ++i)
		out.m_Matrix[i] = new double [out.cols];
		
	for (int i = 0; i < out.rows; ++i)
		for (int j = 0; j < out.cols; ++j)
			out.m_Matrix[i][j] = tmp[i][j];
	return is;
}

//-----------------------------------------------------------------------//
//-----------------------------------------------------------------------//

#ifndef __PROGTEST__
int main(int argc, char const *argv[])
{
	istringstream is;
	ostringstream os;
	double x;
	
	CMatrix a ( 2, 3 );
	a[0][0] = 1;
	a[0][1] = 2;
	a[0][2] = 3;
	a[1][0] = 4;
	a[1][1] = 5;
	a[1][2] = 6;
	const CMatrix b = a;
	CMatrix c ( 3, 2 );
	c[0][0] = 1;
	c[0][1] = 1;
	c[1][0] = 2;
	c[1][1] = -2;
	c[2][0] = 3;
	c[2][1] = 3;
	os . str ("");
	os << a;
	/*
	--8<----8<----8<----8<----8<--
	{
	  {1, 2, 3},
	  {4, 5, 6}
	}
	--8<----8<----8<----8<----8<--
	*/
	os . str ("");
	os << b;
	/*
	--8<----8<----8<----8<----8<--
	{
	  {1, 2, 3},
	  {4, 5, 6}
	}
	--8<----8<----8<----8<----8<--
	*/
	os . str ("");
	os << c;
	/*
	--8<----8<----8<----8<----8<--
	{
	  {1, 1},
	  {2, -2},
	  {3, 3}
	}
	--8<----8<----8<----8<----8<--
	*/
	CMatrix d ( 1, 1 );
	d[0][0] = -1;
	os . str ("");
	os << d;
	/*
	--8<----8<----8<----8<----8<--
	{
	  {-1}
	}
	--8<----8<----8<----8<----8<--
	*/
	d = a + b;
	os . str ("");
	os << d;
	/*
	--8<----8<----8<----8<----8<--
	{
	  {2, 4, 6},
	  {8, 10, 12}
	}
	--8<----8<----8<----8<----8<--
	*/
	d = a - b;
	os . str ("");
	os << d;
	/*
	--8<----8<----8<----8<----8<--
	{
	  {0, 0, 0},
	  {0, 0, 0}
	}
	--8<----8<----8<----8<----8<--
	*/
	d = - a;
	os . str ("");
	os << d;
	/*
	--8<----8<----8<----8<----8<--
	{
	  {-1, -2, -3},
	  {-4, -5, -6}
	}
	--8<----8<----8<----8<----8<--
	*/
	d = a * 2;
	os . str ("");
	os << d;
	/*
	--8<----8<----8<----8<----8<--
	{
	  {2, 4, 6},
	  {8, 10, 12}
	}
	--8<----8<----8<----8<----8<--
	*/
	d = b * c;
	os . str ("");
	os << d;
	/*
	--8<----8<----8<----8<----8<--
	{
	  {14, 6},
	  {32, 12}
	}
	--8<----8<----8<----8<----8<--
	*/

	x = b[0][0]; // x = 1.000000

	is . clear ();
	is . str ( "{ { 1, 2, 3, 4 }, { 4, 3, 2, 1 } } " );
	is >> a; // is . fail () = false
	d = a;
	os . str ("");
	os << a;
	/*
	--8<----8<----8<----8<----8<--
	{
	  {1, 2, 3, 4},
	  {4, 3, 2, 1}
	}
	--8<----8<----8<----8<----8<--
	*/
	is . clear ();
	is . str ( "{ { 1, 2, 3 } { 3, 4, 5} } " );
	is >> c; 
	cout << is . fail ();
	return 0;
}
#endif /* __PROGTEST__ */

