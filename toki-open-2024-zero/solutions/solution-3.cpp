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
  down(i, N) {
    int minimum = A[i];
    down(j, i) { minimum = min(minimum, A[j]); }
    rep(j, 0, i + 1) { A[j] -= minimum; }
  }
  return accumulate(all(A), 0ll);
}
