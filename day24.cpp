#include "day24.hpp"
#include <array>
#include <algorithm>
#include <ranges>
#include <vector>

namespace aoc {

constexpr auto num(char c) {
   if (c >= 'a' && c <= 'z') {
      return static_cast<size_t>(c - 'a');
   }
   else {
      return static_cast<size_t>(26 + (c - '0'));
   }
}

constexpr auto hash(std::string_view sv) {
   return 36*36 * num(sv[0]) + 36 * num(sv[1]) + num(sv[2]);
}

constexpr void set(auto& v, auto& g, auto a, auto b, auto op, auto c) {
   if (v[c]) {
      return;
   }

   if (!v[a] && g[a]) {
      std::apply([&](auto aa, auto ab, auto aop) {
         set(v, g, aa, ab, aop, a);
      }, *g[a]);
   }
   if (!v[b] && g[b]) {
      std::apply([&](auto ba, auto bb, auto bop) {
         set(v, g, ba, bb, bop, b);
      }, *g[b]);
   }

   if (!v[a] || !v[b]) {
      return;
   }

   if (op.starts_with("AND"sv)) {
      v[c] = *v[a] & *v[b];
   }

   else if (op.starts_with("OR"sv)) {
      v[c] = *v[a] | *v[b];
   }

   else if (op.starts_with("XOR"sv)) {
      v[c] = *v[a] ^ *v[b];
   }
}

constexpr auto init() {
   std::array<std::optional<bool>, 36*36*36> v = {};
   std::array<std::optional<std::tuple<size_t, size_t, std::string_view>>, 26*36*36> gates {};

   for (size_t i{}; i != std::string_view::npos;) {
      auto c = day24_v.find(':', i);
      if (c == std::string_view::npos) {
         auto sp = day24_v.find(' ', i+4);
         auto a = hash(day24_v.substr(i));
         auto op = day24_v.substr(i+4, sp);
         auto b = hash(day24_v.substr(sp + 1));
         auto c = hash(day24_v.substr(sp + 8));

         gates[c] = std::make_tuple(a, b, op);
      }
      else {
         v[hash(day24_v.substr(i))] = '1' ==  day24_v[c+2];
      }
      auto nl = day24_v.find('\n', i);
      i = day24_v.find_first_of("abcdefghijklmnopqrstuvwxyz", nl);
   }

   return std::make_pair(v, gates);
}

constexpr auto init_v = init();
constexpr auto init_values_v = init_v.first;
constexpr auto gates_v = init_v.second;

constexpr auto values(auto v, auto gates) {
   for (auto i { hash("z00"sv) }; i <= hash("z99"sv); ++i) {
      if (gates[i]) {
         auto g = *gates[i];
         set(v, gates, std::get<0>(g), std::get<1>(g), std::get<2>(g), i);
      }
   }

   return v;
}

constexpr auto values_v = values(init_values_v, gates_v);

constexpr auto sum(auto values, auto begin, auto end) {

   size_t n {};
   for (auto i { hash(end) }; i >= hash(begin); i--) {
      if (!values[i]) continue;
      n <<= 1;
      n |= *values[i];
   }
   return n;
}

constexpr auto part1_v = sum(values_v, "z00"sv, "z46"sv);

constexpr auto expected_v = sum(values_v, "x00"sv, "x45"sv) + sum(values_v, "y00"sv, "y45"sv);

constexpr auto z00_v = hash("z00"sv);
constexpr auto x00_v = hash("x00"sv);
constexpr auto y00_v = hash("y00"sv);
constexpr auto zmax_v = hash("z45"sv);
constexpr auto xmax_v = hash("x44"sv);
constexpr auto ymax_v = hash("y44"sv);

constexpr auto invalid() {
   std::vector<size_t> inv {};
   std::vector<size_t> xors {};
   std::vector<size_t> ands {};
   for (size_t i = 0; i < gates_v.size(); i++) {
      auto g = gates_v[i];
      if (!g) {
         continue;
      }

      auto [a,b,op] = *g;
      if (a > b) {
         std::swap(a, b);
      }

      if (i >= z00_v && i <= zmax_v) {
         if (i < zmax_v && !op.starts_with("XOR")) {
            inv.push_back(i);
         }
      }
      else {
         if (a < x00_v || a > xmax_v || b < y00_v || b > ymax_v) {
            if (op.starts_with("XOR")) {
               inv.push_back(i);
               continue;
            }
         }

         if (a != x00_v && b != y00_v) {
            if (op.starts_with("XOR")) {
               xors.push_back(i);
            }

            if (op.starts_with("AND")) {
               ands.push_back(i);
            }
         }
      }
   }

   std::ranges::sort(xors);
   std::ranges::sort(ands);

   for (size_t i = 0; i < gates_v.size(); i++) {
      auto g = gates_v[i];
      if (!g) {
         continue;
      }

      auto [a,b,op] = *g;
      if (op.starts_with("XOR")) {
         auto f = std::ranges::lower_bound(xors, a);
         if (f != xors.end() && *f == a) {
            xors.erase(f);
         }
         f = std::ranges::lower_bound(xors, b);
         if (f != xors.end() && *f == b) {
            xors.erase(f);
         }
      }
      else if (op.starts_with("OR")) {
         auto f = std::ranges::lower_bound(ands, a);
         if (f != ands.end() && *f == a) {
            ands.erase(f);
         }
         f = std::ranges::lower_bound(ands, b);
         if (f != ands.end() && *f == b) {
            ands.erase(f);
         }
      }
   }

   std::array<size_t, 8> group {};
   std::ranges::copy(inv, group.begin());
   std::ranges::copy(xors, group.begin() + inv.size());
   std::ranges::copy(ands, group.begin() + inv.size() + xors.size());
   std::ranges::sort(group);
   return group;
}

}

#include <print>

int main() {
using namespace aoc;
   static_assert(part1_v == 61495910098126);
   auto g = invalid();

   for (auto i : g) {
      char a = 'a' + i / (36*36);
      char b = 'a' + (i % (36*36)) / 36;
      char c = 'a' + (i % 36);
      if (b < 'a' || b > 'z') {
         b = '0' + (i % (36*36)) / 36 - 26;
      }
      if (c < 'a' || c > 'z') {
         c = '0' + (i % 36) - 26;
      }
      std::print(",{}{}{}", a, b, c);
   }
   std::println();
}
