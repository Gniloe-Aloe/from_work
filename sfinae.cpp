#include <iostream>
#include <vector>
#include <type_traits>
#include <limits>
#include <functional>
#include <string>
#include <iterator>

template<typename T>
void temp_test(T value) {
    std::cout << "->temp_test() sizeof = " << sizeof(value) << '\n';
}


template <typename T>
void print(const T& number){
    std::cout << "basic print\n";
}
//никогда не вызывается, перегружанная функция > шаблонной
template <>
void print(const uint64_t& number) {
    std::cout << "specialize print\n";
}

void print(const uint64_t& number) {
    std::cout << "overload print\n";
}

template<typename T>
typename T::type sfinae1(T value) {
    std::cout << "t::type\n";
    return 0;
}

template<typename ...>
int sfinae1(...) {
    std::cout << "int\n";
    return 0;
}
//=========
template<bool B, typename T>
struct enable_if {};

template<typename T>
struct  enable_if<true, T>{
    typename T type;
};
//========
class dkos {
public:
    int x;
    int y;
};
//========
template<typename T>
struct is_class {
private:
    template<typename C>
    static int dkosinov(int C::* p)noexcept { return 1; }
    template<typename ...>
    static char dkosinov(...)noexcept { return 1; }
public:
    static const bool value = sizeof(dkosinov<T>()) == sizeof(int);
};
//========
class static_test {
public:
    static_test() {
        
    }

    static int get_value() noexcept {
        return value++;
    }
private:
    static int value;
};
int static_test::value = 0;
//========
class invoke_test {
public:
    void print_line(const int& value) {
        std::cout << "print line() " << value << "\n";
    }
    static void print_string(const int& value) {
        std::cout << "print string() " << value << "\n";
    }
};
//=======
// тип возвращаемого значения выводится из выражения после ->
template<typename T>
auto new_sfinae(const T& value) -> decltype(T().size()) {
    std::cout << 1 << std::endl;
    return 1;
}

// функция для всех типов, не имеющих метода size
size_t new_sfinae(...) {
    std::cout << 2 << std::endl;
    return 2;
}
//=======
template<typename T, typename = std::enable_if_t<std::is_class_v<T>>>
void enif(const T& value) {
    std::cout << "is class" << '\n';
}

void enif(...) {
    std::cout << "is not class" << '\n';
}
//=======
template<typename T, typename = std::enable_if_t<std::is_pointer_v<T>>>
void my_sfinae(const T& pointer) {
    std::cout << "pointer" << std::endl;
}

void my_sfinae(...) {
    std::cout << "not pointer" << std::endl;
}
//=======
template<typename T, typename = std::enable_if_t<std::is_default_constructible_v<T>>>
void sfinae_construct(const T& value) {
    std::cout << "has default constructor\n";
}

void sfinae_construct(...) {
    std::cout << "has not default constructor\n";
}

class without_constructor {
public:
    without_constructor() = delete;
    without_constructor(int x) { this->x = x; };
    int x;
};
//=======
template<typename T, typename I = int>
//we can do it without "typename I" and just write "int()"
auto my_f(T& container) ->decltype(T().begin(), int()){
    std::cout << "has iterator begin()\n";
    return 1;
}

int my_f(...) {
    std::cout << "has not iterator begin()\n";
    return 0;
}
//=======!
template <typename T, typename = std::enable_if_t<std::is_class_v<T>>>
auto print_container(const T& container) ->decltype(T().cbegin(), T().cend, void()) {
    std::cout << "is class" << std::endl;
    for (const auto& it : container)
        std::cout << it << "\t";
    std::cout << '\n';
}

template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
void print_container(const T& value){
    std::cout << "is value" << std::endl;
    std::cout << value << '\n';
}

void print_container(...) {
    std::cout << "is not value or container" << std::endl;
}
//=======!
template<typename T = int>
void strack(const T& value) {
    std::cout << "int\n";
}

template<>
void strack(const double& value) {
    std::cout << "double\n";
}

//шаблонную функцию в классе нельзя сделать виртульной 
int main()
{
    
    /*auto pf_static = &invoke_test::print_string;
    auto pf = &invoke_test::print_line;
    invoke_test obj;

    (obj.*pf)(5);
    std::invoke(pf, obj, 5);
    std::invoke(pf_static, 5);
    pf_static(5);*/

  /*  int x = 10;
    int* p = &x;
    decltype(p) p_var = p;
    ++(*p_var);
    std::cout << p_var << '\t' << *p_var << std::endl;*/

    std::vector<int> vec{ 1, 2, 3 };
    strack(1);
    strack((double)1);


    return 1;
}



