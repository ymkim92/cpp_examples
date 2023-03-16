#include <utility>
#include "tuple_helper.h"

template <typename...T>
std::tuple<T...>& operator &= (std::tuple<T...>& lhs, const std::tuple<T...>& rhs)
{
    internal::and_rhs_to_lhs(lhs, rhs, std::index_sequence_for<T...>{});
    return lhs;
}

template <typename...T>
std::tuple<T...> operator & (std::tuple<T...> lhs, const std::tuple<T...>& rhs)
{
   return lhs &= rhs;
}