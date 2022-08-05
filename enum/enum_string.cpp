#include <iostream>
#include <string>
#include <chrono>

// SystemThreadNames::threadNames[SystemThreadNames::STN_$1]

using std::cout; using std::cin;
using std::endl; using std::string;

class EnumNames {
public:
    enum Fruit { Banana, Coconut, Mango, Carambola, Total } ;
    static const constexpr char* const enum_str[] =
            { "Banana Pie", "Coconut Tart", "Mango Cookie", "Carambola Crumble" };
};

int main(){
    cout << EnumNames::enum_str[EnumNames::Fruit::Mango] << endl;
    return EXIT_SUCCESS;
}
