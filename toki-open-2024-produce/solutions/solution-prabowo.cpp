#include "purwokerto.h"

#include <vector>

const int kMod = 1000000007;

struct Day {
  int type, count;
  Day(int _type, int _count): type(_type), count(_count) {}
};

std::pair<long long, int> purwokerto(int N, int M, std::vector<int> T) {
  std::vector<Day> days;
  for (int i = 0; i < M; ++i) {
    days.emplace_back(T[i], 1);
  }

  auto normalize = [&](std::vector<Day> &days) {
    // Remove count 0
    std::vector<Day> newDays;
    for (int i = 0; i < days.size(); ++i) {
      if (days[i].count != 0) {
        newDays.push_back(days[i]);
      }
    }
    days.swap(newDays);

    // Merge consecutive days
    newDays.clear();
    for (int i = 0; i < days.size(); ) {
      int j = i;
      int count = 0;
      while (j < days.size() && days[j].type == days[i].type) {
        count += days[j++].count;
      }
      newDays.emplace_back(days[i].type, count);
      i = j;
    }
    days.swap(newDays);

    if (!days.empty() && days.back().type != 1) {
      days.pop_back();
    }
    if (!days.empty() && days[0].type != 0) {
      days.erase(days.begin());
    }
  };

  long long max = 0;
  int count = 1;
  for (int i = 0; i < N; ++i) {
    normalize(days);
    for (int j = 0; j < days.size(); j += 2) {
      max += N - i;
      count = 1LL * days[j].count * days[j + 1].count % kMod * count % kMod;
      --days[j + 0].count;
      --days[j + 1].count;
    }
  }

  return {max, count};
}
