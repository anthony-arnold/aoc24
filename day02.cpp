#include "day02.hpp"
#include <cmath>
#include <print>
#include <vector>
#include <numeric>
#include <ranges>

namespace aoc {

consteval bool safe(std::vector<std::int64_t> levels) {
   std::adjacent_difference(levels.begin(), levels.end(), levels.begin());
   auto begin = std::next(levels.begin());

   for (auto it = begin; it != levels.end(); ++it) {
      if (std::signbit(*it) != std::signbit(*begin)) {
         return false;
      }
      if (*it > 3 || *it < -3 ||  *it == 0) {
         return false;
      }
   }
   return true;
}

consteval bool find_safe(std::vector<std::int64_t> levels) {
   for (auto it = levels.begin(); it != levels.end(); ++it) {
      std::vector<std::int64_t> attempt { levels.begin(), it };
      std::copy(std::next(it, 1), levels.end(), std::back_inserter(attempt));
      if (safe(attempt)) {
         return true;
      }
   }
   return false;
}

template <std::ranges::viewable_range R>
consteval std::pair<std::int64_t, std::int64_t> count(R&& reports) {
   std::int64_t i = 0;
   std::int64_t j = 0;
   for (auto r : reports) {
      if (safe(r)) {
         i++;
         j++;
      }
      else if (find_safe(r)) {
         j++;
      }
   }
   return {i, j};
}

}


int main() {
   constexpr auto p = aoc::count(aoc::day02());
   static_assert(p.first == 321);
   static_assert(p.second == 386);
}
