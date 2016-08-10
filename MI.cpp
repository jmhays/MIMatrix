//
// Created by jmh on 7/26/16.
//

#include <vector>
#include <cmath>
#include "MIUtils.h"

using namespace std;

float entropy(const vector<int>& arr){
    float s = 0;
    for (int p: arr){
        if (p!=0){
            s += -p*log(p);
        }
    }
    return s;
}

float mi(const vector<int> &x, const vector<int> &y, bool metric){
    float Hx, Hy, Hxy;
    auto n_samples = x.size();

    Hx = entropy(bincount(x))/n_samples + log(n_samples);
    Hy = entropy(bincount(y))/n_samples + log(n_samples);
    auto xy = hist2d(x, y);
    Hxy = entropy(bincount(xy))/n_samples + log(n_samples);
    if (metric)
    {
        return 1 - (Hx + Hy - Hxy) / Hxy;
    }
    else{
        return Hx + Hy - Hxy;
    }
}

void miMatrixBlock(const vector<vector<int>>& data, vector<vector<float>>& mi_matrix,
                   const vector<pair<long, long>>& idx_subset, bool metric){
    int i = 0, j = 0;

    for (const auto& idxs : idx_subset){
        i = idxs.first;
        j = idxs.second;

        mi_matrix[i][j] = mi(data[i], data[j], metric);
        //if (i!=j) mi_matrix[j][i] = mi_matrix[i][j];
    }
}