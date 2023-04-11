#ifndef MINIMATRIX_H
#define MINIMATRIX_H

#include <vector>

using namespace std;

class miniParser
{
private:
    int64_t** matrix;
    int64_t rows;
    int64_t cols;
    int threadsAmount;
    std::vector<std::vector<int64_t>> parsedMatrix;
    std::vector<int64_t> lineToThread;

public:
    miniParser(int64_t** matrix, int64_t rows, int64_t cols, int threadsAmount) : matrix(matrix), rows(rows), cols(cols), threadsAmount(threadsAmount) {
        parseMatrix();
    }

    void parseMatrix();

    std::vector<std::vector<int64_t>> getParsedMatrix() const {
        return parsedMatrix;
    }

    int getThreadForSpecificRow(int64_t rowNumber);
    int getNumOfThreadUsed();
};

#endif

