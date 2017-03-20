//
// Created by jmh on 7/15/16.
//


#ifndef MIMATRIX_MIUTILS_H
#define MIMATRIX_MIUTILS_H

#include <algorithm>
#include "csvparser.h"
#include <fstream>
#include <vector>

using namespace std;

vector<int> bincount(const vector<int>& x);
vector<int> hist2d(const vector<int>& x, const vector<int>& y);
vector<vector<pair<long, long>>> shardIndices(long n, long numThreads);
vector<vector<int>> readCSV(const char* filename, const char* delimiter);
void dumpCSV(const char* filename, const char* delimiter, vector<vector<float>> &miMatrix);

#endif //MIMATRIX_MIUTILS_H
