#include <iostream>
#include <string>
#include <cassert>
#include <algorithm>

using namespace std;

bool CheckNmeaStart(string nmeaMessage);
bool CheckNmeaGgaCollected(string nmeaMessage);
string msg("$GPGGA,002147.00,0400.00261993,N,00000.00214314,E,1,4,,1.558,M,0.0,M,,*4C\r\n");

int main()
{
    cout << msg << endl;
    assert(CheckNmeaStart(msg) == true);
    assert(CheckNmeaGgaCollected(msg) == true);
}

bool CheckNmeaStart(string nmeaMessage)
{
    if (nmeaMessage.substr(0, 3) == "$GP")
        return true;
    else
        return false;

}

bool CheckNmeaGgaCollected(string nmeaMessage)
{
    const int numOfCommas = 14;

    if (std::count(nmeaMessage.begin(), nmeaMessage.end(), ',') == numOfCommas)
    {
        string last6_2 = nmeaMessage.substr(nmeaMessage.length()-6, 2); // ",*" 4A\r\n
        if (last6_2 == ",*")
            return true;
    }

    return false;
}