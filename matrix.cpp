#include "matrix.hpp"
#include <iostream>
#include <math.h>
using namespace std;

static float activation_func(float arg)
{
    return 1.0/(1.0+exp(-arg));
}

int main()
{
    matrix <float> m1(3,3);
    matrix <float> m2(3,3);
    matrix <float> m_ans(3,3);
    for(int i=0;i<3;i++)
        for(int j=0;j<3;j++)
        {
            m1.e(i,j)=3*(i+1)*(j+2)/100.0;
            m2.e(i,j)=3*(i*i-1)*(j-2)/100.0;
        }
    m_ans.multiply(m1,m2);
    m1.print();
    m2.print();
    m_ans.print();
    m1.apply(&activation_func);
    m1.print();
}
