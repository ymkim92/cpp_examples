#include <iostream>
#include <string>
#include <string_view>
#include <cstring> // For std::strlen
 
void print(std::string s)
{
  std::cout << s << '\n';
}

int main()
{
    std::string_view sv{"balloon"};
    sv.remove_suffix(3);

    // print(sv); // compile error: won't implicitly convert

    std::string str_s{sv};                 // explicit conversion
    print(str_s);                          // okay
    print(static_cast<std::string>(sv)); // okay
    std::cout << std::strlen(str_s.c_str()) << '\n';

    std::string_view str{"balloon"};

    std::cout << str << '\n';

    // We use std::strlen because it's simple, this could be any other function
    // that needs a null-terminated string.
    // It's okay to use data() because we haven't modified the view, and the
    // string is null-terminated.
    std::cout << std::strlen(str.data()) << '\n';

    return 0;
}