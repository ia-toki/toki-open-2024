#include "buffet.h"

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

long long min_leftover(int N, std::vector<int> A) {
  if (N == 1) {
    return 0;
  }
  vector<int> D(N - 1);
  rep(i, 0, N - 1) D[i] = A[i + 1] - A[i];
  int cntLeft = A[0], cntRight = A[N - 1];
  down(i, N - 1) {
    if (D[i] < 0) {
      // Do operation [0, i]
      int cnt = min(cntLeft, -D[i]);
      cntLeft -= cnt;
      rep(j, 0, i + 1) A[j] -= cnt;
    }
  }
  rep(i, 0, N - 1) {
    if (D[i] > 0) {
      // Do operation [i + 1, N - 1]
      int cnt = min(cntRight, D[i]);
      cntRight -= cnt;
      rep(j, i + 1, N) A[j] -= cnt;
    }
  }
  return accumulate(all(A), 0ll);
}
