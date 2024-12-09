#include "day09.hpp"
#include <ranges>
#include <vector>
#include <algorithm>

namespace aoc {

constexpr auto chunked_v = std::views::chunk(day09_v, 2);
constexpr size_t num_files_v = chunked_v.size();

constexpr auto blocks() {
   size_t id {};
   size_t blk {};
   std::array<std::array<size_t, 4>, num_files_v> chunks {};
   for (auto [size, free] : chunked_v | std::views::transform([](auto p) {
      return std::make_pair(
         static_cast<size_t>(p[0] - '0'),
         p[1] == '\n' ? 0 : static_cast<size_t>(p[1] - '0'));
   }))
   {
      chunks[id++] = std::array<size_t, 4> { size, blk, blk+size, free };
      blk += free + size;
   }
   return chunks;
}

constexpr auto blocks_v = blocks();

constexpr size_t defrag() {
   auto blk = blocks_v;
   auto rit = blk.end() - 1;
   auto it = blk.begin();

   size_t csum {};
   size_t loc = 0;
   while (it != blk.end()) {
      size_t id = static_cast<size_t>(std::distance(blk.begin(), it));
      auto& [size, _0, _00, free] = *it;

      size_t summa = (loc+size) * (loc+size-1) / 2 -
         loc * (loc-1) / 2;
      csum += summa * id;
      loc += size;

      while (rit > it && free > 0) {
         auto& [rsize, _1, _11, _111] = *rit;

         size_t rid = static_cast<size_t>(std::distance(blk.begin(), rit));
         auto fill = std::min(free, rsize);

         summa = (loc+fill) * (loc+fill-1) / 2 -
            loc * (loc-1) / 2;
         csum += summa * rid;
         loc += fill;
         free -= fill;
         rsize -= fill;
         if (rsize == 0) {
            rit--;
         }
      }
      it++;
   }

   return csum;
}

constexpr size_t max_free(auto&& r) {
   return std::ranges::max(r, {}, [](auto p) {
      return p[3];
   })[3];
}

constexpr size_t compress() {
   auto blk = blocks_v;
   size_t csum {};
   size_t max_free_ = max_free(blk);

   for (auto rit = blk.end() - 1; rit > blk.begin(); --rit) {
      size_t id = static_cast<size_t>(std::distance(blk.begin(), rit));
      auto& [rsize, loc, _0, _00] = *rit;

      size_t bid = loc;
      if (rsize <= max_free_) {
         for (auto it = blk.begin(); it != rit; ++it) {
            auto& [size, _1, off, free] = *it;
            if (free >= rsize) {
               bool rescan = free == max_free_;
               bid = off;
               free -= rsize;
               off += rsize;
               if (rescan) {
                  max_free_ = max_free(std::ranges::subrange(blk.begin(), rit));
               }
               break;
            }
         }
      }

      size_t summa = (bid+rsize) * (bid+rsize-1) / 2 -
         bid * (bid-1) / 2;
      csum += summa * id;
   }

   return csum;
}

}

int main() {
   static_assert(aoc::defrag() == 6307275788409);
   static_assert(aoc::compress() == 6327174563252);
}
