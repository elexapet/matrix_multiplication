#include <iostream>
#include "matrix.h"
#include "strassen.h"
#include <ctime>

using namespace std;

//-----------------------------------------------------------------------//
//-----------------------------------------------------------------------//


int main(int argc, char const *argv[])
{
	CMatrix matrixA;
	CMatrix matrixB;
	ifstream infileA(argv[2]);
	ifstream infileB(argv[2]);

	if (!infileA.good() || !infileB.good()){
		cerr << "Error opening 'matrixA.txt' or 'matrixB.txt'\n";
		cerr << argv[1] << "|" << argv[2] << "\n";
		return 1;
	}

	cout << "reading\n";

	infileA >> matrixA;
	infileB >> matrixB;

	if (!infileA.good() || !infileB.good()){
		cerr << "Error reading 'matrixA.txt' or 'matrixB.txt' (bad syntax)\n";
		return 1;
	}
	
	CMatrix c;


	cout << matrixA.getWidth() << " executing\n";
	uint tms[] = {1,2,4,6,8,12,24};
	//uint tms[] = {61,122,244};
	for (size_t i = 0; i < 7; ++i)
	{
		omp_set_num_threads(tms[i]);
		double beg = omp_get_wtime();

		try{		
			if (*argv[1] == 'n'){
				cout << "normal\n";
				c = matrixA * matrixB;
			}
			if (*argv[1] == 's'){ 
				cout << "strassen\n";
				c = strassenMult(matrixA,matrixB);
			}
		}catch(CSizeException &e){
			cout << e << '\n';
		}catch(CIndexException &e){
			cout << e << '\n';
		}catch(CSubviewAssigmentException &e){
			cout << e << '\n';
		}

		cout << tms[i] << "x threads:" << omp_get_wtime()-beg << "\n";
	}

	//cout << "writing result\n";
	
	//ofstream outfileC("matrixResult");
	//outfileC << c;

	cout << "done\n";

	return 0;
}