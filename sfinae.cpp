#include <iostream>
#include <vector>
#include <type_traits>
#include <limits>
#include <functional>
#include <string>
#include <iterator>
#include <cstdarg>
#include <any>

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
    std::cout << "is class ";
    for (const auto& it : container)
        std::cout << it << "\t";
    std::cout << '\n';
}

template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
auto print_container(const T& value) ->decltype(T(), void()){
    std::cout << "is value ";
    std::cout << value << '\n';
}

template<typename T, typename = std::enable_if_t<std::is_pointer_v<T>>>
auto print_container(const T& pointer) -> decltype(*T(), void()) {
    std::cout << "is pointer " << *pointer << '\n';
}

template<typename T, typename = std::enable_if_t<std::is_class_v<T>>>
auto print_container(T& my_class) -> decltype(T().dkosinov(), void()) {
    my_class.dkosinov();
}

void print_container(...) {
    std::cout << "is not value or container " << std::endl;
}

class for_sfinae {
public:
    void dkosinov() {
        std::cout << "is_my_class" << std::endl;
    }
};
//=======!
template<typename T = int>
void strack(const T& value) {
    std::cout << "int\n";
}

template<>
void strack(const double& value) {
    std::cout << "double\n";
}
//=======
template<typename T>
void ellips_test(T first, ...) {
    va_list list;
    va_start(list, first);
    std::cout << first << '\t';
    while (true) {
        T tmp = va_arg(list, T);
        std::cout << tmp << '\t';
        if (tmp == -1. || tmp == -1)
            break;
    }
    std::cout << '\n';
    //очищаем лист
    va_end(list);
}
template<typename T>
void ellips_test2(...) {
    va_list list;
    va_start(list, T);
    while (true) {
        std::cout << va_arg(list, T) << '\t';
    }
    
}
//=======
template<typename T, typename = std::enable_if_t<std::is_class_v<T>>>
void dkosinov_sfinae(const T& class_obj){
    std::cout << "is class\n";
}

template<typename T>
auto dkosinov_sfinae(const T& value) ->decltype(++T(), void()) {
    std::cout << "is number\n";
}

void dkosinov_sfinae(...) {
    std::cout << "is not class or number\n";
}
//=======
class S {
public:
    int x;
    bool y;
    double z;
    std::string str;
};
//=======
template<typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
void same(T value) {
    std::cout << "is int" << std::endl;
}

void same(...) {
    std::cout << "not\n";
}
//=======
template <typename T>
auto same2(T t) -> decltype(T().c_str(), void()) {
    std::cout << "string\n";
}

void same2(...) {
    std::cout << "not\n";
}
//or
template <typename T>
std::enable_if_t<std::is_same_v<T, std::string>, std::string> same3(T t) {
    std::cout << "is string\n";
    return t;
}

template <typename T>
std::enable_if_t<!std::is_same_v<T, std::string>, std::string> same3(T t) {
    std::cout << "is not string\n";
    return std::to_string(t);
}
//======= variadic template
template <size_t...>
struct Sum {};

template <>
struct Sum<>
{
    enum { value = 0 };
};

template <size_t first, size_t... other>
struct Sum<first, other...>
{
    enum { value = first + Sum<other...>::value };
};

template <class... Types>
struct Sum_sizeof
{
    enum { value = Sum<sizeof(Types)...>::value };
};
//=======!!
class My_pack {
public:
    int x;
    double y;
    std::string z;
};

template <typename T>
void show_list(T value) {
    std::cout << value << '\n';
    return;
}

template<typename T, typename... Args> // Args — это пакет параметров шаблона
void show_list(T value, Args... arg) // arg — это пакет параметров функции
{
    std::cout << value << "\t";
    show_list(arg...); // <--- Рекурсивные вызовы
}
//retry after long time
template<typename T>
auto fff(T value)->decltype(T().c_str(), void()) {
    std::cout << "is string\n";
}
void fff(...) {
    std::cout << "is not string\n";
}
//==
template <typename T>
std::enable_if_t<std::is_same_v<T, std::string>, std::string> ddd(T t) {
    std::cout << "is string\n";
    return t;
}

template <typename T>
std::enable_if_t<!std::is_same_v<T, std::string>, std::string> ddd(T t) {
    std::cout << "is not string\n";
    return std::to_string(t);
}
//== if constexpr
template<typename T>
void mmm(T value) {
    if constexpr (std::is_same_v<T, std::string>) {
        std::cout << value.size() << " string size\n";
    }
    else {
        std::cout << "is not string\n";
    }
}
//==std::enable_if
template <bool B, typename T = void>
using enable_if_t = typename std::enable_if<B, T>::type;
template <class S>
enable_if_t<std::is_class<S>::value> bar(S value_class) {
    std::cout << "is class\n";
}

template <class S>
enable_if_t<!std::is_class<S>::value> bar(S non_class) {
    std::cout << "is not class \n";
}






//шаблонную функцию в классе нельзя сделать виртульной 
int main()
{
    std::vector<int> vec;
    std::string str;
    
    
  /*  std::string str = "hello";
    show_list(12, str, 5.1, true);
    std::string* pstr = &str;
    decltype(auto) tmp = pstr;
    std::cout << tmp << std::endl;
    std::string newstr = *tmp;
    std::cout << newstr << std::endl;*/
    

    //same3(str);
    //same3(5);

    //std::cout << std::is_same<int, std::string>::value << std::endl;

  /*  ellips_test(1, 2, 3, 4, 5, -1);
    ellips_test(1.1, 2., 3., 4., 5., -1.);
    ellips_test(-1.1, 2., 3., 4.997, 5., -1.);*/

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

    /*std::vector<int> vec{ 1, 2, 3 };
    int* px;
    int x = 10;
    px = &x;
    print_container(1);
    print_container(vec);
    print_container(px);
    print_container(vec.begin());
    for_sfinae obj;
    print_container(obj);*/

    return 0;
}


