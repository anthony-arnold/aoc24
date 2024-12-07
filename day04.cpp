
#include "day04.hpp"

#include <algorithm>
#include <ranges>



namespace aoc {

constexpr size_t cols() {
   return day04_v.find('\n') + 1;
}

constexpr size_t cols_v = cols();

constexpr size_t rows() {
   return std::ranges::count(day04_v, '\n');
}

constexpr size_t rows_v = rows();


constexpr std::string_view XMAS = {"XMAS"sv};

template <std::ranges::forward_range R, std::ranges::forward_range H>
consteval size_t search(R&& range, H&& haystack) {
   size_t x {};
   auto sub = std::ranges::subrange(range.begin(), range.end());
   for(x = 0; ; x++) {
      auto found = std::ranges::search(sub, haystack);
      if (found.empty()) {
         break;
      }
      sub = std::ranges::subrange(found.end(), sub.end());
   }
   return x;
}



template <std::ranges::forward_range R, std::ranges::forward_range H>
constexpr size_t forward(R&& r, H&& h) {
   return search(std::forward<R>(r), std::forward<H>(h));
}

template <std::ranges::forward_range R, std::ranges::forward_range H>
constexpr size_t backward(R&& r, H&& h) {
   return search(std::forward<R>(r), std::views::reverse(std::forward<H>(h)));
}

struct counter {
   constexpr counter() : haystack(XMAS) {}

   template <std::ranges::forward_range R>
   constexpr size_t operator()(R&& r) const {
      return forward(r, haystack) + backward(r, haystack);
   }

   std::string_view haystack;
};

enum class direction {
   HORIZONTAL,
   VERTICAL,
   DIAGONAL_L,
   DIAGONAL_R
};

template <direction>
struct direction_ranges {};

template<>
struct direction_ranges<direction::HORIZONTAL> {
   static constexpr std::ranges::single_view value { day04_v };
};

template<>
struct direction_ranges<direction::VERTICAL> {
   static constexpr auto value = std::views::transform(
      std::views::iota(size_t{}, cols_v),
      [](size_t d) {
         return day04_v | std::views::drop(d) | std::views::stride(cols_v);
      });
};


constexpr size_t day04_size_v = std::ranges::size(day04_v);

template <direction D>
concept diagonal = D == direction::DIAGONAL_L || D == direction::DIAGONAL_R;

// round up to the next valid index
template <direction D> requires diagonal<D>
struct valid_diagonal {};

template <>
struct valid_diagonal<direction::DIAGONAL_R> {
   constexpr size_t operator()(size_t d) {
      return d <= cols_v ? d : ((d / cols_v + 1) * cols_v);
   }
};

template <>
struct valid_diagonal<direction::DIAGONAL_L> {
   constexpr size_t operator()(size_t d) {
      return d <= cols_v ? d : ((d / cols_v + 1) * cols_v - 1);
   }
};

// view the input diagonally
template <direction D> requires diagonal<D>
struct view_diagonal {};

template <>
struct view_diagonal<direction::DIAGONAL_R> {
   constexpr auto operator()(size_t d) const {
      return day04_v
            | std::views::drop(d)
            | std::views::stride(cols_v + 1)
            | std::views::take(cols_v - (d % cols_v));
   }
};

template <>
struct view_diagonal<direction::DIAGONAL_L> {
   constexpr auto operator()(size_t d) const {
      return day04_v
         | std::views::drop(d)
         | std::views::stride(cols_v - 1)
         | std::views::take((d % cols_v) + 1);
   }
};

template <direction D, size_t I, size_t... Is>
constexpr auto diagonal_indices(std::index_sequence<Is...>) {
   if constexpr (I >= day04_size_v) {
      return std::index_sequence<Is...> {};
   }
   else {
      constexpr size_t J = valid_diagonal<D>{}(I);
      return diagonal_indices<D, J+1>(std::index_sequence<Is..., J>{});
   }
}

template <direction D, size_t... Is> requires diagonal<D>
constexpr auto diagonal_ranges(std::index_sequence<Is...>) {
   view_diagonal<D> generator {};
   using range_type = decltype(generator(size_t{}));

   return std::array<range_type, sizeof...(Is)> {
      generator(Is)...
   };
}


template <direction D> requires diagonal<D>
constexpr auto diagonal_ranges() {
   return diagonal_ranges<D>(diagonal_indices<D, 0>(std::make_index_sequence<0>{}));
}

template <direction D> requires diagonal<D>
struct direction_ranges<D> {
   static constexpr auto value = diagonal_ranges<D>();
};

template <direction D>
struct accum {
   static constexpr size_t value = std::ranges::fold_left(
      std::views::transform(std::views::all(direction_ranges<D>::value), counter {}),
      size_t{},
      std::plus {});

};

constexpr size_t part2_f() {
   size_t tot {};
   for (auto a = day04_v.find('A', cols_v); a != std::string_view::npos;) {
      auto col = a % cols_v;
      if (col >= 1 && col < cols_v - 1) {
         auto row = a / cols_v;
         if (row < rows_v - 1) {
            auto diagr = std::array {
               day04_v[(row-1) * cols_v + col - 1],
               day04_v[(row+1) * cols_v + col + 1]
            };
            auto diagl = std::array {
               day04_v[(row-1) * cols_v + col + 1],
               day04_v[(row+1) * cols_v + col - 1]
            };

            std::ranges::sort(diagl);
            std::ranges::sort(diagr);

            if (std::ranges::equal(diagl, "MS"sv) && std::ranges::equal(diagr, "MS"sv)) {
               tot++;
            }
         }
      }
      a = day04_v.find('A', a+1);
   }
   return tot;
}

constexpr size_t part2 = part2_f();

template <direction D>
constexpr size_t accum_v = accum<D>::value;
constexpr size_t horizontal_v = accum_v<direction::HORIZONTAL>;
constexpr size_t vertical_v = accum_v<direction::VERTICAL>;
constexpr size_t rdiagonal_v =  accum_v<direction::DIAGONAL_R>;
constexpr size_t ldiagonal_v = accum_v<direction::DIAGONAL_L>;
constexpr size_t part1 = horizontal_v + vertical_v + rdiagonal_v + ldiagonal_v;

}

int main() {
   static_assert(aoc::part1 == 2427);
   static_assert(aoc::part2 == 1900);
}
