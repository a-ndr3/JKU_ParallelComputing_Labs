#ifndef MINIMATRIX_H
#define MINIMATRIX_H

#include <vector>
#include "matrix.h"

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


class minimatrix 
{
private:
    std::vector<std::vector<int64_t>> parsedMatrix;
    int threads;

public:
    minimatrix(int threadAmount) : threads(threadAmount) {}
    minimatrix() {};


    void parse(myMatrix& matr, int threadId);
    void update(myMatrix& matr, int threadId);
    
    std::vector<std::vector<int64_t>> getParsedMatrix() const {
        return parsedMatrix;
    }

    void swapRows(int64_t row1, int64_t row2);
    vector<int64_t>& getRow(int64_t rowN);
    void setRow(int64_t rowN, const vector<int64_t>& row);
    int64_t get(int64_t rowN, int64_t colN);
    vector<int64_t>& getRowIndexes(int num);
    bool existanceOfRow(int64_t rowN);
    void clear();
    void parse_in_threads(myMatrix& matrix, int threadId);
    void updateRow(int64_t row, const std::vector<int64_t>& newRow);
    void print();
};


#endif

