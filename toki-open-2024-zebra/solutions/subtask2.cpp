#include "zebra.h"

#include <vector>
#include <utility>

int find_root(int N, std::vector<int> U, std::vector<int> V) {
  if (is_zebra({N - 4, N - 3, N - 2}) == 0) return N - 3;
  if (is_zebra({N - 4, N - 2}) == 0) return N - 2;
  if (is_zebra({N - 4, N - 3}) == 0) return N - 1;

  int ans = N - 4;
  int l = 0, r = N - 5;
  while (l <= r) {
    int mid = (l + r) / 2;

    std::vector<int> ask = {mid};
    if ((N - mid) % 2 == 0) {
      ask.push_back(N - 4);
    } else {
      ask.push_back(N - 3);
    }

    if (is_zebra(ask) == 0) {
      ans = mid;
      r = mid - 1;
    } else {
      l = mid + 1;
    }
  }
  return ans;
}
