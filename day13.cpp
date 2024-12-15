#include "day13.hpp"
#include <ranges>
#include <algorithm>
#include <cstdint>

namespace aoc {

struct machine {
   std::pair<int64_t, int64_t> btna;
   std::pair<int64_t, int64_t> btnb;
   std::pair<int64_t, int64_t> prize;
};

constexpr size_t num_mach_v = (std::ranges::count(day13_v, '\n') + 1) / 4;

constexpr int64_t read_num(size_t idx) {
   int64_t n{};
   while (idx < day13_v.size() && day13_v[idx] >= '0' && day13_v[idx] <= '9') {
      n *= 10;
      n += static_cast<int64_t>(day13_v[idx] - '0');
      idx++;
   }
   return n;
}

constexpr auto read_button(size_t idx) {
   auto ax = read_num(idx + 4);
   idx = day13_v.find(',', idx);
   auto ay = read_num(idx + 4);
   return std::make_pair(ax, ay);
}

constexpr std::array<machine, num_mach_v> machines() {
   std::array<machine, num_mach_v> mach {};
   size_t i {};

   for (auto idx = day13_v.find(':'); idx != std::string_view::npos;) {
      auto ba = read_button(idx);
      idx = day13_v.find(':', idx+1);
      auto bb = read_button(idx);

      idx = day13_v.find('=', idx+1);
      auto x = read_num(idx+1);

      idx = day13_v.find('=', idx+1);
      auto y = read_num(idx+1);

      mach[i++] = machine { ba, bb, std::make_pair(x, y) };

      idx = day13_v.find(':', idx);
   }
   return mach;
}


constexpr auto machines_v = machines();

constexpr int64_t det(int64_t a, int64_t b, int64_t c, int64_t d) {
   auto t = a * d - b * c;
   if (t < 0) {
      return -t;
   }
   return t;
}

constexpr std::optional<std::pair<int64_t, int64_t>> solve(size_t i, int64_t f) {
   const machine& mch = machines_v[i];

   auto d = det(mch.btna.first, mch.btnb.first, mch.btna.second, mch.btnb.second);
   if (d == 0) {
      return {};
   }

   auto da = det(mch.prize.first+f, mch.btnb.first, mch.prize.second+f, mch.btnb.second);
   auto db = det(mch.prize.first+f, mch.btna.first, mch.prize.second+f, mch.btna.second);
   auto a = da/d;
   auto b = db/d;
   if (a*mch.btna.first + b*mch.btnb.first != mch.prize.first+f ||
       a*mch.btna.second + b*mch.btnb.second != mch.prize.second+f)
   {
      return {};
   }
   return std::make_pair(a, b);
}

constexpr int64_t sum(int64_t f) {
   return std::ranges::fold_left(
      std::views::iota(size_t{}, num_mach_v),
      int64_t{},
      [f](int64_t s, size_t i) {
         auto a = solve(i, f);
         if (a) {
            s += a->first*3 + a->second;
         }
         return s;
      });
}

}

int main() {
   static_assert(aoc::sum(0) == 37686);
   static_assert(aoc::sum(10000000000000) == 77204516023437);
}
