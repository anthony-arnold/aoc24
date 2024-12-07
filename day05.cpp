#include "day05.hpp"
#include <ranges>
#include <algorithm>
#include <vector>

namespace aoc {

constexpr auto split = day05_v.rfind("\n\n");
constexpr auto rules = day05_v.substr(0, split);
constexpr auto lists = day05_v.substr(split+2);

constexpr size_t num_rules_v = std::ranges::count(rules, '|');

template <size_t From>
constexpr size_t next_pipe_v = rules.find('|', From);

template <typename R>
constexpr size_t number(R&& sv) {
   return static_cast<size_t>((sv[0] - '0') * 10 + (sv[1] - '0'));
}

constexpr auto parse_rules() {
   std::array<std::pair<size_t, size_t>, num_rules_v> arr;
   size_t start{};
   auto end = rules.find('\n');
   size_t i = 0;
   while (end != std::string_view::npos) {
      size_t n = 10 * static_cast<size_t>(rules[start] - '0') +
         static_cast<size_t>(rules[start+1] - '0');
      size_t m = 10 * static_cast<size_t>(rules[start+3] - '0') +
         static_cast<size_t>(rules[start+4] - '0');

      arr[i++] = { n, m };
      start = end + 1;
      end = rules.find('\n', start);
   }
   return arr;
}

constexpr auto rules_v = parse_rules();

template <size_t I>
constexpr auto list_end_v = lists.find('\n', I);

template <size_t I>
constexpr size_t list_size_v = std::ranges::count(lists.substr(I, list_end_v<I>-I), ',') + 1;

template <size_t I>
constexpr std::array<size_t, list_size_v<I>> next_list() {
   auto arr = std::array<size_t, list_size_v<I>>{};
   size_t k = 0;
   auto start = I;
   auto stop = lists.find_first_of(",\n", I);
   for(;;) {
      size_t n = 0;
      for (auto i : std::views::iota(start, stop)) {
         n *= 10;
         n += static_cast<size_t>(lists[i] - '0');
      }
      arr[k++] = n;
      if (lists[stop] == '\n') {
         break;
      }
      start = stop + 1;
      stop = lists.find_first_of(",\n", start);
   }
   return arr;
}

template <size_t I>
constexpr auto list_nums_v = next_list<I>();

template <size_t I>
constexpr auto next_list_start_v = list_end_v<I> + 1;

template <size_t I>
constexpr bool has_next_list_v = next_list_start_v<I> < std::ranges::size(lists);

template <size_t I>
constexpr bool valid_v = std::ranges::is_sorted(list_nums_v<I>, [](auto l, auto r) {
   return std::ranges::find(rules_v, std::make_pair(l, r)) != rules_v.end();
});

template <size_t I>
constexpr auto sorted() {
   auto copy = list_nums_v<I>;
   std::ranges::sort(copy, [](auto l, auto r) {
      return std::ranges::find(rules_v, std::make_pair(l, r)) != rules_v.end();
   });
   return copy;
}

template <size_t I>
constexpr auto sorted_v = sorted<I>();


template <size_t I>
constexpr std::pair<size_t, size_t> answer() {
   size_t j {}, k {};
   if constexpr (valid_v<I>) {
      j = list_nums_v<I>[list_nums_v<I>.size() / 2];
   }
   else {
      k = sorted_v<I>[sorted_v<I>.size() / 2];
   }

   if constexpr (has_next_list_v<I>) {
      auto [m, n] = answer<next_list_start_v<I>>();
      j += m;
      k += n;
   }
   return { j, k };
}

constexpr auto answer_v = answer<0>();
constexpr size_t part1_v = answer_v.first;
constexpr size_t part2_v = answer_v.second;


}


int main() {
   static_assert(aoc::part1_v == 5091);
   static_assert(aoc::part2_v == 4681);
}
