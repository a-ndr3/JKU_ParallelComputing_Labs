// PC_assignment1.cpp : Defines the entry point for the application.
//

#include "PC_assignment1.h"
#include "globals.h"
#include "matrix.cpp"
#include "gauss_methods.h"

using namespace std;

int main()
{
	// myMatrix* A = new myMatrix(5, 5); heap alloc

	// myMatrix B(5, 5); stack alloc

	//myMatrix ** C = new myMatrix*[5];
	//C[0] = new myMatrix(5, 5);

	//A->fill_matrix();
	//B.fill_matrix();


	//A->print();
	//B.print();

	myMatrix A(4);

	A.fill_matrix(15);

	A.print();

	myMatrix result = gaussJordanElimination(A);
}
