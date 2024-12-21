#include "day19.hpp"
#include <ranges>
#include <algorithm>
#include <array>

namespace aoc {

constexpr auto towel_str_v = day19_v.substr(0, day19_v.find('\n'));

constexpr size_t num_towels_v = std::ranges::count(towel_str_v, ',') + 1;

constexpr auto towels() {
   std::array<std::string_view, num_towels_v> twls {};
   auto it = twls.begin();
   for (auto twl : std::views::split(day19_v.substr(0, day19_v.find('\n')), ", "sv)) {
      *it++ = std::string_view(twl);
   }
   std::ranges::sort(twls);
   return twls;
}

constexpr auto towels_v = towels();

constexpr auto size(std::string_view sv) {
   return sv.size();
}

constexpr auto max_v = std::ranges::max(towels_v, {}, size).size();
constexpr auto min_v = std::ranges::min(towels_v, {}, size).size();

constexpr size_t possible(std::string_view sv, auto& memo, size_t h=0) {
   size_t c{};
   if (memo[h]) {
      return *memo[h];
   }

   auto begin = towels_v.begin();
   auto end = towels_v.end();
   size_t i;
   for (i = min_v; begin != end && i <= std::min(sv.size(), max_v); i++) {
      auto r = std::ranges::equal_range(begin, end, sv.substr(0, i), {},
                                        [i](auto t) { return t.substr(0, i); });

      if (r.begin() != towels_v.end()) {
         begin = r.begin();
         end = r.end();
         if (begin != end && sv.starts_with(*begin)) {
            i = begin->size();
            if (sv.size() == begin->size()) {
               c++;
            }
            else {
               c += possible(sv.substr(begin->size()), memo, h+i);
            }
         }
      }
   }

   memo[h] = c;
   return c;
}

constexpr auto solve() {
   size_t count {};
   size_t total {};
   auto start = day19_v.find('\n') + 2;
   auto end = day19_v.find('\n', start);
   while (end != std::string_view::npos) {
      std::array<std::optional<size_t>, 64> memo {};
      auto pattern = day19_v.substr(start, end - start);
      auto num = possible(pattern, memo);
      if (num != 0) {
         count++;
      }
      total += num;
      start = end + 1;
      end = day19_v.find('\n', start);
   }
   return std::make_pair(count, total);
}

constexpr auto soln_v = solve();
constexpr size_t part1_v = soln_v.first;
constexpr size_t part2_v = soln_v.second;

}


int main() {
   static_assert(aoc::part1_v == 267);
   static_assert(aoc::part2_v == 796449099271652);
}
