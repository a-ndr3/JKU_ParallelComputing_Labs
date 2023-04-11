#include "minimatrix.h"
#include <iostream>

std::vector<std::vector<int64_t>> parsedMatrix;

void miniParser::parseMatrix() {
	
	vector<std::vector<int64_t>> localPars;
	vector<int64_t> localLines;

	for (int i = 0; i < rows; ++i) {
		
		auto th = i % threadsAmount;
		
		std::vector<int64_t> temp;
		for (int row = 0; row < rows; ++row) 
		{
			temp.push_back(matrix[i][row]);
		}

		localPars.push_back(temp);

		localLines.push_back(th);
	}

	parsedMatrix = localPars;
	lineToThread = localLines;
}

int miniParser::getThreadForSpecificRow(int64_t rowNumber) 
{
	return lineToThread[rowNumber];
}

int miniParser::getNumOfThreadUsed()
{
	return lineToThread.size();
}
