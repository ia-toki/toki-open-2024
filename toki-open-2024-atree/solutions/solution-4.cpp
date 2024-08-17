#include "copper.h"

#if not LOCAL
#define NDEBUG 1
#endif
#include <bits/stdc++.h>
using namespace std;

#define rep(i, a, b) for (auto i = a; i < (b); ++i)
#define down(x, a) for (auto x = a; x--;)
#define all(x) begin(x), end(x)
#define sz(x) int(size(x))
#define let auto const

using ll = long long;
using lint = ll;
using pii = pair<int, int>;
using vi = vector<int>;

template <class T>
struct YComb_ {
  T f;
  template <class... A>
  auto operator()(A&&... a) const {
    return f(ref(*this), forward<A>(a)...);
  }
};
template <class T>
YComb_<T> YComb(T f) {
  return {move(f)};
}

long long min_risk(int N, int M, int L, std::vector<int> U, std::vector<int> V,
                   std::vector<int> A, std::vector<int> S) {
  let LOG = [](int N) {
    int ans = 0;
    while ((1 << ans) < N) ++ans;
    return ans;
  }(N);
  vector<vector<int>> posA(N);
  vector<vector<pair<int, int>>> goRgt(M, vector<pair<int, int>>(LOG, {-1, 0}));
  vector<vector<pair<int, int>>> goLft(M, vector<pair<int, int>>(LOG, {-1, 0}));
  rep(i, 0, M) { posA[A[i]].push_back(i); }
  rep(i, 0, M) {
    if (A[i] + 1 < N) {
      let it = lower_bound(all(posA[A[i] + 1]), i);
      if (it != posA[A[i] + 1].end()) {
        goRgt[i][0].first = *it;
        goRgt[i][0].second = 0;
      } else {
        goRgt[i][0].first = *posA[A[i] + 1].begin();
        goRgt[i][0].second = 1;
      }
    }
    if (A[i] - 1 >= 0) {
      let it = lower_bound(all(posA[A[i] - 1]), i);
      if (it != posA[A[i] - 1].end()) {
        goLft[i][0].first = *it;
        goLft[i][0].second = 0;
      } else {
        goLft[i][0].first = *posA[A[i] - 1].begin();
        goLft[i][0].second = 1;
      }
    }
  }
  rep(j, 1, LOG) {
    rep(i, 0, M) {
      if (goRgt[i][j - 1].first != -1) {
        auto [u, d] = goRgt[i][j - 1];
        goRgt[i][j].first = goRgt[u][j - 1].first;
        goRgt[i][j].second = goRgt[u][j - 1].second + d;
      }
      if (goLft[i][j - 1].first != -1) {
        auto [u, d] = goLft[i][j - 1];
        goLft[i][j].first = goLft[u][j - 1].first;
        goLft[i][j].second = goLft[u][j - 1].second + d;
      }
    }
  }
  int current = 0;
  long long ans = 0;
  for (auto s : S) {
    if (A[current] < s) {
      let diff = s - A[current];
      for (int j = LOG - 1; j >= 0; j--) {
        if ((diff >> j) & 1) {
          assert(goRgt[current][j].first != -1);
          ans += goRgt[current][j].second;
          current = goRgt[current][j].first;
        }
      }
    } else if (A[current] > s) {
      let diff = A[current] - s;
      for (int j = LOG - 1; j >= 0; j--) {
        if ((diff >> j) & 1) {
          assert(goLft[current][j].first != -1);
          ans += goLft[current][j].second;
          current = goLft[current][j].first;
        }
      }
    }
  }
  return ans;
}
