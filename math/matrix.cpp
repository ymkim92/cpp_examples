// g++ -I ~/devel/cpp/eigen-3.4.0/ matrix.cpp
#include <iostream>
#include <iomanip>
#include <stdio.h>

#include <Eigen/Dense>

 
using Eigen::MatrixXd;
using namespace Eigen;

int main()
{
    int i;
    Eigen::Matrix<float, 2, 2> m_A;
    Eigen::Matrix<float, 2, 2> m_B;
    m_A << 0.8, 0.4, 0.345, 0.257;
    m_B = m_A.transpose();
    for (i = 0; i < 1000; i++)
    {
        m_A = m_B * m_A;
    }

    printf("%d\n", (int)(m_A(0,0)*1e9));
    std::cout << std::setprecision(60);
    std::cout << m_A << std::endl;

    return 0;
}
// {
//     MatrixXd m(2, 2);
//     m(0, 0) = 3;
//     m(1, 0) = 2.5;
//     m(0, 1) = -1;
//     m(1, 1) = m(1, 0) + m(0, 1);
//     std::cout << m << std::endl;
// }