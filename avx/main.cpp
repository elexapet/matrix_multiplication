#include <iostream>
#include "matrix.h"
#include "strassen.h"

using namespace std;

//-----------------------------------------------------------------------//
//-----------------------------------------------------------------------//


int main(int argc, char const *argv[])
{
	CMatrix matrixA;
	CMatrix matrixB;
	ifstream infileA("../matrixA.txt");
	ifstream infileB("../matrixB.txt");

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

	//cout << matrixA;
	//cout << "\n\n";
	//cout << matrixB;
	//cout << "\n\n";
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