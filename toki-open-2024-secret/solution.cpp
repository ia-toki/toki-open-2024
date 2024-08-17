#include "secret.h"

#include <bits/stdc++.h>

const int MAX_N = 1e5;

int N, M;
std::vector<int> A;
std::vector<int> compA;
std::vector<int> valueOfCompA;

// road x is the road that connects x and x+1
std::set<int> isRoadBlocked;
std::multiset<int> blockedRoadSet;

namespace findQuery2Range {

int pNode = 0;
int tree[60 * MAX_N];
int L[60 * MAX_N];
int R[60 * MAX_N];
int root[MAX_N + 5];

int update(int n, int l, int r, int pos, int val) {
  int newNd = ++pNode;
  if (l == r) {
    tree[pNode] = tree[n] + val;
    return newNd;
  }
  int mid = (l + r) >> 1;
  if (pos <= mid) {
    int newLeftNd = update(L[n], l, mid, pos, val);
    L[newNd] = newLeftNd;
    R[newNd] = R[n];
  } else {
    int newRightNd = update(R[n], mid+1, r, pos, val);
    L[newNd] = L[n];
    R[newNd] = newRightNd;
  }
  tree[newNd] = tree[L[newNd]] + tree[R[newNd]];
  return newNd;
}

int update(int root, int pos, int val) {
  return update(root, 0, N-1, pos, val);
}

// find smallest number in the set[lN, rN] greater than pos
int queryFindRight(int lN, int rN, int l, int r, int pos) {
  if (tree[rN] - tree[lN] <= 0 || r < pos) {
    return -1;
  }
  if (l >= pos && l == r && tree[rN] - tree[lN] > 0) {
    return l;
  }
  int mid = (l + r) >> 1;
  int res = queryFindRight(L[lN], L[rN], l, mid, pos);
  if (res == -1) {
    res = queryFindRight(R[lN], R[rN], mid+1, r, pos);
  }
  return res;
}

int queryFindRight(int rootL, int rootR, int pos) {
  return queryFindRight(rootL, rootR, 0, N-1, pos);
}

// find biggest number in the set[lN, rN] smaller than pos
int queryFindLeft(int lN, int rN, int l, int r, int pos) {
  if (tree[rN] - tree[lN] <= 0 || l > pos) {
    return -1;
  }
  if (r <= pos && l == r && tree[rN] - tree[lN] > 0) {
    return l;
  }
  int mid = (l + r) >> 1;
  int res = queryFindLeft(R[lN], R[rN], mid+1, r, pos);
  if (res == -1) {
    res = queryFindLeft(L[lN], L[rN], l, mid, pos);
  }
  return res;
}

int queryFindLeft(int rootL, int rootR, int pos) {
  // int compressedPos = upper_bound(valueOfCompA.begin(), valueOfCompA.end(), pos) - valueOfCompA.begin() - 1;
  return queryFindLeft(rootL, rootR, 0, N-1, pos);
}

void prepare() {
  root[0] = update(0, compA[0], 1);
  for (int i = 1; i < N; i++) {
    root[i] = update(root[i-1], compA[i], 1);
  }
}

// return neighbouring village of road x in set[rootL, rootR]
std::pair<int, int> getNeighbourVillageOfRoadX(int x, int rootL, int rootR) {
  int ansL, ansR;
  // handle left
  {
    ansL = queryFindLeft(rootL, rootR, x);
    if (ansL == -1) {
      ansL = queryFindLeft(rootL, rootR, N-1);
    }
  }
  // handle right
  {
    ansR = queryFindRight(rootL, rootR, x+1);
    if (ansR == -1) {
      ansR = queryFindRight(rootL, rootR, 0);
    }
  }
  return {ansL, ansR};
}

}

std::pair<int, int> getQuery2RangeCheckExist(int x, int S, int T) {
  auto [lVil, rVil] = findQuery2Range::getNeighbourVillageOfRoadX(x, S == 0 ? 0 : findQuery2Range::root[S-1], findQuery2Range::root[T]);
  // check if solution exist
  if (lVil > rVil) std::swap(lVil, rVil);
  if (lVil == rVil) {
    // only one village active
    return {lVil, rVil};
  } else {
    bool insideEmpty = blockedRoadSet.lower_bound(lVil) == blockedRoadSet.lower_bound(rVil);
    bool outsideEmpty = *blockedRoadSet.rbegin() < rVil && *blockedRoadSet.begin() >= lVil;
    if (!insideEmpty && outsideEmpty) {
      return {rVil, lVil+N};
    } else if (insideEmpty && !outsideEmpty) {
      return {lVil, rVil};
    } else {
      return {-1, -1};
    }
  }
}

