#include "zebra.h"

#include <vector>
#include <utility>

int find_root(int N, std::vector<int> U, std::vector<int> V) {
  std::vector<int> ask(N - 1);
  for (int i = 0; i < N - 1; ++i) ask[i] = i;
  if (is_zebra(ask) == 0) return 0;

  int l = 0, r = N - 1;
  while (l + 1 < r) {
    int mid = (l + r) / 2;

    std::vector<int> ask;
    for (int i = mid; i < r; ++i) ask.push_back(i);
    if (ask.size() < 2) {
      if (0 < l) ask.push_back(0);
      if (r < N - 1) ask.push_back(N - 2);
    }

    if (is_zebra(ask) == 1) {
      l = mid;
    } else {
      r = mid;
    }
  }

  return l + 1;
}
