#include <type_traits>
#include <iostream>

// Fibonacci
template <std::size_t N>
struct Fib: std::integral_constant<std::size_t,
        Fib<N - 2>() + Fib<N - 1>()> {};

template <>
struct Fib<0> : std::integral_constant<std::size_t, 0> {};

template <>
struct Fib<1> : std::integral_constant<std::size_t, 1> {};


// TypeList
template <int ... T>
struct IntList {};

template <int H, int ... T>
struct IntList<H, T...> {
    static const int Head = H;
    using Tail = IntList<T...>;
};

template <>
struct IntList<> {};


// Length of TypeList

// Var 1
template <typename List>
struct Length_1 {
 static const int value = 1 + Length_1<typename List::Tail>::value;
};
template <>
struct Length_1<IntList<>> {
 static const int value = 0;
};

// Var 2
template <typename T>
struct Length_2;

template <template <int ...> class IntList, int ...Values>
struct Length_2 <IntList<Values...> > {
    static constexpr int value = sizeof...(Values);
};


// PushBack, PushFront, Concatenate, Generator<N>
template <int Item, typename List>
struct PushBack;

template <int Item, int ... ListItems>
struct PushBack<Item, IntList<ListItems...>> {
    using type = IntList<ListItems..., Item>;
};

template <int Item, typename List>
struct PushFront;

template <int Item, int ... ListItems>
struct PushFront<Item, IntList<ListItems...>> {
    using type = IntList<Item, ListItems...>;
};

template <typename TL_left, typename TL_right>
struct Concatenate;

template <int ... left_items, int ... right_items>
struct Concatenate <IntList<left_items...>, IntList<right_items...>> {
    using type = IntList<left_items..., right_items...>;
};

// Var 1 (PushBack)
// template <int start, int end = 0>
// struct Generate;

// template <>
// struct Generate<0> {
//     using type = IntList<>;
// };

// template <int N>
// struct Generate <N> {
//     using type = typename PushBack<N - 1, typename Generate<N-1>::type>::type;
// };

// Var 2 (Generator)
template <size_t N, int K = 0>
struct Generate {
    using type = typename PushFront<K, typename Generate<N-1, K + 1>::type>::type;
};

template <int K>
struct Generate<0, K> {
    using type = IntList<>;
};

template <typename T>
void typeChecker() {
    std::cout << __PRETTY_FUNCTION__ << std::endl;
}

void test_metaFunctions() {
    std::cout << Fib<10>::value << std::endl;

    using primes = IntList<2,3,5,7,11,13>;

    // constexpr int head = primes::Head;
    // using odd_primes = primes::Tail;
    // std::cout << head << std::endl;

    constexpr size_t len = Length_1<primes>::value; // 6
    static_assert(Length_1<primes>::value == Length_2<primes>::value);
    std::cout << len << std::endl;

    using new_primes = PushFront<1, primes>::type;
    constexpr size_t len1 = Length_2<new_primes>::value; // 6
    std::cout << len1 << std::endl;

    using result = Concatenate<primes, new_primes>::type;
    constexpr size_t len2 = Length_1<result>::value; // 6
    std::cout << len2 << std::endl;

    using aaa = Generate<9>::type;
    constexpr size_t len3 = Length_2<aaa>::value; // 6
    std::cout << len3 << std::endl;

    typeChecker<aaa>();
}
