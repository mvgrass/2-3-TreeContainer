#include <iostream>
#include <ctime>
#include <random>
#include "Tree.h"

template <class Comp>
void generate(Tree<int, Comp>& tree) {
    const int MIN = 0;
    const int MAX = 100;

    const int size = rand()%8+12;

    tree.clear();
    int value;
    for (int i = 0;i < size;i++) {
        do {
            value = rand() % (MAX - MIN) + MIN;
        } while (tree.exist(value));

        tree.insert(value);
    }
}

struct Comp{
    bool operator()(const int& v1, const int& v2) const {
        return v1>v2;
    }
};

int main() {
    srand(time(0));


    Tree<int> a, b, c, d, e, f;
    Tree<int, Comp>f1;
    f1.insert(4);
    f1.insert(3);

    a = {1, 2, 4};
    b = {2, 5, 1};
    c = a+b;
    generate(d);
    generate(e);

    const int MIN = 0;
    const int MAX = 100;

    const int size = rand()%8+12;

    int value;
    for (int i = 0;i < size;i++) {
        value = rand() % (MAX - MIN) + MIN;
        d.erase(value);
    }

    std::cout<<"A:\n";
    std::for_each(a.begin(), a.end(),[](int val){std::cout<<val<<' ';});
    std::cout<<'\n';

    std::cout<<"A:\n";
    for(auto x : a)
        std::cout<<x<<' ';
    std::cout<<'\n';

	f = a - ((b + (c&d)) | e);
    std::cout << "A:\n" << a << "\n"
              << "B:\n" << b << "\n"
              << "C:\n" << c << "\n"
              << "D:\n" << d << "\n"
              << "E:\n" << e << "\n"
              << "F:\n" << f << "\n";

	Tree<std::pair<int, int>> tr2;
	tr2.insert(std::make_pair(3, 2));
	tr2.insert(std::make_pair(1, 1));
	tr2.insert(std::make_pair(3, 3));
	tr2.insert(std::make_pair(2, 15));
	std::cout << "tr2: "<< '\n';
	for(auto it = tr2.begin(); it!=tr2.end(); it++)
		std::cout<<it->first<<" "<<it->second<<"\n";

	tr2.erase(std::make_pair(2, 15));	
	std::cout<< "tr2:"<< '\n';
	for(auto it = tr2.begin(); it!=tr2.end(); it++)
		std::cout<<it->first<<" "<<it->second<<"\n";

    return 0;
}
