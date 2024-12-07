#include "day01.hpp"
#include <print>
#include <algorithm>
#include <vector>
#include <array>
#include <ranges>
#include <numeric>
#include <unordered_set>

namespace aoc {

consteval auto difference(auto&& r)  {
   auto diff = [](auto pair) {
      auto [a, b] = pair;
      std::int64_t c = a - b;
      if (c < 0) {
         return -c;
      }
      return c;
   };

   return std::views::transform(r, diff) | std::ranges::to<std::vector>();
}

consteval std::pair<std::int64_t, std::int64_t> sum(auto&& r) {
   auto a1 = std::views::transform(r, [](auto pair) { return pair.first; })
      | std::ranges::to<std::vector>();
   auto a2 = std::views::transform(r, [](auto pair) { return pair.second; })
      | std::ranges::to<std::vector>();
   std::ranges::sort(a1);
   std::ranges::sort(a2);

   std::int64_t p1 = std::ranges::fold_left(difference(std::views::zip(a1, a2)),
                                            0,
                                            std::plus{});

   std::int64_t p2 = 0;
   for (auto [i, _] : r) {
      auto [begin, end] = std::ranges::equal_range(a2, i);
      p2 += i * std::distance(begin, end);
   }

   return { p1, p2 };
}

}

int main() {
   constexpr auto answer = aoc::sum(aoc::day01());
   static_assert(answer.first == 1882714);
   static_assert(answer.second == 19437052);
   std::println("{} {}", answer.first, answer.second);
}
