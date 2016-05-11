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
	ifstream infileA("../matrix1024.txt");
	ifstream infileB("../matrix1024.txt");

	if (!infileA.good() || !infileB.good()){
		cerr << "Error opening 'matrixA.txt' or 'matrixB.txt'\n";
		return 1;
	}

	cout << "reading\n";
	
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
	ofstream outfileC("matrixNormal");
	ofstream outfileD("matrixStrassen");
	CMatrix c, d;

	cout << "executing\n";

	
	try{
		outfileC << matrixA * matrixB;
		outfileD << strassenMult(matrixA,matrixB);
	}catch(CSizeException &e){
		cout << e << '\n';
	}catch(CIndexException &e){
		cout << e << '\n';
	}catch(CSubviewAssigmentException &e){
		cout << e << '\n';
	}	

	return 0;
}