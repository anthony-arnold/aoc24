#include "day15.hpp"
#include <ranges>
#include <algorithm>
#include <string>
#include <vector>
#include <print>

namespace aoc {

constexpr auto break_v = std::ranges::search(day15_v, "\n\n"sv);
constexpr auto map_v = std::string_view(day15_v.begin(), break_v.begin());
constexpr auto instr_v = std::string_view(break_v.end(), day15_v.end());
constexpr auto cols_v = map_v.find('\n') + 1;

constexpr auto solve(std::string& map, auto cols) {
   auto pos = map.find('@');
   std::vector next { pos };
   std::array<decltype(pos), 5000> stack {};
   size_t sp {};

   for (char c : instr_v) {
      decltype(pos) shift {};
      switch(c) {
         case '^':
            shift = static_cast<decltype(shift)>(-cols);
            break;
         case 'v':
            shift = cols;
            break;
         case '>':
            shift = 1;
            break;
         case '<':
            shift = static_cast<decltype(shift)>(-1);
            break;
         default:
            continue;
      }

      next.assign(1, pos);
      sp = 0;


      while (!std::ranges::contains(next, '#', [&map](auto n) { return map[n]; })) {
         if (std::ranges::all_of(next, [&map](auto n) { return map[n] == '.'; })) {
            while (sp) {
               auto n = stack[--sp];
               if (map[n]=='.') continue;
               map[n+shift] = map[n];
               map[n] = '.';
            }
            pos += shift;
            break;
         }
         for (auto n : next) {
            stack[sp++] = n;
         }

         for (auto it = next.begin(); it != next.end();) {
            if (map[*it] != '.') {
               auto n = *it;
               auto m = n + shift;
               *it = m;

               if (c != '>' && c != '<') {
                  if (map[m] == '[') {
                     it = next.insert(it+1, m+1);
                  }
                  else if (map[m] == ']') {
                     it = next.insert(it, m-1);
                     ++it;
                  }
               }
               ++it;
            }
            else {
               it = next.erase(it);
            }
         }
      }
   }
}

constexpr auto sum(std::string_view map, char box, auto cols) {
   size_t gps {};
   for (auto [i, c] : std::views::enumerate(map)) {
      if (c==box) {
         auto x = static_cast<decltype(cols)>(i) % cols;
         auto y = static_cast<decltype(cols)>(i) / cols;
         gps += x + y * 100;
      }
   }
   return gps;
}

constexpr auto sim() {
   std::string map(map_v.begin(), map_v.end());
   solve(map, cols_v);
   return sum(map, 'O', cols_v);
}

constexpr auto sim2() {
   std::string map{};
   map.reserve(map_v.size() * 2);
   for (auto c : map_v) {
      switch(c) {
         case '#':
            map += "##";
            break;
         case 'O':
            map += "[]";
            break;
         case '.':
            map += "..";
            break;
         case '@':
            map += "@.";
            break;
         default:
            map += c;
            break;
      }
   }
   solve(map, cols_v*2-1);
   return sum(map, '[', cols_v*2-1);

}

}

int main() {
   static_assert(aoc::sim() == 1515788);
   static_assert(aoc::sim2() == 1516544);
}
