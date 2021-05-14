// RUN: %clang_cc1 -fsyntax-only -verify -Wno-unused %s

namespace NS {
int good = 55;
}

void f(int var) {
  // expected-error@+1{{expected '(' after '__builtin_unique_stable_name'}}
  __builtin_unique_stable_name int; // Correct usage is __builtin_unique_stable_name(int);

  // expected-error@+1{{expected '(' after '__builtin_unique_stable_name'}}
  __builtin_unique_stable_name {int}; // Correct usage is __builtin_unique_stable_name(int);

  // expected-error@+2{{expected ')'}}
  // expected-note@+1{{to match this '('}}
  __builtin_unique_stable_name(int; //Missing paren before semicolon

  // expected-error@+2{{expected ')'}}
  // expected-note@+1{{to match this '('}}
  __builtin_unique_stable_name(int, float); // Missing paren before comma

  __builtin_unique_stable_name(var);
  __builtin_unique_stable_name(NS::good);
}

template <typename T>
void f2() {
  __builtin_unique_stable_name(typename T::good_type);
}

struct S {
  class good_type {};
};

void use() {
  f2<S>();
}
