#include <iostream>
#include <vector>

int* identify(int& ref, int* ptr1, int* ptr2, int* ptr3, int* ptr4)
{
    if(&ref == ptr1){
        return ptr1;
    }
    if(&ref == ptr2){
        return ptr2;
    }
    if(&ref == ptr3){
        return ptr3;
    }
    if(&ref == ptr4){
        return ptr4;
    }
    return nullptr;
}

int main()
{
    int ptr_id;
    std::cin >> ptr_id;
    int a = 27;
    
    int** arr_ptr = new int*[4];
    if (ptr_id >= 0 && ptr_id < 4)
    {
        arr_ptr[ptr_id] = &a;
    }
    
    int* result = identify(a, arr_ptr[0], arr_ptr[1], arr_ptr[2], arr_ptr[3]);
    
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