//
// Created by jmh on 7/26/16.
//

#include <algorithm>
#include "csvparser.h"
#include <fstream>
#include <vector>
#include <cmath>

using namespace std;

vector<int> bincount(const vector<int>& x){
    auto num_bins = *max_element(x.cbegin(), x.cend());
    vector<int> counts(num_bins+1, 0);

    for(int elem: x) counts[elem]++;

    return counts;
}

vector<int> hist2d(const vector<int>& x, const vector<int>& y){
    vector<int> xy(x.size(), 0);

    auto numYBins = *max_element(y.cbegin(), y.cend()) + 1;


    transform(x.begin(), x.end(), xy.begin(), bind1st(multiplies<int>(), numYBins));
    transform(y.begin(), y.end(), xy.begin(), xy.begin(), plus<int>());

    return xy;
}

vector<vector<pair<long, long>>> shardIndices(long n, long numThreads){
    vector<vector<pair<long, long>>> shardedVector;
    long i = 0, j = 0;
    long numElem = n*(n+1)/2;
    auto threadSize = numElem/numThreads;

    /*
     * The pairs of MI indices are split up so in the following way:
     * All numThreads-1 threads have length threadSize.
     * The final thread takes all the leftover indices (since the number of
     * calculations, numElem, may not divide evenly into numThreads)
     * */

    long elemCount = 0;
    for (int thread = 0; thread < numThreads; thread++){

        vector<pair<long, long>> aShard;

        /* First numThreads-1 threads */
        if (thread < numThreads - 1){
            for (long k = elemCount; k < elemCount + threadSize; k++) {
                j = (long) ((-1 + sqrt(8 * k + 1)) / 2);
                i = k - j * (j + 1) / 2;
                aShard.push_back({i, j});
            }
            elemCount += threadSize;
        }

            /* Last thread */
        else{
            for (long k = elemCount; k < numElem; k++){
                j = (long) ((-1 + sqrt(8 * k + 1)) / 2);
                i = k - j * (j + 1) / 2;
                aShard.push_back({i, j});
            }
        }
        shardedVector.push_back(aShard);
    }

    return shardedVector;
}

vector<vector<int>> readCSV(const char* filename, const char* delimiter) {
    vector<vector<int>> matrix;
    int i = 0;

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

void dumpCSV(const char* filename, const char* delimiter, vector<vector<float>> &miMatrix){

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
