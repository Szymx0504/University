#include <iostream>
#include <vector>

namespace value
{
    int gcd(int a, int b)
    {
        while (b > 0)
        {
            int r = a % b;
            a = b;
            b = r;
        }
        return a;
    }
}

namespace reference
{
    void gcd(int a, int b, int &result)
    {
        while (b > 0)
        {
            int r = a % b;
            a = b;
            b = r;
        }
        result = a;
    }
}

namespace pointer
{
    void gcd(int a, int b, int *result)
    {
        while(b > 0){
            int r = a % b;
            a = b;
            b = r;
        }
        if(result != nullptr){
            *result = a;
        }
    }
}

int main()
{
    int a, b, c;
    std::cin >> a >> b >> c;
    int *result = &c;
    if (c == 0)
    {
        result = nullptr;
    }

    // by value
    std::cout << value::gcd(a, b) << std::endl;
    // by reference
    reference::gcd(a, b, c);
    std::cout << c << std::endl;
    // by pointer
    pointer::gcd(a, b, result);
    if (result != nullptr)
    {
        std::cout << *result << std::endl;
    }
    else
    {
        std::cout << 0 << std::endl;
    }
    return 0;
}