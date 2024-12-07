#include "day03.hpp"

#include <vector>
#include <optional>
#include <cstdint>

namespace aoc {

consteval std::optional<int> parse(std::string_view sv) {
   int n = 0;
   for (auto c : sv) {
      if (c < '0' || c > '9') {
         return {};
      }

      n *= 10;
      n += static_cast<int>(c - '0');
   }
   return n;
}

consteval std::pair<std::int64_t, std::int64_t> answer() {
   bool on = true;
   std::int64_t part1 = {};
   std::int64_t part2 = {};
   std::string_view input = day03_v;

   while (input.contains(')')) {
      std::string_view::size_type skip = 1;
      if (input.starts_with("do()")) {
         skip = 4;
         on = true;
      }
      else if (input.starts_with("don't()")) {
         skip = 7;
         on = false;
      }
      else if (input.starts_with("mul("))
      {
         skip = 4;
         auto comma = input.find(",", 4);
         if (comma != std::string_view::npos) {
            auto end = input.find(")", comma+1);
            auto x = parse(input.substr(4, comma - 4));
            auto y = parse(input.substr(comma + 1, end - comma - 1));
            if (x && y) {
               auto mul = x.value() * y.value();
               part1 += mul;
               if (on) {
                  part2 += mul;
               }
               skip = end;
            }
         }
      }
      input = input.substr(skip);
   }

   return { part1, part2 };
}

}

int main() {
   static_assert(aoc::answer() == std::make_pair(188116424LL, 104245808LL));
}
