#include "day22.hpp"
#include <ranges>
#include <algorithm>
#include <array>

namespace aoc {

constexpr auto num_secrets_v = std::ranges::count(day22_v, '\n');

constexpr auto secrets() {
   std::array<size_t, num_secrets_v> sec {};

   size_t x {};
   size_t i {};
   for (auto j = day22_v.find('\n'); j != std::string_view::npos; j = day22_v.find('\n', i)) {
      size_t n {};
      for (auto c : day22_v.substr(i, j - i)) {
         n *= 10;
         n += static_cast<size_t>(c - '0');
      }
      sec[x++] = n;
      i = j + 1;
   }
   return sec;
}

constexpr auto secrets_v = secrets();

constexpr auto mix(size_t i, size_t j) {
   return i ^ j;
}

constexpr auto prune(size_t i) {
   return i % 16777216;
}

constexpr auto next(size_t i) {
   i = prune(mix(i, i*64));
   i = prune(mix(i, i/32));
   return prune(mix(i, i*2048));
}

constexpr auto sequence(size_t i) {
   std::array<size_t, 2000> seq {};
   for (size_t j = 0; j < 2000; j++) {
      i = next(i);
      seq[j] = i;
   }
   return seq;
}

template <size_t I>
constexpr auto sequence_v = sequence(secrets_v[I]);

template <size_t... I>
constexpr auto sum_2000(std::index_sequence<I...>) {
   return (sequence_v<I>.back() + ...);
}

constexpr auto part1() {
   return sum_2000(std::make_index_sequence<num_secrets_v> {});
}

}

//#include <print>
int main() {
   //std::print("{}", aoc::part1());
   static_assert(aoc::part1() == 13429191512);
}
