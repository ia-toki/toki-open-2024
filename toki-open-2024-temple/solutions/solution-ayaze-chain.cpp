#include "temple.h"

#include <bits/stdc++.h>
using namespace std;

long long min_weight(int N, int M, int K,
                     std::vector<int> U, std::vector<int> V, std::vector<int> W,
                     std::vector<int> P) {
    vector<int> vec;
    for (int w : W) {
        while (!vec.empty() && vec.back() <= w) {
            vec.pop_back();
        }
        vec.push_back(w);
    }
    return accumulate(vec.begin(), vec.end(), 0ll);
}
