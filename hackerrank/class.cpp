#include <iostream>
#include <sstream>
using namespace std;

/*
Enter code for class Student here.
Read statement for specification.
*/

class Student
{
private:
    int age;
    int standard;
    string first_name, last_name;
public:
    int get_age() { return age; }
    void set_age(int a_age) { age = a_age; }

    int get_standard() { return standard; }
    void set_standard(int a_standard) { standard = a_standard; }

    string get_first_name() { return first_name; }
    void set_first_name(string a_first_name) { first_name = a_first_name; }

    string get_last_name() { return last_name; }
    void set_last_name(string a_last_name) { last_name = a_last_name; }

    string to_string()
    {
        stringstream ss;
        ss << age << "," << first_name << "," << last_name << "," << standard << endl;
        return ss.str();
    }
};

int main() {
    int age, standard;
    string first_name, last_name;
    
    cin >> age >> first_name >> last_name >> standard;
    
    Student st;
    st.set_age(age);
    st.set_standard(standard);
    st.set_first_name(first_name);
    st.set_last_name(last_name);
    
    cout << st.get_age() << "\n";
    cout << st.get_last_name() << ", " << st.get_first_name() << "\n";
    cout << st.get_standard() << "\n";
    cout << "\n";
    cout << st.to_string();
    
    return 0;
}