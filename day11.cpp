#include "day11.hpp"
#include <tuple>
#include <cmath>

#include <print>
namespace aoc {


constexpr size_t digits(size_t I) {
   if (I==0) {
      return 1;
   }

   size_t j {};
   for (; I != 0; j++, I /= 10) {}
   return j;
}

template <size_t I>
constexpr size_t digits_v = digits(I);

template <size_t I>
constexpr auto split() {
   size_t x { I };
   size_t y {};
   size_t z {1};
   for (size_t j = 0; j < digits_v<I> / 2; j++) {
      y += z * (x % 10);
      x /= 10;
      z *= 10;
   }

   return std::make_tuple(x, y);
}

template <size_t I>
constexpr size_t left_v = std::get<0>(split<I>());

template <size_t I>
constexpr size_t right_v = std::get<1>(split<I>());

template <size_t I, bool Even = digits_v<I>%2==0>
struct split_t {};

template <size_t I>
struct split_t<I, true> {
   using type = std::index_sequence<left_v<I>, right_v<I>>;
   static constexpr type value {};
};

template <size_t I, typename = void>
struct evolve {
   static constexpr std::index_sequence<I * 2024> value {};
};

template <>
struct evolve<0> {
   static constexpr std::index_sequence<1> value {};
};

template <size_t I>
struct evolve<I, std::void_t<typename split_t<I>::type>> : split_t<I>
{
};

template <size_t Times, size_t I>
struct length {
private:

   template <size_t... Is>
   constexpr static size_t apply(std::index_sequence<Is...>) {
      return (length<Times - 1, Is>::value + ...);
   }

public:
   constexpr static size_t value = apply(evolve<I>::value);

};

template <size_t I>
struct length<0, I> {
   constexpr static size_t value = 1;
};

template <size_t Times, size_t... Is>
constexpr auto blink(std::index_sequence<Is...>) {
   return (length<Times, Is>::value + ...);
}

constexpr auto part1_v = aoc::blink<25>(aoc::day11_v);
constexpr auto part2_v = aoc::blink<75>(aoc::day11_v);
}


int main() {
   static_assert(aoc::part1_v == 182081);
   static_assert(aoc::part2_v == 216318908621637);
}
