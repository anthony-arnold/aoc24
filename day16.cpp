#include "day16.hpp"
#include <ranges>
#include <algorithm>
#include <array>
#include <vector>

namespace aoc {

enum dir { east, north, west, south };
struct node { size_t pos; dir face; };

constexpr size_t cost[4][4] = {
   { 1, 1001, 2001, 1001 },
   { 1001, 1, 1001, 2001 },
   { 2001, 1001, 1, 1001 },
   { 1001, 2001, 1001, 1 },
};

constexpr auto source = day16_v.find('S');
constexpr auto destin = day16_v.find('E');

constexpr size_t cols_v = day16_v.find('\n') + 1;

constexpr bool blocked(size_t p) {
   return p >= day16_v.size() || day16_v[p] == '#';
}

struct key {
   node n;
   size_t total {};
};

constexpr auto count_visited(auto&& min_cost, auto&& prev) {
   auto shortest = std::ranges::min(std::ranges::subrange(&min_cost[destin*4],
                                                          &min_cost[(destin+1)*4]));
   std::array<size_t, day16_v.size() * 4> stack;
   size_t sp{};
   std::array<bool, day16_v.size()> visit {};
   visit[destin]=true;
   for (size_t i = 0; i < 4; i++) {
      if (min_cost[destin*4 + i] == shortest) {
         stack[sp++] = destin*4 + i;
      }
   }
   while (sp) {
      auto top = stack[--sp];

      for (auto p : prev[top]) {
         visit[p/4] = true;
         stack[sp++] = p;
      }
   }

   return std::ranges::count(visit, true);
}

constexpr void find_paths(auto&& min_cost, auto&& prev) {
   auto comp = [](auto lhs, auto rhs) {
      return rhs.total < lhs.total;
   };

   std::array<key, day16_v.size()*4> queue {
      key { node { source, dir::east }, 0 }
   };
   size_t size = 1;

   while (size > 0) {
      auto k = queue[0];
      std::ranges::pop_heap(queue.begin(), queue.begin() + size, comp);
      --size;

      if (k.total > min_cost[k.n.pos * 4 + static_cast<size_t>(k.n.face)]) {
         continue;
      }

      std::array<node, 4> adj {
         node { k.n.pos - cols_v, dir::north },
         node { k.n.pos + cols_v, dir::south },
         node { k.n.pos - 1, dir::west },
         node { k.n.pos + 1, dir::east }
      };

      for (auto n : adj) {
         if (blocked(n.pos)) {
            continue;
         }
         auto t = k.total + cost[k.n.face][n.face];
         auto cidx = n.pos * 4 + static_cast<size_t>(n.face);
         if (t < min_cost[cidx]) {
            queue[size++] = key { n, t };
            std::ranges::push_heap(queue.begin(), queue.begin() + size, comp);
            min_cost[cidx] = t;
            prev[cidx].clear();
         }

         if (t == min_cost[cidx]) {
            prev[cidx].push_back(k.n.pos*4 + static_cast<size_t>(k.n.face));
         }
      }
   }
}

constexpr auto path() {
   std::array<size_t, day16_v.size()*4> min_cost {};
   std::ranges::fill(min_cost, static_cast<size_t>(-1));
   min_cost[source * 4] = 0;

   std::array<std::vector<size_t>, day16_v.size()*4> prev {};

   find_paths(min_cost, prev);

   auto shortest = std::ranges::min(std::ranges::subrange(&min_cost[destin*4],
                                                          &min_cost[(destin+1)*4]));

   auto visited = count_visited(min_cost, prev);

   return std::make_tuple(shortest, visited);
}

constexpr auto answer = path();

}

int main() {
   static_assert(std::get<0>(aoc::answer) == 75416);
   static_assert(std::get<1>(aoc::answer) == 476);
}
