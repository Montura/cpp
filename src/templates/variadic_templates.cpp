#include <cstdarg>
#include <cassert>
#include <iostream>
#include <utility>
#include <sstream>

// C++ lectures at MIPT (in Russian). Lecture 7. Variadic templates.
namespace varargs_c {
  int sumAll(int nargs, ...) {
    va_list ap;
    int cnt = 0;
    va_start(ap, nargs);
    for (int i = 0; i < nargs; ++i) {
      cnt += va_arg(ap, int);
    }
    va_end(ap);
    return cnt;
  }

  void test() {
    assert(sumAll(0, 0) == 0);
    assert(sumAll(1, 0) == 0);
    assert(sumAll(1, 1) == 1);
    assert(sumAll(1, 2) == 2);
    assert(sumAll(1, 3) == 3);
    assert(sumAll(2, 0, 0) == 0);
    assert(sumAll(2, 0, 1) == 1);
    assert(sumAll(2, 1, 0) == 1);
    assert(sumAll(2, 1, 1) == 2);
    assert(sumAll(2, 1, 2) == 3);
    assert(sumAll(3, 1, 5, 9) == 15);
  }
}

namespace packs_convolutions {
/// Convolution patterns

  template <typename T>
  class AddSpace {
    const T& refArg;
  public:
    explicit AddSpace(const T& arg): refArg(arg) {}

    friend std::ostream &operator<<(std::ostream &os, const AddSpace &space) {
      os << space.refArg << ' ';
      return os;
    }
  };

  enum class Convolution {
    LEFT,        // ... op pack          | -> ( ... (p1 op p2) op p3) ... op pN)
    INIT_LEFT,   // init op ... op pack  | -> ( ... (init op p1) op p2) ... op pN)
    RIGHT,       // pack op ...          | -> ( p1 op (p2 op (...  (pN-1 op pN) ... )
    RIGHT_FINIT  // pack op ... op fini  | -> ( p1 op (p2 op (...  (pN-1 op fini) ... )
  };

  template<typename ... Args>
  void printAll(std::string && expected, Args &&... args) {
    std::stringstream s;
    (s << ... << AddSpace(args));
    assert((s.str() == expected));
  }

  template<Convolution Conv, typename ... Args>
  int applyConvWith10(Args &&... args) {
    switch (Conv) {
      case Convolution::LEFT:
        return (... - args);
      case Convolution::INIT_LEFT:
        return (10 - ... - args);
      case Convolution::RIGHT:
        return (args - ...);
      case Convolution::RIGHT_FINIT:
        return (args - ... - 10);
      default:
        return 0;
    }
  }

  void test() {
    assert(applyConvWith10<Convolution::LEFT>(1, 2, 3) == -4);        // ((1 - 2) - 3) = -4
    assert(applyConvWith10<Convolution::RIGHT>(1, 2, 3) == 2);        // (1 - (2 - 3)) = 2
    assert(applyConvWith10<Convolution::INIT_LEFT>(1, 2, 3) == 4);    // (((10 - 1) - 2) - 3) = 4
    assert(applyConvWith10<Convolution::RIGHT_FINIT>(1, 2, 3) == -8); // (1 - (2 - (3 - 10))) = -8

    printAll("");
    printAll("1 ", 1);
    printAll("1 2 3 ", 1, 2, 3);
  }
}

namespace unpack_arg_packs {
  template<typename ... T>
  int h(T && ... t) {
//    std::cout << "\t" << __PRETTY_FUNCTION__ << std::endl;
    return 0;
  }

  template<typename ... Args>
  void f(Args &&...) {
//    std::cout << __PRETTY_FUNCTION__ << std::endl;
  }

  template<typename ... Args>
  void g(Args && ... args) {
    f(args...);
    f(&args...);
    f(h(args)...);
    f(const_cast<const Args *>(&args)...);
  }

  template<typename ... Args>
  void tricky(Args && ... args) {
    f(h(args...) + h(args)...);
    f(h(args, args...)...);
  }

  void test() {
    f();
    f(0);
    f(1);
    g(1, 1.0); // -> g(int x, double y);
    //  -> f(x, y);
    //  -> f(&x, &y);
    //  -> f(h(x), h(y));
    //      -> h(x)
    //      -> h(y)
    //  -> f(const_cast<const Args*>(&x), const_cast<const Args*>(&y));

    int x = 0, y = 0, z = 0;
    tricky(x, y, z);
// -> f(h(x,y,z) + h(x), h(x,y,z) + h(y), h(x,y,z) + h(z));
// -> f(h(x, x, y, z), h(y, x, y, z), h(z, x, y, z));
  }
}

