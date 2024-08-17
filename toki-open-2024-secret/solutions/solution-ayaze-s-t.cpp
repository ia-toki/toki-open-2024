#include "secret.h"

#include <bits/stdc++.h>
using namespace std;

const int kMaxN = 2e5 + 5;

vector<int> A;
int M;
int N;

vector<int> uniqueA;
vector<long long> ansActiveBetween;
long long sumSpec, sumSpec2;
set<int> active;

void init(int N, int M, std::vector<int> A) {
    ::M = M;
    ::A = A;
    ::N = N;

    for (int i = 0 ; i+1 < N ; i++) {
        int a = A[i];
        int b = A[i+1];
        int d = abs(a-b);

        sumSpec += min(d, M-d);
        sumSpec2 += d;
    }

    uniqueA = A;
    sort(uniqueA.begin(), uniqueA.end());
    uniqueA.erase(unique(uniqueA.begin(), uniqueA.end()), uniqueA.end());
    ansActiveBetween.resize(uniqueA.size());

    vector<pair<int, int>> events;
    for (int i = 0 ; i+1 < N ; i++) {
        int a = A[i];
        int b = A[i+1];
        if (a > b) swap(a, b);
        int d = b-a;
        events.push_back({a, M-2*d});
        events.push_back({b, 2*d-M});
    }
    sort(events.begin(), events.end());

    long long currentSum = sumSpec2;
    for (int i = 0, j = 0 ; i < static_cast<int>(uniqueA.size()) ; i++) {
        while (j < static_cast<int>(events.size()) && events[j].first <= uniqueA[i]) {
            currentSum += events[j].second;
            j++;
        }
        ansActiveBetween[i] = currentSum;
    }
}

void toggle(int R) {
    if (active.count(R)) active.erase(R);
    else active.insert(R);
}

long long query(int S, int T) {
    if (active.empty()) {
        return sumSpec;
    }

    auto lowestNext = active.lower_bound(uniqueA[0]);
    if (lowestNext == active.end() || *lowestNext >= uniqueA.back()) {
        return sumSpec2;
    }

    // from here on, some falls inside

    // sandwiched
    if (*active.begin() < uniqueA[0] || *active.rbegin() >= uniqueA.back()) {
        return -1;
    }

    auto it = upper_bound(uniqueA.begin(), uniqueA.end(), *lowestNext);
    int villageNext = *it;
    // sandwiched, not everything is contained in between a segment
    if (*active.rbegin() >= villageNext) {
        return -1;
    }

    // safe
    --it;
    int idx = it - uniqueA.begin();
    return ansActiveBetween[idx];
}
