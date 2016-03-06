#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

using namespace std;

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
	uint64_t rows;
	uint64_t rmax;
	uint64_t cols;
	double ** m_Matrix;
	bool isSubMatrix
public:
	CMatrix ( int r = 0, int c = 0, bool ctrl = 0 )
	{	
		isSubMatrix = false;

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
		isSubMatrix = false;

		cols = right.cols;
		rows = right.rows;
		m_Matrix = new double* [rows];

		for (int i = 0; i < rows; ++i)
			m_Matrix[i] = new double [cols];
		for (int i = 0; i < rows; ++i)
			for (int j = 0; j < cols; ++j){
					m_Matrix[i][j] = right.m_Matrix[i][j];
				}
	}

	//constructs submatrix of right without copying its content
	CMatrix (const CMatrix & right, uint64_t rows, uint64_t cols)
	{
		isSubMatrix = true;

		this.cols = cols;
		this.rows = rows;
		m_Matrix = right.m_Matrix
	}

	~CMatrix ()
	{
		if (isSubMatrix == false){
			for (int i = 0; i < rows; ++i){
				delete [] m_Matrix[i];
			}
			delete [] m_Matrix;
		}
	}

	CMatrix & operator = ( const CMatrix & right )
	{
		if ( this == &right ) return *this;
		freeMatrix( m_Matrix, rows );
		if (cols != right.cols || rows != right.rows){
			cols = right.cols;
			rows = right.rows;
			m_Matrix = new double* [rows];
		
			for (int i = 0; i < rows; ++i)
				m_Matrix[i] = new double [cols];
		}
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
        return Proxy( m_Matrix[r], r, rows, cols );
    }

    uint64_t getWidth()
    {
    	return cols;
    }

    uint64_t getHeight()
    {
    	return rows;
    }

	friend ostream & operator << ( ostream & os, const CMatrix & in );
	friend istream & operator >> ( istream & is, CMatrix & out );
	friend CMatrix strassenMult(const CMatrix & left, const CMatrix & right);
	friend CMatrix strassenRecursion(const CMatrix & left, const CMatrix & right);

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

int main(int argc, char const *argv[])
{
	istringstream is;
	ostringstream os;
	
	CMatrix matrixA;
	CMatrix matrixB;
	ifstream infileA("matrixA.txt");
	ifstream infileB("matrixB.txt");

	if (!infileA.good() || !infileB.good()){
		cerr << "Error opening 'matrixA.txt' or 'matrixB.txt'\n";
		return 1;
	}

	infileA >> matrixA;
	infileB >> matrixB;

	if (!infileA.good() || !infileB.good()){
		cerr << "Error reading 'matrixA.txt' or 'matrixB.txt' (bad syntax)\n";
		return 1;
	}

	cout << matrixA;
	cout << "\n\n";
	cout << matrixB;
	cout << "\n\n";
	cout << matrixA*matrixB;

	return 0;
}
