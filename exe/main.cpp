#include <iostream>
#include "any.hpp"
#include <string>

struct tag {
    int id;
    std::string description;
};

int main()
{
    sphere::any a;
    int n;
    std::cin >> n;
    for (int i = 0; i < n; ++i) {
        char type;
        std::cin >> type;
        switch (type)
        {
        case 'i':
        {
            int value;
            std::cin >> value;
            a = value;
            break;
        }
        case 'd':
        {
            double value;
            std::cin >> value;
            a = value;
            break;
        }
        case 'f':
        {
            float value;
            std::cin >> value;
            a = value;
            break;
        }
        case 's':
        {
            std::string value;
            std::cin >> value;
            a = value;
            break;
        }
        case 't':
        {
            tag value;
            std::cin >> value.id >> value.description;
            a = value;
            break;
        }
        }
    }
    
    try {
    char type;
    std::cin >> type;
    switch (type)
        {
        case 'i':
            std::cout << sphere::any_cast<int>(a) << std::endl;
            break;
        case 'd':
            std::cout << sphere::any_cast<double>(a) << std::endl;
            break;
        case 'f':
            std::cout << sphere::any_cast<float>(a) << std::endl;
            break;
        case 's':
            std::cout << sphere::any_cast<std::string>(a) << std::endl;
            break;
        case 't':
        {
            tag tg = sphere::any_cast<tag>(a);
            std::cout << tg.id << ' ' << tg.description << std::endl;
            break;
        }
        }
    }
    catch (const sphere::bad_any_cast &e) {
        std::cout << "cast failed" << std::endl;
    }
}