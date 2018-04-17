#include <iostream>
#include <vector>
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
    
	generate(a);
	generate(b);
	generate(c);
	generate(d);
	generate(e);


	f = a - ((b + (c&d)) | e);
	std::cout << "Двуместные операции над множествами\n"
		<< "f = a - ((b + (c&d)) | e);\n\n";
	std::cout << "A:\n" << a << "\n"
		<< "B:\n" << b << "\n"
		<< "C:\n" << c << "\n"
		<< "D:\n" << d << "\n"
		<< "E:\n" << e << "\n"
		<< "F:\n" << f << "\n";
	std::cout << "===============================================================================\n";
	
	std::cout << "Демонстрация работы с различными компараторами и стандартными алгоритмами из библиотеки Algorithms\n\n";
	Tree<int> increase_tree1;
	generate(increase_tree1);
	std::cout << "increase tree1:\nВывод при помощи std::for_each\n";
	std::for_each(increase_tree1.begin(), increase_tree1.end(), [](int val) {std::cout << val << ' ';});
	std::cout << '\n';

	Tree<int> increase_tree2;
	generate(increase_tree2);
	std::cout << "increase tree2:\nВывод при помощи std::for_each\n";
	std::for_each(increase_tree2.begin(), increase_tree2.end(), [](int val) {std::cout << val << ' ';});
	std::cout << '\n';

	std::vector<int>vec;
	std::merge(increase_tree1.begin(), increase_tree1.end(), increase_tree2.begin(), increase_tree2.end(), std::back_inserter(vec));
	std::cout << "\nСлияние increase tree1 и increase tree2 при помощи std::merge\n";
	for (int val : vec)
		std::cout << val << " ";
	std::cout << "\n\n";

	Tree<int, Comp> decrease_tree;
	generate(decrease_tree);
	std::cout << "decrease tree:\n";
	for (auto x : decrease_tree)
		std::cout << x << ' ';
	std::cout << '\n';

	std::cout << "===============================================================================\n"
		<< "Демонстрация работы с типом данных std::pair\n";

	Tree<std::pair<int, int>> pair_tree = { std::make_pair(3, 2), std::make_pair(1, 1), std::make_pair(3, 3), std::make_pair(2, 15)};
	std::cout << "pair_tree: " << '\n';
	for (auto it = pair_tree.begin(); it != pair_tree.end(); it++)
		std::cout << it->first << " " << it->second << "\n";

	std::cout << "\nУдаление пары (2, 15)\n";
	pair_tree.erase(std::make_pair(2, 15));
	std::cout << "pair_tree:" << '\n';
	for (auto it = pair_tree.begin(); it != pair_tree.end(); it++)
		std::cout << it->first << " " << it->second << "\n";


    return 0;
}
