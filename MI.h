//
// Created by jmh on 7/15/16.
//
#include <vector>
#include <cmath>

#ifndef MIMATRIX_MI_H
#define MIMATRIX_MI_H

#endif //MIMATRIX_MI_H

using namespace std;

double entropy(vector<int>&& arr){
    double s = 0;
    for (int p: arr){
        if (p!=0){
            s += -p*log(p);
        }
    }
    return s;
}

double mi(vector<int> &x, vector<int> &y){
    double Hx, Hy, Hxy;
    int n_samples = (int) x.size();

    Hx = entropy(bincount(x))/n_samples + log(n_samples);
    Hy = entropy(bincount(y))/n_samples + log(n_samples);
    vector<int> xy = hist2d(x, y);
    Hxy = entropy(bincount(xy))/n_samples + log(n_samples);
    return Hx + Hy - Hxy;
}

void miMatrixBlock(vector<vector<int>> &data, vector<vector<double>> &mi_matrix,
                   vector<pair<int, int>> &idx_subset){
    int i = 0, j = 0;

    for (pair<int, int> &idxs : idx_subset){
        i = idxs.first;
        j = idxs.second;

        mi_matrix[i][j] = mi(data[i], data[j]);
        if (i!=j) mi_matrix[j][i] = mi_matrix[i][j];
    }
}