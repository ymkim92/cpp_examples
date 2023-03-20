#include <tuple>
#include <iostream>

template <typename ... Ts, std::size_t ... Is>
std::tuple<Ts...> sumT (std::tuple<Ts...> const & t1,
                        std::tuple<Ts...> const & t2,
                        std::index_sequence<Is...> const &)
 { return { (std::get<Is>(t1) + std::get<Is>(t2))... }; }

template <typename ... Ts>
std::tuple<Ts...> operator+ (std::tuple<Ts...> const & t1,
                             std::tuple<Ts...> const & t2)
 { return sumT(t1, t2, std::make_index_sequence<sizeof...(Ts)>{}); }

int main ()
 {
   std::tuple<int,double> t = std::make_tuple(1,2);
   t = {3, 2};
   auto t1 = t + t;
   std::cout << std::get<0>(t1) << std::endl;
   std::cout << std::get<1>(t1) << std::endl;
 }