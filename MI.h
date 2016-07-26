//
// Created by jmh on 7/15/16.
//

#ifndef MIMATRIX_MI_H
#define MIMATRIX_MI_H

float entropy(const vector<int>& arr);
float mi(const vector<int> &x, const vector<int> &y);
void miMatrixBlock(const vector<vector<int>>& data, vector<vector<float>>& mi_matrix,
                   const vector<pair<long, long>>& idx_subset);

#endif //MIMATRIX_MI_H

