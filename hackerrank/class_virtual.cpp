// input
/*
4
1
Walter 56 99
2
Jesse 18 50 48 97 76 34 98
2
Pinkman 22 10 12 0 18 45 50
1
White 58 87
*/

#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;

class Person
{
public:
    string name;
    int age;
    virtual void getdata();
    virtual void putdata();
};

void Person::getdata()
{
    cin >> this->name >> this->age;
}

void Person::putdata()
{
}

class Professor: public Person
{
private:
    int publications;
    static int id;
    int cur_id;
public:
    Professor() {this->cur_id = ++id;}
    virtual void getdata();
    virtual void putdata();
};

int Professor::id = 0;

void Professor::getdata()
{
    cin >> this->name >> this->age >> this->publications;
}

void Professor::putdata()
{
    cout << this->name << " " << this->age << " " << this->publications << " " << this->cur_id << endl;
}

class Student: public Person
{
private:
    int marks[6];
    int marksSum;
    static int id;
    int cur_id;
public:
    Student():marksSum(0) {this->cur_id = ++id;}
    virtual void getdata();
    virtual void putdata();
};

int Student::id = 0;

void Student::getdata()
{
    cin >> this->name >> this->age;
    for (int i=0; i<6; i++)
    {
        cin >> this->marks[i];
        this->marksSum += this->marks[i];
    }
}

void Student::putdata()
{
    cout << this->name << " " << this->age << " " << this->marksSum << " " << this->cur_id << endl;
}

int main(){

    int n, val;
    cin>>n; //The number of objects that is going to be created.
    Person *per[n];

    for(int i = 0;i < n;i++){

        cin>>val;
        if(val == 1){
            // If val is 1 current object is of type Professor
            per[i] = new Professor;

        }
        else per[i] = new Student; // Else the current object is of type Student

        per[i]->getdata(); // Get the data from the user.

    }

    for(int i=0;i<n;i++)
        per[i]->putdata(); // Print the required output for each object.

    return 0;

}
