#include<iostream>
#include<random>
#include<algorithm>
#include"PVector.hpp"

template<class T>
void print_pvector(const my::PVector<T>& pvec)
{
	std::cout << "[ ";
	for (const auto& item : pvec)
		std::cout << item << " ";

	std::cout << ']' << std::endl;
}

int main(int argc, char* argv[])
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(1, 100);

	my::PVector<int> vec(10);
	for (auto& i : vec)
		i = dis(gen);

	print_pvector(vec);

	std::sort(vec.ptr_begin(), vec.ptr_end(), my::PVectorPredicateWrapper<int>());

	print_pvector(vec);

	std::sort(vec.ptr_begin(), vec.ptr_end(), my::PVectorPredicateWrapper<int, std::greater<int>>());

	print_pvector(vec);

	system("pause");
	return 0;
}