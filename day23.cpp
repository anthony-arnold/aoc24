#include "day23.hpp"
#include <array>
#include <vector>
#include <ranges>
#include <algorithm>

namespace aoc {

constexpr auto hash(std::string_view sv) {
   return static_cast<size_t>(sv[0] - 'a') * 26 + static_cast<size_t>(sv[1] - 'a');
}

constexpr auto BK(auto&& g, auto&& r, auto&& p, auto&& x) {
   std::vector<std::vector<size_t>> f {};
   if (p.empty() && x.empty()) {
      std::ranges::sort(r);
      f.emplace_back(std::move(r));
      return f;
   }

   for (auto node : p) {
      auto r2 = r;
      r2.push_back(node);

      std::vector<size_t> p2 = p;
      std::erase_if(p2, [&](auto n) {
         return !std::ranges::binary_search(g[node], n);
      });

      std::vector<size_t> x2 = x;
      std::erase_if(x2, [&](auto n) {
         return !std::ranges::binary_search(g[node], n);
      });

      for (auto fc : BK(g, std::move(r2), std::move(p2), std::move(x2))) {
         f.emplace_back(std::move(fc));
      }

      std::erase(p, node);
      x.push_back(node);
   }

   return f;
}

constexpr auto answer() {
   size_t n3 {};
   std::array<std::vector<size_t>, 26*26> g {};
   std::vector<size_t> v {};
   std::vector<std::vector<size_t>> edges{};

   auto split = std::views::split(day23_v, '\n');
   for (auto sub : split) {
      auto line = std::string_view(sub.data());
      if (line.empty()) {
         continue;
      }
      auto a = hash(line);
      auto b = hash(line.substr(3));
      if (a > b) {
         std::swap(a, b);
      }
      edges.emplace_back(std::vector { a, b });

      auto it = std::ranges::lower_bound(g[a], b);
      if (it == g[a].end() || *it != b) {
         g[a].insert(it, b);
      }

      it = std::ranges::lower_bound(g[b], a);
      if (it == g[b].end() || *it != a) {
         g[b].insert(it, a);
      }

      it = std::ranges::lower_bound(v, a);
      if (it == v.end() || *it != a) {
         v.insert(it, a);
      }
      it = std::ranges::lower_bound(v, b);
      if (it == v.end() || *it != b) {
         v.insert(it, b);
      }
   }
   std::ranges::sort(edges);

   std::vector<std::vector<size_t>> clique3 {};
   for (auto it = edges.begin(); it != edges.end() - 1; ++it) {
      for (auto jt = it + 1; jt != edges.end(); ++jt) {
         if (jt->front() > it->back()) {
            break;
         }

         std::vector<size_t> set {};
         std::ranges::set_symmetric_difference(*it, *jt, std::back_inserter(set));
         if (set.size() == 2 && std::ranges::binary_search(edges, set)) {
            set.clear();
            std::ranges::set_union(*it, *jt, std::back_inserter(set));
            auto kt = std::ranges::lower_bound(clique3, set);
            if (kt == clique3.end() || *kt != set) {
               clique3.insert(kt, set);
            }
         }
      }
   }
   for (auto set : clique3) {
      if (std::ranges::any_of(set, [](auto t) {
         return t / 26 == 19;
      })) {
         n3++;
      }
   }

   auto cliques = BK(g, std::vector<size_t> {}, v, std::vector<size_t> {});
   auto max = std::ranges::max_element(cliques, {}, [](auto&& c) { return c.size(); });
   auto size = max->size();
   std::array<size_t, 26*26> names {};
   std::ranges::copy(*max, names.begin());
   return std::make_tuple(n3, size, names);
}

}

#include <print>
int main() {
   // N.B. I couldn't find a way to construct the string in-compiler.
   auto [n3, nm, names] = aoc::answer();

   std::string pw {};
   for (size_t i{}; i < nm; i++) {
      if (!pw.empty()) {
         pw += ",";
      }
      pw += ('a' + names[i] / 26);
      pw += ('a' + names[i] % 26);
   }
   std::println("{} {}", n3, pw);
}
