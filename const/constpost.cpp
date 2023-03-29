#include <iostream>

// class SensorHealth
// {
// public:
//     int status{};

//     int is_okay() const
//     {
//         return status;
//     }
// };

struct sensor_health_t 
{

    int status {};

    // there should be const
    // otherwise, compiler will complain with this message:
    // error: passing ‘const sensor_health_t’ as ‘this’ argument discards qualifiers [-fpermissive]
    int is_healthy() const
    {
        if (status == 1) {
            return 2;
        }
        return 3;
    }
};

class SensorHealth
{
public:
    sensor_health_t status {};

    int is_okay() const
    {
        return status.is_healthy();
    }
};

int main() 
{
    auto health = SensorHealth();
    std::cout << health.is_okay() << std::endl;
    return 0;
}
