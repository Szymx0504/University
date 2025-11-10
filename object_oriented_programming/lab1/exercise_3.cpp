#include <iostream>
#include <vector>

void shift(int *arr, int size, int n)
{
    if (n > size)
    {
        n = n % size;
    }
    while (n < 0)
    {
        n += size;
    }
    int *temp = new int[n];
    for (int i = 0; i < n; i++)
    {
        temp[i] = arr[size - n + i];
    }
    for (int i = size - 1; i >= n; i--)
    {
        arr[i] = arr[i - n];
    }
    for (int i = 0; i < n; i++)
    {
        arr[i] = temp[i];
    }
    delete[] temp;
}

int main()
{
    int size, n;
    std::cin >> size >> n;
    int *arr = new int[size];
    for (int i = 0; i < size; ++i)
    {
        std::cin >> arr[i];
    }

    shift(arr, size, n);

    for (int i = 0; i < size; ++i)
    {
        std::cout << arr[i] << std::endl;
    }

    delete[] arr;

    return 0;
}