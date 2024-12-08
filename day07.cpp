#include "day07.hpp"
#include <ranges>
#include <algorithm>

namespace aoc {

constexpr size_t num_eq_v = std::ranges::count(day07_v, '\n');

constexpr std::array<size_t, num_eq_v> starts() {
   std::array<size_t, num_eq_v> arr {};
   auto s = day07_v.find("\n");

   for (size_t j {1}; j < num_eq_v; s = day07_v.find("\n", s+1), j++) {
      arr[j] = s + 1;
   }
   return arr;
}

template <size_t Start, size_t End>
constexpr size_t num() {
   size_t n{};
   for (size_t i {Start}; i != End; ++i) {
      n *= 10;
      n += static_cast<size_t>(day07_v[i] - '0');
   }
   return n;
}

template <size_t Start, size_t End>
constexpr size_t num_v = num<Start, End>();

constexpr auto starts_v = starts();

template <size_t Start>
constexpr size_t numnums() {
   auto c = day07_v.find(":", Start);
   auto n = day07_v.find("\n", c);

   size_t i{};
   auto s = day07_v.find(" ", c);
   while (s < n) {
      i++;
      s = day07_v.find(" ", s+1);
   }
   return i;
}

template <size_t Start>
constexpr size_t numnums_v = numnums<Start>();

template <size_t Start, size_t I, size_t N>
constexpr void rhs(std::array<size_t, N>& out) {
   if constexpr (I < N) {
      out[I] = num_v<Start, day07_v.find_first_of(" \n", Start)>;
      rhs<day07_v.find(" ", Start) + 1, I+1, N>(out);
   }
}

template <size_t Start>
constexpr auto eqn() {
   size_t res = num_v<Start, day07_v.find(":", Start)>;
   std::array<size_t, numnums_v<Start>> arr;
   rhs<day07_v.find(" ", Start) + 1, 0, numnums_v<Start>>(arr);

   return std::make_tuple(res, arr);
}

template <size_t Start>
constexpr auto eqn_v = eqn<Start>();

constexpr size_t concat (size_t x, size_t y) {
   size_t z {y};
   while (z) {
      x *= 10;
      z /= 10;
   }
   return x + y;
}

template <size_t I, size_t N>
constexpr size_t eval(const std::array<int, N>& mask) {
   auto t = std::get<1>(eqn_v<starts_v[I]>);
   size_t r = t.front();
   size_t i = 1;
   for (auto it = mask.begin(); i < t.size(); ++i, ++it) {
      size_t term = t[i];
      switch (*it) {
         case 0:
            r += term;
            break;
         case 1:
            r *= term;
            break;
         case 2:
            r = concat(r, term);
            break;
      }
   }
   return r;
}

template <size_t N>
constexpr std::array<int, N> trimask_full() {
   std::array<int, N> mask {};
   std::ranges::fill(mask, 2);
   return mask;
}

template <size_t N>
constexpr std::array<int, N> trimask_full_v = trimask_full<N>();

template <size_t N>
constexpr bool is_full(const std::array<int, N>& mask) {
   return mask == trimask_full_v<N>;
}

template <size_t N>
constexpr void increment(std::array<int, N>& mask) {
   mask[0]++;
   for (size_t i{0}; mask[i] == 3; ++i) {
      mask[i] = 0;
      mask[i+1]++;
   }
}

struct answer_t { size_t a1, a2; };
constexpr answer_t operator+(answer_t lhs, answer_t rhs) {
   return {lhs.a1+rhs.a1, lhs.a2+rhs.a2};
}

template <size_t I>
constexpr size_t res_v = std::get<0>(eqn_v<starts_v[I]>);

template <size_t I>
constexpr answer_t solve2() {
   size_t part1 {};
   size_t part2 {};

   std::array<int, numnums_v<starts_v[I]>-1> trinary_mask {};
   for (;;) {
      if (eval<I>(trinary_mask) == res_v<I>) {
         part2 = res_v<I>;
         if (std::ranges::find(trinary_mask, 2) == trinary_mask.end()) {
            part1 = res_v<I>;
            break;
         }
      }
      if (is_full(trinary_mask)) {
         break;
      }
      increment(trinary_mask);
   }
   return { part1, part2 };
}

template <size_t I>
constexpr auto soln2_v = solve2<I>();

template <size_t... Is>
constexpr auto soln2s(std::index_sequence<Is...>) {
   return (soln2_v<Is> + ...);
}

constexpr auto answer() {
   return soln2s(std::make_index_sequence<num_eq_v>{});
}

constexpr answer answers_v = answer();
constexpr size_t part1_v = answers_v.a1;
constexpr size_t part2_v = answers_v.a2;

}

#include <print>
int main () {
   static_assert(aoc::part1_v == 1289579105366);
   static_assert(aoc::part2_v == 92148721834692);
}
