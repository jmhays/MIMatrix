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
    long numElem = n*(n-1)/2;
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
                i = n - 2 - floor(sqrt(-8*k+4*n*(n-1) - 7)/2.0 - 0.5);
                j = k + i + 1 - n*(n-1)/2 + (n-i)*((n-i)-1)/2;
                aShard.push_back({i, j});
            }
            elemCount += threadSize;
        }

            /* Last thread */
        else{
            for (long k = elemCount; k < numElem; k++){
                i = n - 2 - floor(sqrt(-8*k+4*n*(n-1) - 7)/2.0 - 0.5);
                j = k + i + 1 - n*(n-1)/2 + (n-i)*((n-i)-1)/2;
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

void dumpMIMat(const char* filename, vector<float> &miMatrix){

    ofstream out(filename);
    auto matDim = miMatrix.size();

    for (auto i=0; i < matDim; i++){
        out << miMatrix[i] << endl;
    }
}

void dumpIndices(const char* filename, vector<vector<pair<long, long>>> &shardedIndices, long n){

    FILE * out;
    out = fopen(filename, "w");
    auto nThreads = shardedIndices.size();

    for (int i = 0; i < nThreads; i++){
        for (int j = 0; j < shardedIndices[i].size(); j++){
            auto firstPair = shardedIndices[i][j].first;
            auto secondPair = shardedIndices[i][j].second;
            auto k = (n*(n-1)/2) - (n-firstPair)*((n-firstPair)-1)/2 + secondPair - firstPair - 1;
            fprintf(out, "%li %li %li\n", k, firstPair, secondPair);
        }
    }
}