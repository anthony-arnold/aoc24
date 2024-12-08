#include "day08.hpp"
#include <ranges>
#include <algorithm>

namespace aoc {

constexpr long width_v = day08_v.find('\n') + 1;
constexpr long height_v = std::ranges::count(day08_v, '\n');

using coord_t = std::pair<long, long>;

constexpr coord_t twodee(size_t i) {
   auto x1 = i % width_v;
   auto y1 = i / width_v;
   return { x1, y1 };
}

constexpr bool inside(coord_t x) {
   return x.first >= 0 && x.first < width_v-1 && x.second >= 0 && x.second < height_v;
}

constexpr long gcd(long a, long b) {
   while (b != 0) {
      long c = a % b;
      a = b;
      b = c;
   }
   return a;
}

constexpr long check() {
   std::array<bool, width_v * height_v> antinodes {};
   size_t i = 0;
   for(;;) {
      auto j = day08_v.find_first_not_of(".\n", i);
      if (j == std::string_view::npos) {
         break;
      }

      antinodes[j] = true;
      char c = day08_v[j];
      auto k = j+1;
      for(;;) {
         k = day08_v.find(c, k);
         if (k == std::string_view::npos) {
            break;
         }
         antinodes[k] = true;
         auto x1 = twodee(j);
         auto x2 = twodee(k);
         auto dx = coord_t(x2.first - x1.first, x2.second - x1.second);
         auto g = gcd(dx.first, dx.second);
         dx.first /= g;
         dx.second /= g;

         auto fwd = coord_t(x2.first + dx.first, x2.second + dx.second);
         while (inside(fwd)) {
            antinodes[static_cast<size_t>(fwd.second*width_v + fwd.first)] = true;
            fwd.first += dx.first;
            fwd.second += dx.second;
         }

         auto back = coord_t(x1.first - dx.first, x1.second - dx.second);
         while (inside(back)) {
            antinodes[static_cast<size_t>(back.second*width_v + back.first)] = true;
            back.first -= dx.first;
            back.second -= dx.second;
         }

         k++;
      }

      i = j+1;
   }
   return std::ranges::count(antinodes, true);
}

}

int main() {
   static_assert(aoc::check() == 861);
}
