#include <vector>
#include <any>
#include <iostream>

template<typename T>
void foo(const std::any& value) {
	std::cout << std::any_cast<T>(value) << std::endl;
}

int main()
{
	std::cout << "Hello Denis" << std::endl;
	std::any m(20);
	
	std::cout << std::any_cast<int>(m) << std::endl;
	m.emplace<int>(30);
	std::cout << std::any_cast<int>(m) << std::endl;
	std::cout << sizeof(m) << " sizeof any" << std::endl;
	std::cout << sizeof(std::any_cast<int>(m)) << " sizeof type in any" << std::endl;
	std::vector<int> vec{ 1, 2, 3 };
	std::any an_vec(vec);
	auto tmp_vec = std::any_cast<std::vector<int>>(an_vec);
	for (size_t i = 0; i < tmp_vec.size(); ++i)
		std::cout << tmp_vec[i] << '\t';
	std::cout << '\n';

	foo<int>(5);
	foo<double>(16.2);

	return 0;
}
