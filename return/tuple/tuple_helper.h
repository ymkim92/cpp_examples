#include <tuple>

namespace internal
{
    //see: https://stackoverflow.com/a/16387374/4181011
    template<typename T, size_t... Is>
    void and_rhs_to_lhs(T& t1, const T& t2, std::integer_sequence<size_t, Is...>)
    {
        auto l = { (std::get<Is>(t1) &= std::get<Is>(t2), 0)... };
        (void)l; // prevent unused warning
    }
}

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

template <typename ... Ts, std::size_t ... Is>
std::tuple<Ts...> sumT (std::tuple<Ts...> const & t1,
                        std::tuple<Ts...> const & t2,
                        std::index_sequence<Is...> const &)
 { return { (std::get<Is>(t1) + std::get<Is>(t2))... }; }

template <typename ... Ts>
std::tuple<Ts...> operator+ (std::tuple<Ts...> const & t1,
                             std::tuple<Ts...> const & t2)
 { return sumT(t1, t2, std::make_index_sequence<sizeof...(Ts)>{}); }
