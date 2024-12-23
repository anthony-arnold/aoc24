#include "day22.hpp"
#include <ranges>
#include <algorithm>
#include <array>
#include <vector>

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
   std::array<size_t, 2001> seq {i};
   for (size_t j = 1; j <= 2000; j++) {
      i = next(i);
      seq[j] = i;
   }
   return seq;
}

constexpr auto sequences() {
   std::array<std::array<size_t, 2001>, num_secrets_v> seqs {};
   for (auto i = 0; i < num_secrets_v; i++) {
      seqs[i] = sequence(secrets_v[i]);
   }
   return seqs;
}

constexpr auto sequences_v = sequences();

constexpr auto part1() {
   return std::ranges::fold_left(
      sequences_v,
      size_t {},
      [](size_t sum, const auto& s) {
         return sum + s.back();
      });
}

constexpr auto hash(size_t i, size_t j) {
   size_t h {};
   for (size_t k = 1; k < 5; k++) {
      auto d = static_cast<int>(sequences_v[i][j+k] % 10) -
         static_cast<int>(sequences_v[i][j+k-1] % 10);
      h *= 19;
      h += d + 9;
   }
   return h;
}

constexpr auto part2() {
   std::vector<size_t> ch(130321);
   std::vector<bool> chi;

   for (size_t i = 0; i < num_secrets_v; i++) {
      chi.assign(130321, false);

      for (size_t j = 0; j < 1997; j++) {
         size_t h {hash(i, j)};
         if (!chi[h]) {
            chi[h] = true;
            ch[h] += sequences_v[i][j+4] % 10;
         }
      }
   }
   return std::ranges::max(ch);
}

}

int main() {
   static_assert(aoc::part1() == 13429191512);
   static_assert(aoc::part2() == 1582);
}
