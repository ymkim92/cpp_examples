#include <iostream>

enum class DayScoped{ mon, tue, wed, thu, fri, sat, sun };

DayScoped useMe(DayScoped day){

    switch (day)
    {
    case DayScoped::mon:
        std::cout << "Monday" << std::endl;
        break;
    case DayScoped::sun:
        std::cout << "Sunday" << std::endl;
        break;
    default:
        std::cout << "Other day" << std::endl;
    }
    return day;
}

uint32_t m_status;
DayScoped& operator++(DayScoped& d)
{
    return d = (d == DayScoped::sun) ? DayScoped::mon : static_cast<DayScoped>(static_cast<int>(d)+1);
}

// DayScoped& operator|=(DayScoped& d1, DayScoped& d2)
// {
//     m_status |= 1 << static_cast<uint32_t>(d2);
//     return d2;
// }

DayScoped& operator=(DayScoped& d1, DayScoped d2)
{
    // m_status = 0;
    // m_status |= 1 << static_cast<uint32_t>(d);
    d1 = d2;
}

int main()
{
    DayScoped day {DayScoped::mon};
    auto ret = useMe(day); 
    
    ++day;      // day++ doesn't work!! ??
    // ret |= useMe(day); 
    std::cout << m_status << std::endl;
    return 0;
}