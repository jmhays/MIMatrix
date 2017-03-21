//
// Created by jmh on 7/15/16.
//

#ifndef MIMATRIX_MI_H
#define MIMATRIX_MI_H

float entropy(const vector<int>& arr);
float mi(const vector<int> &x, const vector<int> &y, bool metric);
void miMatrixBlock(const vector<vector<int>>& data, vector<float>& mi_matrix,
                   const vector<pair<long, long>>& idx_subset, bool metric);

#endif //MIMATRIX_MI_H

