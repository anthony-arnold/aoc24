#include "day06.hpp"

#include <tuple>
#include <ranges>
#include <algorithm>
#include <cstring>

namespace aoc {

constexpr size_t num_cols_v = day06_v.find('\n') + 1;
constexpr auto start_v = day06_v.find('^');
constexpr auto day06_p = day06_v.data();
constexpr auto day06_size_v = day06_v.size();

enum class DIR { UP, RIGHT, DOWN, LEFT };

constexpr DIR next_dir(DIR d) {
   return static_cast<DIR>((static_cast<int>(d) + 1) % 4);
}

constexpr size_t next_pos(DIR D, size_t P) {
   switch (D) {
      case DIR::UP:
         return P - num_cols_v;
      case DIR::DOWN:
         return P + num_cols_v;
      case DIR::LEFT:
         return P - 1;
      case DIR::RIGHT:
         return P + 1;
   }
}

constexpr int mask(DIR d) {
   return 1 << static_cast<int>(d);
}

constexpr bool gone(size_t p) {
   return p >= day06_size_v || day06_p[p] == '\n';
}

constexpr bool loop(DIR d, size_t ob, size_t p, std::array<int, day06_size_v> q) {
   int* path = q.data();
   auto m = mask(d);
   path[p] |= m;

   size_t step = next_pos(d, p);
   while (!gone(step)) {
      if (day06_p[step] == '#' || step == ob) {
         d = next_dir(d);
         m = mask(d);
      }
      else {
         if (path[step] & m) {
            return true;
         }
         p = step;
      }
      path[p] |= m;
      step = next_pos(d, p);
   }
   return false;
}

constexpr auto walk() {
   std::array<int, day06_size_v> pt = {};
   auto path = pt.data();
   std::array<std::optional<bool>, day06_size_v> lp = {};
   auto loops = lp.data();

   DIR d = DIR::UP;
   size_t p = start_v;

   auto m = mask(d);
   path[p] |= m;
   size_t step = next_pos(d, p);
   do {
      if (day06_p[step] == '#') {
         d = next_dir(d);
         m = mask(d);
      }
      else {
         if (!loops[step] && step != start_v) {
            loops[step] = loop(next_dir(d), step, p, pt);
         }
         p = step;
      }

      path[p] |= m;
      step = next_pos(d, p);
   } while (!gone(step));

   return std::make_pair(pt, lp);
}

constexpr auto walk_v = walk();
constexpr auto part1_v = std::ranges::count_if(std::get<0>(walk_v), [](int i) { return i != 0; });
constexpr auto part2_v = std::ranges::count(std::get<1>(walk_v), true);

}

int main() {
   static_assert(aoc::part1_v == 5461);
   static_assert(aoc::part2_v == 1836);
}
