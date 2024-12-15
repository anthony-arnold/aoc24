#include "day12.hpp"
#include <ranges>
#include <algorithm>

namespace aoc {

constexpr size_t cols_v = day12_v.find('\n')+1;
constexpr size_t size_v = day12_v.size();

constexpr bool match(size_t j, size_t i) {
   return i < day12_v.size() && day12_v[j] == day12_v[i];
}


constexpr auto flood() {
   std::array<bool, size_v> visited {true};
   std::array<size_t, size_v> stack {0};
   size_t sp {1};
   size_t total1 {};
   size_t total2 {};
   size_t peri {};
   size_t area {};
   size_t edges {};

   for (;;) {
      auto top = stack[--sp];
      area++;

      std::array<size_t, 4> adj = {
         top-1,
         top+1,
         top-cols_v,
         top+cols_v
      };
      std::array<bool, 4> matches {};
      std::ranges::transform(adj,
                             matches.begin(),
                             [top](auto a) {
                                return match(top, a);
                             });

      peri += 4;
      for (auto [a,m] : std::views::zip(adj, matches)) {
         if (m) {
            if (!visited[a]) {
               stack[sp++] = a;
               visited[a] = true;
            }
            peri--;
         }
      }

      if (!matches[0]) {
         edges += !matches[2];
         edges += !matches[3];
      }
      if (!matches[1]) {
         edges += !matches[2];
         edges += !matches[3];
      }

      if (matches[0] && matches[3]) {
         edges += !match(top, top+cols_v-1);
      }
      if (matches[0] && matches[2]) {
         edges += !match(top, top-cols_v-1);
      }
      if (matches[1] && matches[3]) {
         edges += !match(top, top+cols_v+1);
      }
      if (matches[1] && matches[2]) {
         edges += !match(top, top-cols_v+1);
      }

      if (sp == 0) {
         if (day12_v[top] != '\n') {
            total1 += area * peri;
            total2 += area * edges;
         }
         auto it = std::ranges::find(visited, false);
         if (it == visited.end()) {
            break;
         }
         size_t idx = static_cast<size_t>(std::distance(visited.begin(), it));
         peri = 0;
         area = 0;
         edges = 0;
         visited[idx] = true;
         stack[sp++] = idx;
      }
   }
   return std::make_pair(total1, total2);
}

constexpr auto flood_v = flood();
constexpr size_t part1_v = flood_v.first;
constexpr size_t part2_v = flood_v.second;

}

int main() {
   static_assert(aoc::part1_v == 1456082);
   static_assert(aoc::part2_v == 872382);
}
