#pragma once

namespace StdStuff {
  void test_hold_alternative() {
    // std::holds_alternative
    // - checks if the variant v holds the alternative T. The call is ill-formed if T does not appear exactly once in Types...
    std::variant<int, std::string> v = "abc";
    std::cout << std::boolalpha
              << "variant holds int? : "
              << std::holds_alternative<int>(v) << '\n'
              << "variant holds string? : "
              << std::holds_alternative<std::string>(v) << '\n';
  }

  void test_string_literals() {
    using namespace std::string_literals;

    // std::literals::string_literals::operator""s
    // Forms a string literal of the desired type:
    //  1) returns std::string{str, len}
    //  2) returns std::u8string{str, len}
    //  3) returns std::u16string{str, len}
    //  4) returns std::u32string{str, len}
    //  5) returns std::wstring{str, len}

    std::string s1 = "abc\0\0def";
    std::string s2 = "abc\0\0def"s;
    std::cout << "s1: " << s1.size() << " \"" << s1 << "\"\n";
    std::cout << "s2: " << s2.size() << " \"" << s2 << "\"\n";
  }

}