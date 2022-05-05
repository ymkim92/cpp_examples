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
    // std::srand(std::time(nullptr));
    // int random = rand() % Total;
    // auto input = static_cast<Fruit>(random);

    // switch (input) {
    //     case Banana: cout << enum_str[Banana] << endl; break;
    //     case Coconut: cout << enum_str[Coconut] << endl; break;
    //     case Mango: cout << enum_str[Mango] << endl; break;
    //     case Carambola: cout << enum_str[Carambola] << endl; break;
    //     case Total: cout << "Incorrect ingredient!" << endl; break;
    // }

    cout << EnumNames::enum_str[EnumNames::Fruit::Mango] << endl;
    return EXIT_SUCCESS;
}
