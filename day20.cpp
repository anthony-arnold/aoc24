#include "day20.hpp"
#include <ranges>
#include <algorithm>
#include <array>

namespace aoc {

constexpr auto cols_v = day20_v.find('\n') + 1;
constexpr auto rows_v = std::ranges::count(day20_v, '\n');

constexpr auto start_v = day20_v.find('S');
constexpr auto end_v = day20_v.find('E');

struct node {
   size_t pos {};
   size_t cost {};
};

struct comp {
   constexpr bool operator()(const node& lhs, const node& rhs) const {
      return lhs.cost > rhs.cost;
   }
};

constexpr auto adjacent(size_t n) {
   std::array<size_t, 4> adj {
      n - cols_v,
      n + cols_v,
      n - 1,
      n + 1,
   };

   std::array<size_t, 4> use {};
   size_t b {};

   for (auto a : adj) {
      if (a >= rows_v * cols_v || day20_v[a] == '\n') {
         continue;
      }
      use[b++] = a;
   }
   return std::make_pair(use, b);
}

constexpr auto path() {
   std::array<size_t, cols_v * rows_v> order {};
   std::array<size_t, cols_v * rows_v> cost {};
   size_t i{};
   auto n = start_v;
   auto prev = n;

   while (n != end_v) {
      cost[n] = i;
      order[i++] = n;

      auto [adj, a] = adjacent(n);
      for (auto nb : std::views::take(adj, a)) {
         if (day20_v[nb] != '#' && nb != prev) {
            prev = n;
            n = nb;
            break;
         }
      }
   }
   cost[end_v] = i;
   order[i] = end_v;

   return std::make_pair(cost, order);
}

constexpr auto path_v = path();
constexpr auto costs_v = path_v.first;
constexpr auto order_v = path_v.second;

constexpr size_t good_cheat(auto start, auto end, auto len) {
   auto cost_a = costs_v[start];
   auto cost_b = costs_v[end];

   if (cost_b < cost_a) {
      return 0;
   }

   auto d = cost_b - cost_a;
   if (d < len) {
      return 0;
   }
   if (d - len >= 100) {
      return 1;
   }
   return 0;
}


constexpr auto walk() {
   size_t g1{}, g2{};
   std::array<node, cols_v * rows_v> queue {};
   std::array<size_t, cols_v * rows_v> min_cost {};
   size_t i {};
   while (order_v[i] != end_v) {
      auto n = order_v[i++];

      std::ranges::fill(min_cost, static_cast<size_t>(-1));
      min_cost[n] = 0;
      queue[0] = node { n, 0 };
      size_t size = 1;

      while (size) {
         std::ranges::pop_heap(queue.begin(), queue.begin() + size--, comp{});
         auto m = queue[size];

         if (m.cost > min_cost[m.pos]) {
            continue;
         }

         if (m.cost >= 2 && day20_v[m.pos] != '#') {
            auto g = good_cheat(n, m.pos, m.cost);
            g2 += g;
            if (m.cost == 2) {
               g1 += g;
            }
         }

         if (m.cost < 20) {
            auto [adj, a] = adjacent(m.pos);
            for (auto nb : std::views::take(adj, a)) {
               if (m.cost + 1 < min_cost[nb]) {
                  queue[size++] = node { nb, m.cost + 1 };
                  min_cost[nb] =  m.cost + 1;
                  std::ranges::push_heap(queue.begin(), queue.begin() + size, comp{});
               }
            }
         }
      }
   }
   return std::make_pair(g1, g2);
}

constexpr auto walk_v = walk();

}

int main() {
   static_assert(aoc::walk_v.first == 1365);
   static_assert(aoc::walk_v.second == 986082);
}
