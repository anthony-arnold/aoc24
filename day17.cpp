#include "day17.hpp"
#include <ranges>
#include <algorithm>
#include <array>

namespace aoc {

constexpr auto num(auto it) {
   size_t n {};
   while (*it >= '0' && *it <= '9') {
      n *= 10;
      n += static_cast<size_t>(*it - '0');
      ++it;
   }
   return n;
}

constexpr size_t prog_size_v = std::ranges::count(day17_v, ',') + 1;

constexpr auto parse() {
   auto it = day17_v.find(':') + 2;
   auto a = num(&day17_v[it]);

   it = day17_v.find(':', it) + 2;
   auto b = num(&day17_v[it]);

   it = day17_v.find(':', it) + 2;
   auto c = num(&day17_v[it]);

   it = day17_v.find(':', it) + 2;

   std::array<size_t, prog_size_v> prog {};
   for (auto& i : prog) {
      i = num(&day17_v[it]);
      it += 2;
   }

   return std::make_tuple(a, b, c, prog);
}

constexpr auto parsed_v = parse();

constexpr size_t a_v = std::get<0>(parsed_v);
constexpr size_t b_v = std::get<1>(parsed_v);
constexpr size_t c_v = std::get<2>(parsed_v);
constexpr auto prog_v = std::get<3>(parsed_v);

constexpr auto run(size_t a = a_v) {
   size_t ip {};
   size_t b {b_v};
   size_t c {c_v};
   std::array<size_t, 128> output {};
   std::ranges::fill(output, static_cast<size_t>(-1));
   size_t op {};

   auto combo = [&a, &b, &c](auto operand) {
      switch(operand) {
         case 4:
         return a;
         case 5:
         return b;
         case 6:
         return c;
      }
      return operand;
   };

   while (ip < prog_v.size()) {
      auto opcode = prog_v[ip];
      auto operand = prog_v[ip+1];

      switch(opcode) {
         case 0:
            a = a / (1 << combo(operand));
            break;
         case 1:
            b = b ^ operand;
            break;
         case 2:
            b = combo(operand) % 8;
            break;
         case 3:
            if (a != 0) {
               ip = operand;
               continue;
            }
            break;
         case 4:
            b = b ^ c;
            break;
         case 5:
            output[op++] = combo(operand) % 8;
            break;
         case 6:
            b = a / (1 << combo(operand));
            break;
         case 7:
            c = a / (1 << combo(operand));
            break;
      }
      ip += 2;
   }

   return output;
}

constexpr auto answer_v = run();
constexpr auto answer_end_v = std::ranges::find(answer_v, static_cast<size_t>(-1));
constexpr auto answer_size_v = std::distance(answer_v.begin(), answer_end_v);

constexpr auto part1() {
   std::array<size_t, answer_size_v> vals {};
   std::ranges::copy(std::views::take(answer_v, answer_size_v), vals.begin());
   return vals;
}

constexpr auto part1_v = part1();

constexpr auto search(long length, size_t a) {
   auto expect = std::views::drop(prog_v, static_cast<long>(prog_v.size()) - length);
   auto out = run(a);
   auto end = std::ranges::find(out, static_cast<size_t>(-1));
   auto size = std::distance(out.begin(), end);

   if (size == length) {
      auto res = std::views::take(out, size);
      auto sub = std::ranges::search(expect, res);
      if (sub.begin() != sub.end()) {
         if (length == prog_v.size()) {
            return a;
         }
         for (size_t i {}; i<8; i++) {
            auto ok = search(length+1, a*8+i);
            if (ok) {
               return ok;
            }
         }
      }
   }
   return size_t{};
}

constexpr size_t start_search() {
   for (size_t i{}; i < 8; i++) {
      auto ok = search(1, i);
      if (ok) {
         return ok;
      }
   }
   return {};
}

}

int main() {
   static_assert(aoc::part1_v == std::array<size_t, aoc::answer_size_v> {7,3,0,5,7,1,4,0,5});
   static_assert(aoc::start_search() == 202972175280682);
}
