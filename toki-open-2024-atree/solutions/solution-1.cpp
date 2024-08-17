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
  vector<int> posA(N);
  rep(i, 0, N) { posA[A[i]] = i; }
  vector<int> pR(N), pL(N);
  rep(i, 1, N) {
    if (posA[i - 1] > posA[i]) {
      pL[i] = pL[i - 1];
      pR[i] = pR[i - 1] + 1;
    } else {
      pL[i] = pL[i - 1] + 1;
      pR[i] = pR[i - 1];
    }
  }
  long long ans = 0;
  int current = 0;
  for (auto s : S) {
    if (current < s) {
      ans += pR[s] - pR[current];
    } else if (current > s) {
      ans += pL[current] - pL[s];
    }
    current = s;
  }
  return ans;
}