void compressA() {
  std::vector<int> sortTmp = A;
  sort(sortTmp.begin(), sortTmp.end());
  sortTmp.resize(unique(sortTmp.begin(), sortTmp.end()) - sortTmp.begin());
  valueOfCompA = sortTmp;
  compA.resize(N);
  for (int i = 0; i < N; i++) {
    compA[i] = std::lower_bound(sortTmp.begin(), sortTmp.end(), A[i]) - sortTmp.begin();
  }
}

namespace solveNoBlockedRoad {

long long pSum[MAX_N + 5];

void prepare() {
  for (int i = 1; i < N; i++) {
    int l = std::min(A[i-1], A[i]);
    int r = std::max(A[i-1], A[i]);
    pSum[i] = pSum[i-1] + std::min(r - l, l + M - r);
  }
}

long long query(int S, int T) {
  return pSum[T] - pSum[S];
}

}

namespace solveBlockedRoad {

int pNode = 0;
long long tree[60 * MAX_N];
int L[60 * MAX_N];
int R[60 * MAX_N];
int root[MAX_N + 5];

int update(int n, int l, int r, int pos, int val) {
  int newNd = ++pNode;
  if (l == r) {
    tree[newNd] = val;
    return newNd;
  }
  int mid = (l + r) >> 1;
  if (pos <= mid) {
    L[newNd] = update(L[n], l, mid, pos, val);
    R[newNd] = R[n];
  } else {
    L[newNd] = L[n];
    R[newNd] = update(R[n], mid+1, r, pos, val);
  }
  tree[newNd] = tree[L[newNd]] + tree[R[newNd]];
  return newNd;
}

int update(int n, int pos, int val) {
  return update(n, 0, N-1, pos, val);
}

long long query(int n, int l, int r, int ql, int qr) {
  if (ql <= l && r <= qr) {
    return tree[n];
  }
  int mid = (l + r) >> 1;
  long long res = 0;
  if (ql <= mid) res += query(L[n], l, mid, ql, qr);
  if (qr > mid) res += query(R[n], mid+1, r, ql, qr);
  return res;
}

long long query(int n, int ql, int qr) {
  return query(n, 0, N-1, ql, qr);
}


std::vector<std::pair<int, int>> pendingUpdate[MAX_N +  5];

void prepare() {
  for (int i = 1; i < N; i++) {
    if (A[i] == A[i-1]) continue;
    int l = std::min(A[i-1], A[i]);
    int r = std::max(A[i-1], A[i]);
    root[0] = update(root[0], i, r - l);
    pendingUpdate[std::min(compA[i-1], compA[i]) + 1].emplace_back(i, l+M - r);
    pendingUpdate[std::max(compA[i-1], compA[i]) + 1].emplace_back(i, r - l);
  }
  for (int i = 1; i < N; i++) {
    root[i] = root[i-1];
    for (auto [pos, val] : pendingUpdate[i]) {
      root[i] = update(root[i], pos, val);
    }
  }
}

long long getSolution(int lVil, int rVil, int S, int T) {
  return query(root[lVil], S+1, T);
}

}

void init(int N, int M, std::vector<int> A) {
  ::N = N, ::M = M, ::A = A;
  compressA();
  findQuery2Range::prepare();
  solveNoBlockedRoad::prepare();
  solveBlockedRoad::prepare();
  return;
}

void toggle(int R) {
  int compressedPos = upper_bound(valueOfCompA.begin(), valueOfCompA.end(), R) - valueOfCompA.begin() - 1;
  if (compressedPos < 0) compressedPos = valueOfCompA.size() - 1;
  if (isRoadBlocked.count(R)) {
    blockedRoadSet.erase(blockedRoadSet.find(compressedPos));
    isRoadBlocked.erase(R);
  } else {
    blockedRoadSet.insert(compressedPos);
    isRoadBlocked.insert(R);
  }
  return;
}

long long query(int S, int T) {
  // std::cout << "isRoadBlocked: ";
  // for (auto elem : isRoadBlocked) std::cout << elem << " ";
  // std::cout << ", ";
  // std::cout << A[S] << " -- " << A[T] << ": ";
  long long ans = 0;
  if (blockedRoadSet.empty()) {
    // get the opt solution
    ans = solveNoBlockedRoad::query(S, T);
  } else {
    auto [lVil, rVil] = getQuery2RangeCheckExist(*blockedRoadSet.begin(), S, T);
    if (lVil == -1) {
      ans = -1;
    } else {
      ans = solveBlockedRoad::getSolution(lVil, rVil, S, T);
    }
  }
  return ans;
}
