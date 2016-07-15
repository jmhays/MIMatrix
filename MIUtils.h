//
// Created by jmh on 7/15/16.
//

#include <algorithm>
#include "csvparser.h"
#include <fstream>

#ifndef MIMATRIX_MIUTILS_H
#define MIMATRIX_MIUTILS_H

#endif //MIMATRIX_MIUTILS_H

using namespace std;

vector<int> bincount(vector<int> &x){
    int num_bins = *max_element(x.begin(), x.end());
    vector<int> counts(num_bins, 0);

    for(int elem: x) counts[elem]++;

    return counts;
}

vector<int> hist2d(vector<int> &x, vector<int> &y){
    int num_bins = max(*max_element(x.begin(), x.end()),
                       *max_element(y.begin(), y.end()));
    vector<int> xy((int) x.size(), 0);

    transform(x.begin(), x.end(), xy.begin(),
              bind1st(multiplies<int>(), num_bins));
    transform(y.begin(), y.end(), xy.begin(), x.begin(), plus<int>());

    return xy;
}

vector<vector<pair<int, int>>> shardIndices(int n, int numThreads){
    int i = 0, j = 0;
    int numElem = n*(n+1)/2;
    int threadSize = numElem/numThreads;
    vector<vector<pair<int, int>>> shardedVector;

    /*
     * The pairs of MI indices are split up so in the following way:
     * All numThreads-1 threads have length threadSize.
     * The final thread takes all the leftover indices (since the number of
     * calculations, numElem, may not divide evenly into numThreads)
     * */

    int elemCount = 0;
    for (int thread = 0; thread < numThreads; thread++){

        vector<pair<int, int>> aShard;

        /* First numThreads-1 threads */
        if (thread < numThreads - 1){
            for (int k = elemCount; k < elemCount + threadSize; k++) {
                j = (int) ((-1 + sqrt(8 * k + 1)) / 2);
                i = k - j * (j + 1) / 2;
                aShard.push_back({i, j});
            }
            elemCount += threadSize;
        }

        /* Last thread */
        else{
            for (int k = elemCount; k < numElem; k++){
                j = (int) ((-1 + sqrt(8 * k + 1)) / 2);
                i = k - j * (j + 1) / 2;
                aShard.push_back({i, j});
            }
        }
        shardedVector.push_back(aShard);
        }

    return shardedVector;
}

vector<vector<int>> readCSV(const char* filename, const char* delimiter) {
    int i = 0;
    vector<vector<int>> matrix;

    printf("Reading file: %s\n", filename);

    CsvParser *csvparser = CsvParser_new(filename, delimiter, 0);
    CsvRow *row;

    clock_t t;
    t = clock();

    while ((row = CsvParser_getRow(csvparser))) {
        const char **rowFields = CsvParser_getFields(row);
        vector<int> a_row;
        for (i = 0; i < CsvParser_getNumFields(row); i++) {
            a_row.push_back(atoi(rowFields[i]));
        }
        matrix.push_back(a_row);
        CsvParser_destroy_row(row);
    }
    CsvParser_destroy(csvparser);

    printf("Total time to read file: %f\n",
           (float) (clock() - t) / CLOCKS_PER_SEC);

    return matrix;
}

void dumpCSV(const char* filename, const char* delimiter,
             vector<vector<double>> &miMatrix){

    ofstream out(filename);
    int matDim = (int) miMatrix.size();

    for (int i=0; i < matDim; i++){
        for (int j=0; j < matDim; j++){
            if (j==matDim-1){
                out << miMatrix[i][j] << endl;
            }
            else{
                out << miMatrix[i][j] << delimiter;
            }
        }
    }
}
