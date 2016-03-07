#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cstdint>

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

class CSizeException
{
private:
	const char op;
	const uint64_t r1, c1, r2, c2;
public:
	CSizeException	( const uint64_t & a1, const uint64_t & a2, const uint64_t & b1,
					  const uint64_t & b2, const char zn) : op (zn), r1(a1), c1(a2), r2(b1), c2(b2) {}
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
	const uint64_t r, c;
public:
	CIndexException ( const uint64_t & a1, const uint64_t & a2 ) : r(a1), c(a2) {}
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

class CSubviewAssigmentException
{
private:
public:
	CSubviewAssigmentException ( ) {}
	friend ostream & operator << ( ostream & os, const CSubviewAssigmentException & in );
};

ostream & operator << ( ostream & os, const CSubviewAssigmentException & in )
{
	os << "Assigning matrice to subview of different size is not allowed";
	return os;
}

//-----------------------------------------------------------------------//
//-----------------------------------------------------------------------//

class CMatrix
{
private:
	uint64_t offrows; //Offset for submatrices rows beginning
	uint64_t offcols; //Offset for submatrices columns beginning
	uint64_t rows; //Allocated rows
	uint64_t cols; //Allocated columns
	uint64_t rmax; //Used for matrice parsing
	double ** m_Matrix; //2D array
	bool isSubMatrice; //Submatrice mark
public:
	//Constructor
	CMatrix ( uint64_t r = 0, uint64_t c = 0, bool ctrl = 0 )
	{	
		isSubMatrice = false;
		offrows = offcols = 0;

		cols = c;
		m_Matrix = new double* [rows = r];

		for (uint64_t i = 0; i < rows; ++i)
			m_Matrix[i] = new double [cols];	
		if (!ctrl)
		{
			for (uint64_t i = 0; i < rows; ++i)
				for (uint64_t j = 0; j < cols; ++j)
				{
					m_Matrix[i][j] = .0;
				}
		}
	}
	//Copy constructor
	CMatrix ( const CMatrix & right )
	{
		isSubMatrice = false;
		offcols = offrows = 0;

		cols = right.cols;
		rows = right.rows;
		m_Matrix = new double* [rows];

		for (uint64_t i = 0; i < rows; ++i)
			m_Matrix[i] = new double [cols];
		for (uint64_t i = 0; i < rows; ++i)
			for (uint64_t j = 0; j < cols; ++j){
				m_Matrix[i][j] = right.m_Matrix[i][j];
			}
	}

	//Constructs submatrices of right without copying its content (subview)
	CMatrix (const CMatrix & right, uint64_t fromRows, uint64_t fromCols, uint64_t rows, uint64_t cols)
	{
		isSubMatrice = true;
		offrows = fromRows;
		offcols = fromCols;
		this->cols = cols;
		this->rows = rows;
		m_Matrix = right.m_Matrix;
	}

	~CMatrix ()
	{
		if (isSubMatrice == false){
			for (uint64_t i = 0; i < rows; ++i){
				delete [] m_Matrix[i];
			}
			delete [] m_Matrix;
		}
	}

	CMatrix & operator = ( const CMatrix & right )
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

	CMatrix operator + ( const CMatrix & right ) const
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

	CMatrix operator - ( const CMatrix & right ) const
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

	CMatrix operator - ( void ) const
	{
		CMatrix tmp ( getHeight(), getWidth(), true );
		for (uint64_t i = 0; i < tmp.rows; ++i)
			for (uint64_t j = 0; j < tmp.cols; ++j)
			{
				tmp.m_Matrix[i][j] = -m_Matrix[i+offrows][j+offcols];
			}
		return tmp;		
	}

	CMatrix operator * ( const CMatrix & right ) const
	{
		if ( getWidth() != right.getHeight() )
			throw CSizeException ( getHeight(), getWidth(), right.getHeight(), right.getWidth(), '*' );
		CMatrix tmp ( getHeight(), right.getWidth() );
		for (uint64_t i = 0; i < getHeight(); ++i)
			for (uint64_t j = 0; j < right.getWidth(); ++j)
				for (uint64_t k = 0; k < getWidth(); ++k)
				{
					tmp.m_Matrix[i][j] += m_Matrix[i+offrows][k+offcols] * right.m_Matrix[k+right.offrows][j+right.offcols];
				}
		return tmp;	
	}

	CMatrix operator * ( const double num ) const
	{
		CMatrix tmp ( getHeight(), getWidth(), true );
		for (uint64_t i = 0; i < tmp.rows; ++i)
			for (uint64_t j = 0; j < tmp.cols; ++j)
			{
				tmp.m_Matrix[i][j] = num * m_Matrix[i+offrows][j+offcols];
			}
		return tmp;
	}

	class Proxy {
    public:
        Proxy( double* in, const uint64_t r, const uint64_t & rmax, const uint64_t & cmax,
        		 const uint64_t & sr,  const uint64_t & sc) 
				: m_Array(in), r(r), cmax(cmax), rmax(rmax), sr(sr), sc(sc) {}
        double & operator [] ( const uint64_t c ) const
        {
            if ( c < sc || c >= cmax || r < sr || r >= rmax ) throw CIndexException ( r, c );
			return m_Array[c];
        }
    private:
        double* m_Array;
        uint64_t r, cmax, rmax, sr, sc;
    };

    Proxy operator [] ( const uint64_t r ) const
    {
        return Proxy( m_Matrix[r], r, rows, cols, offrows, offcols );
    }

    uint64_t getWidth() const
    {
    	return cols - offcols;
    }

    uint64_t getHeight() const
    {
    	return rows - offrows;
    }

    //Our friends
	friend ostream & operator << ( ostream & os, const CMatrix & in );
	friend istream & operator >> ( istream & is, CMatrix & out );
	friend CMatrix strassenMult(const CMatrix & left, const CMatrix & right);
	friend CMatrix strassenRec(const CMatrix & left, const CMatrix & right);

};

//-----------------------------------------------------------------------//
//-----------------------------------------------------------------------//

ostream & operator << ( ostream & os, const CMatrix & in )
{
	os << "{";
	for (uint64_t i = 0; i < in.rows; ++i)
	{
		if ( i > 0 ) os << ", \n";
		os << "{";
		for (uint64_t j = 0; j < in.cols; ++j)
		{
			if ( j > 0 ) os << ", ";
			os << in.m_Matrix[i+in.offrows][j+in.offcols];
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
	uint64_t unpaired = 0, r = 0, c, refc = 0, rmax = 64, cmax = 64;
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
					for (uint64_t i = 0; i < r; ++i)
						for (uint64_t j = 0; j < c; ++j)
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
			for (uint64_t i = 0; i < r; ++i)
				for (uint64_t j = 0; j < refc; ++j)
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
	for (uint64_t i = 0; i < out.rows; ++i)
		out.m_Matrix[i] = new double [out.cols];
		
	for (uint64_t i = 0; i < out.rows; ++i)
		for (uint64_t j = 0; j < out.cols; ++j)
			out.m_Matrix[i][j] = tmp[i][j];
	return is;
}

//-----------------------------------------------------------------------//
//-----------------------------------------------------------------------//

int main(int argc, char const *argv[])
{
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
	cout << "Normal:\n" << matrixA*matrixB;
	cout << "\n\n";
	try{
		cout << "Strassen:\n" << strassenMult(matrixA,matrixB);
	}catch(CSizeException &e){
		cout << e << '\n';
	}catch(CIndexException &e){
		cout << e << '\n';
	}catch(CSubviewAssigmentException &e){
		cout << e << '\n';
	}

	return 0;
}
