#include "day10.hpp"
#include <ranges>
#include <algorithm>
#include <queue>
#include <unordered_set>

namespace aoc {

constexpr size_t cols_v = day10_v.find('\n') + 1;
//constexpr size_t rows_v = std::ranges::count(day10_v, '\n');

constexpr auto trailheads() {
   return std::views::enumerate(day10_v)
   | std::views::filter([](auto p) { return std::get<1>(p)=='0'; })
      | std::views::transform([](auto p) { return static_cast<size_t>(std::get<0>(p)); });
}

constexpr size_t position(size_t row, size_t col) {
   return row * cols_v + col;
}

constexpr std::array<size_t, 4> adjacent(size_t n) {
   size_t row = n / cols_v;
   size_t col = n % cols_v;

   return {
      position(row+1, col),
      position(row-1, col),
      position(row, col+1),
      position(row, col-1)
   };
}

constexpr auto search(size_t i) {
   static const size_t szrb = day10_v.size();
   std::array<size_t, szrb> q {i};
   size_t begin {};
   size_t end {1};

   std::array<bool, szrb> visited {};
   visited[i] = true;

   size_t nines{};

   while (begin != end) {
      size_t n = q[begin];
      begin = (begin + 1) % szrb;

      char c = day10_v[n];
      if (c == '9') {
         nines++;
         continue;
      }

      for(size_t a : adjacent(n)) {
         if (a < day10_v.size()) {
            char d = day10_v[a];
            if (d == c + 1 && !visited[a]) {
               visited[a] = true;
               q[end] = a;
               end = (end + 1) % szrb;
            }
         }
      }
   }

   return nines;
}

constexpr auto search2(size_t i) {
   static const size_t szrb = day10_v.size();
   std::array<size_t, szrb> stack {i};
   size_t sp = 1;

   std::array<size_t, szrb> nines {};
   while (sp) {
      auto n = stack[--sp];
      char c = day10_v[n];
      if (c == '9') {
         nines[n]++;
         continue;
      }

      for(size_t a : adjacent(n)) {
         if (a < day10_v.size()) {
            char d = day10_v[a];
            if (d == c + 1) {
               stack[sp++] = a;
            }
         }
      }
   }
   return std::ranges::fold_left(nines, size_t{}, std::plus {});
}

constexpr auto answer(auto&& f) {
   return std::ranges::fold_left(
      std::views::transform(trailheads(), f),
      size_t{},
      std::plus{});
}

constexpr size_t part1_v = answer(search);
constexpr size_t part2_v = answer(search2);

}

int main() {
   static_assert(aoc::part1_v == 538);
   static_assert(aoc::part2_v == 1110);
}
