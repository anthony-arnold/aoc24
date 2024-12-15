#include "day14.hpp"
#include <ranges>
#include <algorithm>
namespace aoc {


struct bot {
   long x[2];
   long dx[2];
};

constexpr size_t num_bots_v = std::ranges::count(day14_v, '\n');

constexpr long read_num(size_t i) {
   long n = 0;
   long f = 1;
   if (day14_v[i] == '-') {
      i++;
      f = -1;
   }

   while (day14_v[i] >= '0' && day14_v[i] <= '9') {
      n *= 10;
      n += static_cast<long>(day14_v[i] - '0');
      i++;
   }
   return n * f;
}

constexpr auto read() {
   size_t j{};
   std::array<bot, num_bots_v> bots {};

   auto i = day14_v.find('=');
   while (i != std::string_view::npos) {
      auto& b = bots[j++];
      b.x[0] = read_num(i+1);
      i = day14_v.find(',', i);
      b.x[1] = read_num(i+1);
      i = day14_v.find('=', i);
      b.dx[0] = read_num(i+1);
      i = day14_v.find(',', i);
      b.dx[1] = read_num(i+1);
      i = day14_v.find('=', i);
   }

   return bots;
}

constexpr auto bots_v = read();

constexpr long p_x = 101;
constexpr long p_y = 103;

constexpr auto modulo(auto n, auto d) {
   while (n < 0) {
      n += d;
   }
   return n % d;
}

constexpr auto project(const bot& b, long sec) {
   auto x = modulo(b.x[0] + b.dx[0] * sec, p_x);
   auto y = modulo(b.x[1] + b.dx[1] * sec, p_y);
   return std::make_pair(x, y);
}

constexpr auto safety(long sec) {
   std::array<size_t, 4> saf {};
   for (const auto& bot : bots_v) {
      auto [x, y] = project(bot, sec);
      if (x == p_x/2 || y == p_y/2) {
         continue;
      }

      size_t q{};
      if (y > p_y/2) {
         q++;
      }
      if (x > p_x/2) {
         q += 2;
      }
      saf[q]++;
   }
   return saf;
}

constexpr auto sim(long sec) {
   return std::ranges::fold_left(safety(sec), size_t{1}, std::multiplies {});
}


constexpr long tree() {
   for (long sec {1}; ; sec++) {
      std::array<int, p_x * p_y> grid {};
      for (const auto& b : bots_v) {
         auto [x,y] = project(b, sec);
         grid[static_cast<size_t>(y*p_x+x)]++;
      }
      if (std::ranges::all_of(grid, [](int i) { return i < 2; })) {
         return sec;
      }
   }
}

}

int main() {
   static_assert(aoc::sim(100) == 220971520);
   static_assert(aoc::tree() == 6355);
}
