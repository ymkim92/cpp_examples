#include <iostream>

using namespace std;
/*
 * Create classes Rectangle and RectangleArea
 */

class Rectangle
{
protected:
    int width;
    int height;
public:
    Rectangle(): width(0), height(0) {};
    Rectangle(int w, int h): width(w), height(h) {};
    void display();
};

void Rectangle::display()
{
    cout << width << " " << height << endl;
}

class RectangleArea: public Rectangle
{
public:
    void read_input();
    void display();
};

void RectangleArea::read_input()
{
    cin >> width >> height;
}

void RectangleArea::display()
{
    cout << width * height << endl;
}



int main()
{
    /*
     * Declare a RectangleArea object
     */
    RectangleArea r_area;
    
    /*
     * Read the width and height
     */
    r_area.read_input();
    
    /*
     * Print the width and height
     */
    r_area.Rectangle::display();
    
    /*
     * Print the area
     */
    r_area.display();
    
    return 0;
}