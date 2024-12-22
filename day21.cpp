#include "day21.hpp"
#include <ranges>
#include <algorithm>
#include <array>

namespace aoc {

constexpr auto num_codes_v = std::ranges::count(day21_v, '\n');

constexpr auto codes() {
   std::array<std::string_view, num_codes_v> v {};
   size_t n {};
   for (size_t i = 0; i < num_codes_v; i++) {
      auto m = day21_v.find('\n', n);
      v[i] = day21_v.substr(n, m-n);
      n = m+1;
   }
   return v;
}

constexpr auto codes_v = codes();

constexpr std::pair<int, int> diff(auto from, auto to) {
   auto dx = to.first - from.first;
   auto dy = to.second - from.second;

   if (dx < 0) dx = -1;
   if (dy < 0) dy = -1;
   if (dx > 0) dx = 1;
   if (dy > 0) dy = 1;

   return { dx, dy };
}

constexpr std::pair<int, int> next(auto dx, auto dy) {
   if (dx == 0) {
      switch(dy) {
         case -1:
            return { -1, 0 };
         case 0:
            return { 0, 0 };
         default:
            return { -1, 1 };
      }
   }
   else {
      switch(dx) {
         case -1:
            return { -2, 1 };
         default:
            return { 0, 1 };
      }
   }
}


template <size_t D>
struct move {
   move<D-1> next_bot {};

   std::array<std::optional<size_t>, 36*36> memory {};

   constexpr std::optional<size_t> recall(auto from, auto to, std::optional<size_t> set = {}) {
      auto x1 = from.first + 3;
      auto y1 = from.second + 3;

      auto x2 = to.first + 3;
      auto y2 = to.second + 3;

      auto v1 = y1 * 6 + x1;
      auto v2 = y2 * 6 + x2;

      auto w = v1*36 + v2;

      if (set) {
         memory[w] = set;
      }
      return memory[w];
   }

   constexpr auto operator()(auto from, auto to) {
      auto [dx, dy] = diff(from, to);

      auto r = recall(from, to);
      if (r) {
         return *r;
      }

      auto pdx = std::make_pair(dx, 0);
      auto pdy = std::make_pair(0, dy);
      auto tox = [to](auto pos) {
         return std::make_pair(to.first, pos.second);
      };
      auto toy = [to](auto pos) {
         return std::make_pair(pos.first, to.second);
      };

      auto a1 = attempt(from, toy, tox, pdy, pdx);
      auto a2 = attempt(from, tox, toy, pdx, pdy);

      return *recall(from, to, std::min(a1, a2));
   }

   constexpr auto attempt(auto from, auto to1, auto to2, auto d1, auto d2) {
      auto pos = from;
      auto bot = std::pair<int, int> {};

      auto to = to1(pos);
      auto a = walk(pos, bot, to, d1.first, d1.second);
      if (a == static_cast<size_t>(-1)) {
         return a;
      }

      to = to2(pos);
      auto b = walk(pos, bot, to, d2.first, d2.second);
      if (b == static_cast<size_t>(-1)) {
         return b;
      }

      a += b + next_bot(bot, std::pair<int, int>{});
      return a;
   }

   constexpr auto walk(auto& pos, auto& bot, auto to, auto dx, auto dy) {
      size_t i {};
      while (pos != to) {
         auto n = next(dx, dy);
         i += next_bot(bot, n);
         pos.first += dx;
         pos.second += dy;
         if (pos.first == -2 && pos.second == 0) {
            return static_cast<size_t>(-1);
         }
         bot = n;
      }
      return i;
   }
};

template <>
struct move<0> {
   constexpr auto operator()(auto from, auto to) const {
      return 1;
   }
};

constexpr auto complexity(std::string_view code, auto& mover) {
   size_t n {};
   size_t s {};
   std::pair<int, int> pos {};

   for (auto c : code) {
      if (c >= '0' && c <= '9') {
         n *= 10;
         n += static_cast<size_t>(c - '0');
      }

      std::pair<int, int> next {};
      switch(c) {
         case '0':
            next.first = -1;
            break;
         case '1':
            next.first = -2;
            next.second = -1;
            break;
         case '2':
            next.first = -1;
            next.second = -1;
            break;
         case '3':
            next.second = -1;
            break;
         case '4':
            next.first = -2;
            next.second = -2;
            break;
         case '5':
            next.first = -1;
            next.second = -2;
            break;
         case '6':
            next.second = -2;
            break;
         case '7':
            next.first = -2;
            next.second = -3;
            break;
         case '8':
            next.first = -1;
            next.second = -3;
            break;
         case '9':
            next.second = -3;
            break;
      }

      s += mover(pos, next);
      pos = next;
   }

   return n*s;
}

constexpr auto part1() {
   move<3> mover{};
   auto cmplx = std::views::transform(codes_v, [&](auto code) {
      return complexity(code, mover);
   });
   return std::ranges::fold_left(cmplx, size_t{}, std::plus {});
}

constexpr auto part2() {
   move<26> mover{};
   auto cmplx = std::views::transform(codes_v, [&](auto code) {
      return complexity(code, mover);
   });
   return std::ranges::fold_left(cmplx, size_t{}, std::plus {});
}

}

int main() {
   static_assert(aoc::part1() == 138764);
   static_assert(aoc::part2() == 169137886514152);
}