namespace emplace_back {
  struct Stat {
    int ctor_count = 0;
    int copy_ctor_count = 0;
    int dtor_count = 0;

    void fillZero() {
      ctor_count = copy_ctor_count = dtor_count = 0;
    }
  };

  static Stat stat;

  class Heavy{
    int n;
    int *t;
  public:
    Heavy (int sz) : n(sz), t(new int[n]) {
      stat.ctor_count++;
    }

    Heavy(const Heavy &rhs) : n(rhs.n), t(new int[n]) {
      stat.copy_ctor_count++;
      memcpy(t, rhs.t, n*sizeof(int));
    }

    ~Heavy () {
      stat.dtor_count++;
      delete [] t;
    }
  };

  template <typename T>
  class Stack {
    struct StackElem {
      T elem;
      StackElem *next;
      StackElem(T e, StackElem *nxt) : elem (e), next (nxt) {}

      template< class... Args >
      StackElem(StackElem *nxt, Args&&... args) : elem(std::forward<Args>(args)...), next(nxt) {}
    };

    struct StackElem *top_;

  public:
    Stack () : top_ (nullptr) {}
    Stack (const Stack<T>& rhs) = delete;
    ~Stack ();

    Stack<T>& operator=(const Stack<T>& rhs) = delete;

    void push_back (const T&);

    template <typename ... Args >
    void emplace_back( Args&&... args );
  };

  template <typename T>
  void Stack<T>::push_back(const T& elem) {
    auto *newelem = new StackElem (elem, top_);
    top_ = newelem;
  }

  template <typename T>
  template <typename ... Args>
  void Stack<T>::emplace_back(Args && ... args) {
    auto *newelem = new StackElem (top_, std::forward<Args>(args)...);
    top_ = newelem;  }

  template <typename T>
  Stack<T>::~Stack () {
    StackElem *nxt = top_;
    while (nxt != nullptr) {
      StackElem *tmp = nxt->next;
      delete nxt;
      nxt = tmp;
    }
    top_ = nullptr;
  }

  void pushBack(int count) {
    Stack<Heavy> s;
    for (int i = 0; i < count; ++i) {
      s.push_back(Heavy(100));
    }
  }

  void emplaceBack(int count) {
    Stack<Heavy> s;
    for (int i = 0; i < count; ++i) {
      s.emplace_back(100);
    }
  }

  void test() {
    pushBack(10);
    assert(stat.ctor_count == 10);
    assert(stat.copy_ctor_count == 20);
    assert(stat.dtor_count == 30);
    stat.fillZero();

    emplaceBack(10);
    assert(stat.ctor_count == 10);
    assert(stat.copy_ctor_count == 0);
    assert(stat.dtor_count == 10);
    stat.fillZero();
  }
}

namespace tuples {
  template <typename T>
  std::pair<bool, T> parseStringPair(const std::string& s) {
    std::istringstream iss(s);
    T tmp;
    bool success = static_cast<bool>(iss >> tmp);
    return std::make_pair(success, tmp);
  }

  template <typename T>
  std::tuple<bool, T> parseStringTuple(const std::string& s) {
    std::istringstream iss(s);
    T tmp;
    bool success = static_cast<bool>(iss >> tmp);
    return std::make_pair(success, tmp);
  }

  template<typename ... Args>
  int sumAll(Args &&... args) {
    return (args + ...);
  }

  void test() {
    auto pair = parseStringPair<int>("456");
    bool success = false;
    int result = 0;
    std::tie(success, result) = pair;
    assert(success);
    assert(result == 456);

    auto tuple = parseStringTuple<int>("789");
    std::tie(success, result) = tuple;
    assert(success);
    assert(result == 789);

    // The type deduction for std::apply is broken. Has to specified them manually
    auto arg = std::make_tuple(1, 2, 3);
    assert(std::apply(sumAll<int, int, int>, std::move(arg)) == 6);
  }
}

void testVariadicTemplates() {
  unpack_arg_packs::test();
  varargs_c::test();
  packs_convolutions::test();
  emplace_back::test();
  tuples::test();
}


