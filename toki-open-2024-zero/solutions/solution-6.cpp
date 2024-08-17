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
  int minimum = *min_element(all(A));
  down(i, N) {
    int cur_min = *min_element(begin(A), begin(A) + i + 1);
    if (cur_min > minimum) {
      int diff = cur_min - minimum;
      rep(j, 0, i + 1) A[j] -= diff;
    }
  }
  rep(i, 0, N) {
    int cur_min = *min_element(begin(A) + i, end(A));
    if (cur_min > minimum) {
      int diff = cur_min - minimum;
      rep(j, i, N) A[j] -= diff;
    }
  }

  // Subtask 5 Solution
  int intervals = *min_element(all(A));
  rep(i, 0, N) A[i] -= intervals;

  while (intervals > 0) {
    array<int, 4> best = {0, 0, 0};  // (length, count, left, right)
    rep(i, 0, N) {
      if (A[i] == 0) {
        continue;
      }
      int j = i;
      while (j + 1 < N && A[j + 1] > 0) {
        j++;
      }
      int count = *min_element(begin(A) + i, begin(A) + j + 1);
      int length = j - i + 1;
      best = max(best, {length, count, i, j});
      i = j;
    }
    auto [length, count, left, right] = best;
    if (length == 0) {
      break;
    }
    count = min(count, intervals);
    intervals -= count;
    rep(i, left, right + 1) { A[i] -= count; }
  }
  return accumulate(all(A), 0ll);
}
