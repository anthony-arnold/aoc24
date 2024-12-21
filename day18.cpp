#include "day18.hpp"
#include <ranges>
#include <algorithm>
#include <array>

namespace aoc {

constexpr size_t cols_v = 71;
constexpr size_t rows_v = 71;

constexpr auto num(size_t i) {
   size_t n {};
   while (day18_v[i] >= '0' && day18_v[i] <= '9') {
      n *= 10;
      n += static_cast<size_t>(day18_v[i] - '0');
      i++;
   }
   return n;
}

constexpr auto grid() {
   std::array<std::optional<size_t>, cols_v * rows_v> map {};

   size_t t {};
   size_t i {};
   while (i != std::string_view::npos) {
      auto x = num(i);
      i = day18_v.find(',', i) + 1;
      auto y = num(i);
      i = day18_v.find('\n', i);
      i = day18_v.find_first_of("0123456789", i);

      map[x * cols_v + y] = t++;
   }
   return map;
}

constexpr auto grid_v = grid();

struct node {
   size_t pos {};
   size_t cost {};
};

constexpr auto dest = (rows_v - 1) * cols_v + (cols_v - 1);

constexpr auto solve(auto comp, auto blocked) {
   std::array<size_t, cols_v * rows_v> min_cost {};
   std::ranges::fill(min_cost, static_cast<size_t>(-1));
   min_cost[0] = 0;

   std::array<node, cols_v * rows_v> queue {
      node {}
   };

   size_t size {1};
   while (size) {
      std::ranges::pop_heap(queue.begin(), queue.begin() + size--, comp);
      auto n = queue[size];
      if (n.pos == dest) {
         break;
      }
      if (n.cost > min_cost[n.pos]) {
         continue;
      }

      std::array<node, 4> adj = {};
      size_t a {};
      if (n.pos < (cols_v - 1) * rows_v) {
         adj[a++] = node { n.pos + cols_v, n.cost + 1 };
      }
      if (n.pos >= cols_v) {
         adj[a++] = node { n.pos - cols_v, n.cost + 1 };
      }
      if (n.pos % cols_v != 0) {
         adj[a++] = node { n.pos - 1, n.cost + 1 };
      }
      if (n.pos % cols_v != cols_v-1) {
         adj[a++] = node { n.pos + 1, n.cost + 1 };
      }

      for (auto nb : std::views::take(adj, static_cast<long>(a))) {
         if (!blocked(nb.pos) && nb.cost < min_cost[nb.pos]) {
            min_cost[nb.pos] = nb.cost;
            queue[size++] = nb;
            std::ranges::push_heap(queue.begin(), queue.begin() + size, comp);
         }
      }
   }
   return min_cost[dest];
}

constexpr bool compare(const node& lhs, const node& rhs) {
   return lhs.cost > rhs.cost;
}

constexpr auto part1_v = solve(compare, [](size_t pos) {
   return grid_v[pos] && *grid_v[pos] < 1024;
});

constexpr auto find() {
   size_t p = 0;
   for (size_t i {0}; ; i++) {
      size_t nl = day18_v.find('\n', p);
      if (i > 1024) {
         auto cost = solve(compare, [i](size_t pos) {
            return grid_v[pos] && *grid_v[pos] <= i;
         });
         if (cost == static_cast<size_t>(-1)) {
            return day18_v.substr(p, nl-p);
         }
      }
      p = nl+1;
   }
}

}

int main () {
   using namespace aoc;
   static_assert(aoc::part1_v == 314);
   static_assert(aoc::find() == "15,20"sv);
}
