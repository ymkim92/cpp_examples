#include <iostream>
#include <map>

std::map<uint16_t, std::string> vv {
    {0, "SYS_ERR_ALARM"},
    {3, "SYS_ERR_SPI"},
    {4, "SYS_ERR_OVERRANAGE"}
};

// static void gen_mapping()
// {
//     vv.insert(std::pair<uint16_t, std::string>(0, "SYS_ERR_ALARM"));
//     vv.insert(std::pair<uint16_t, std::string>(3, "SYS_ERR_SPI"));
//     vv.insert(std::pair<uint16_t, std::string>(4, "SYS_ERR_OVERRANAGE"));
// }

int main()
{
    // gen_mapping();
    std::cout << vv[3] << std::endl;
    std::cout << vv[2] << std::endl;
    return 0;
}