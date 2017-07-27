#include "bignumber.h"

#include <stdio.h>
#include <iostream>

int main()
{
    BigNumber bn1("12345678901234567890123456789012345678901234567890123456789012345678901234567890");
    BigNumber sum = bn1 + bn1;
    QString str = sum.toString();
    std::cout << str.toStdString() << std::endl;
    exit(0);
}
