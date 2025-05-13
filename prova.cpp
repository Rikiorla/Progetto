#include <iostream>

int main()
{
    std::cout << "Provide a natural number\n";
    int i;
    std::cin >> i;
    std::cout << "Provide a higher natural number\n";
    int j;
    std::cin >> j;
    int sumnm{(j * (1 + j) / 2) - ((i - 1) * (i) / 2)};
    std::cout << "The sum of all the natural numbers between " << i << " and "
              << j << " is " << sumnm << '\n';
}