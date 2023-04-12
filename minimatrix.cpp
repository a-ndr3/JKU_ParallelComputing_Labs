#include "minimatrix.h"
#include <iostream>

std::vector<std::vector<int64_t>> parsedMatrix;
vector<std::vector<int64_t>> mini_matrix_row_indices;

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


void minimatrix::parse(myMatrix& matrix, int threadId)
{
	vector<std::vector<int64_t>> localPars;
	vector<std::vector<int64_t>> localind(threads, vector<int64_t>());

	for (int i = 0; i < matrix.getRows(); ++i)
	{

		auto th = i % threads;

		if (th == threadId)
		{
			std::vector<int64_t> temp;
			for (int row = 0; row < matrix.getRows(); ++row)
			{
				temp.push_back(matrix[i][row]);
			}
			localPars.push_back(temp); localind[th].push_back(i);
		}
	}

	for (size_t i = 0; i < localPars.size(); i++)
	{
		parsedMatrix.push_back(localPars[i]);
	}

	for (auto i : localind)
	{
		if (i.size() != 0)
		{
			mini_matrix_row_indices.push_back(i);
		}
	}
}

void minimatrix::update(myMatrix& matrix, int threadId)
{
	for (int i = 0; i < parsedMatrix.size(); ++i) {

		auto th = i % threads;

		if (th == threadId)
		{
			std::vector<int64_t> temp;

			for (int row = 0; row < matrix.getRows(); ++row)
			{
				temp.push_back(matrix[i][row]);
			}
			swap(parsedMatrix[threadId], temp);
		}
	}
}

void minimatrix::swapRows(int64_t row1, int64_t row2)
{
	swap(parsedMatrix[row1], parsedMatrix[row2]);
}

std::vector<int64_t>& minimatrix::getRow(int64_t row) {

	return parsedMatrix[row];
}

void minimatrix::setRow(int64_t row, const std::vector<int64_t>& new_row) {

	parsedMatrix[row] = new_row;
}

int64_t minimatrix::get(int64_t row, int64_t col) {
	return parsedMatrix[row][col];
}

vector<int64_t> minimatrix::getRowIndexes(int num) {
	return mini_matrix_row_indices[num];
}

bool minimatrix::existanceOfRow(int64_t rowN)
{
	return rowN < parsedMatrix.size();
}