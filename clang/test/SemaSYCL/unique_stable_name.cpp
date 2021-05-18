// RUN: %clang_cc1 %s --std=c++17 -fsyntax-only -fsycl-is-device -Wno-unused -verify

template <typename KernelName, typename KernelType>
[[clang::sycl_kernel]] void kernel_single_task(KernelType kernelFunc) { // #1 #2 #3 #4 #5 #6 #7 #8 #9 #10 #11
  kernelFunc();
}

template <typename Func>
void kernel3func(const Func &F3) {
  constexpr const char *F3_output = __builtin_unique_stable_name(F3);
  // expected-error@#8{{kernel instantiation changes the result of an evaluated '__builtin_unique_stable_name'}}
  // expected-note@+1{{in instantiation of function template specialization}}
  kernel_single_task<class kernel3>(F3); // 8th instantiation throwing error
}

template <typename Func>
void kernel4func(const Func &F4) {
  constexpr const char *F4_output = __builtin_unique_stable_name(F4);
  // expected-error@#9{{kernel instantiation changes the result of an evaluated '__builtin_unique_stable_name'}}
  // expected-note@+1{{in instantiation of function template specialization}}
  kernel_single_task<class kernel4>([]() {}); // 9th instantiation throwing error
}

template <template <typename> typename Outer, typename Inner>
struct S {
  void operator()() const;
};

template <typename Ty>
struct T {};

template <typename Func>
void kernel13_14func(const Func &F) {
  // expected-error@#10{{kernel instantiation changes the result of an evaluated '__builtin_unique_stable_name'}}
  // expected-note@+1{{in instantiation of function template specialization}}
  kernel_single_task<class kernel13>(F); // 10th instantiation throwing error
  // expected-error@#11{{kernel instantiation changes the result of an evaluated '__builtin_unique_stable_name'}}
  // expected-note@+1{{in instantiation of function template specialization}}
  kernel_single_task<class kernel14>(F); // 11th instantiation throwing error
  // Using the same functor twice should be fine
}

template <typename Ty>
auto func() -> decltype(__builtin_unique_stable_name(Ty::str));

struct Derp {
  static constexpr const char str[] = "derp derp derp";
};

template <typename T>
static constexpr const char *output1 = __builtin_unique_stable_name(T);

#define MACRO12()                \
  auto l12 = []() { return 1; }; \
  constexpr const char *l1_output = __builtin_unique_stable_name(l12);

int main() {

  // kernel0
  // kernel_single_task<class kernel0>(func<Derp>());

  // kernel1 Evaluate the builtin in a constant context, then call the kernel with that same type
  auto l1 = []() {};
  constexpr const char *l1_output = __builtin_unique_stable_name(l1);
  kernel_single_task<class kernel1>(
      [=]() { l1(); });

  // kernel2 Call to the builtin on a lambda, then using the lambda for a kernel name
  auto l2 = []() { return 1; };
  constexpr const char *l2_output = __builtin_unique_stable_name(l2); // expected-note {{'__builtin_unique_stable_name' evaluated here}}
  // expected-error@#1{{kernel instantiation changes the result of an evaluated '__builtin_unique_stable_name'}}
  // expected-note@+1{{in instantiation of function template specialization}}
  kernel_single_task<class kernel2>(l2); // 1st instantiation throwing error

  // kernel3
  // The current function is named with a lambda (that is, takes a lambda as a template parameter).  Call the builtin (perhaps on the current function?), then pass it to a kernel.
  // expected-note@+1{{in instantiation of function template specialization}}
  kernel3func([]() {});

  // kernel4 Same as above, but you use a NEW lambda defined inline to name the kernel.
  // expected-note@+1{{in instantiation of function template specialization}}
  kernel4func([]() {});

  // kernel7 - expect error
  // Same as kernel6, except make l6 part of naming kernel
  auto l7 = []() { return 1; };
  auto l8 = [](decltype(l7) *derp = nullptr) { return 2; };
  constexpr const char *l7_output = __builtin_unique_stable_name(l7); // expected-note {{'__builtin_unique_stable_name' evaluated here}}
  // expected-error@#2{{kernel instantiation changes the result of an evaluated '__builtin_unique_stable_name'}}
  // expected-note@+1{{in instantiation of function template specialization}}
  kernel_single_task<class kernel7>(l8); // 2nd instantiation throwing error

  // kernel8 and kernel9 - expect no error
  // Make two lambdas
  // Call the builtin on the second of the two lambdas (in a constexpr context)
  // Within a constexpr if, pass the first lambda to a kernel in the true branch, pass the second lambda to a kernel in the false branch
  auto l9 = []() { return 1; };
  auto l10 = []() { return 2; };
  // expected-note@+1{{'__builtin_unique_stable_name' evaluated here}}
  constexpr const char *l10_output = __builtin_unique_stable_name(l10);
  if constexpr (1) {
    // expected-error@#3{{kernel instantiation changes the result of an evaluated '__builtin_unique_stable_name'}}
    // expected-note@+1{{in instantiation of function template specialization}}
    kernel_single_task<class kernel8>(l9); // 3rd instantiation throwing
  } else {
    // expected-error@#4{{kernel instantiation changes the result of an evaluated '__builtin_unique_stable_name'}}
    // expected-note@+1{{in instantiation of function template specialization}}
    kernel_single_task<class kernel9>(l10); // 4th instantiation throwing error
  }

  // This section is not supposed to emit any diagnostics but currently there are two cases with issues
  //
  // kernel13 and kernel14 - expect no error
  // pass the same lambda to two kernels
  kernel13_14func([]() {}); // expected-note 2 {{in instantiation of function template specialization}}

  // kernel5 - same as kernel11 below?
  // Same as the above two, except the thing in the template parameter/kernel calls are a function object that has a lambda in its template parameters.

  // kernel6 - expect no error
  // Make a lambda
  // Make another lambda that captures the first one
  // Call the builtin on the first lambda (in a constexpr context)
  // Pass the second lambda to a kernel
  auto l5 = []() { return 1; };
  auto l6 = [l5]() { return 2; };
  constexpr const char *l5_output = __builtin_unique_stable_name(l5); // expected-note {{'__builtin_unique_stable_name' evaluated here}}
  // expected-error@#5{{kernel instantiation changes the result of an evaluated '__builtin_unique_stable_name'}}
  // expected-note@+1{{in instantiation of function template specialization}}
  kernel_single_task<class kernel6>(l6); // 5th instantiation throwing error

  // Make a lambda
  // Call the builtin on the first lambda (in a constexpr context)
  // Pass the lambda as a template template parameter to a kernel
  auto l11 = []() { return 1; };
  // expected-note@+1{{'__builtin_unique_stable_name' evaluated here}}
  constexpr const char *l11_output = __builtin_unique_stable_name(l11);
  // expected-error@#6{{kernel instantiation changes the result of an evaluated '__builtin_unique_stable_name'}}
  // expected-note@+1{{in instantiation of function template specialization}}
  kernel_single_task<class kernel11>(S<T, decltype(l11)>{}); // 6th instantiation throwing error

  // expected-error@#7{{kernel instantiation changes the result of an evaluated '__builtin_unique_stable_name'}}
  kernel_single_task<class kernel12>( // 7th instantiation throwing error
      []() {
        // expected-note@+1{{'__builtin_unique_stable_name' evaluated here}}
        MACRO12();
      });
}

namespace NS {}

void f() {
  // expected-error@+1{{use of undeclared identifier 'bad_var'}}
  __builtin_unique_stable_name(bad_var);
  // expected-error@+1{{use of undeclared identifier 'bad'}}
  __builtin_unique_stable_name(bad::type);
  // expected-error@+1{{no member named 'still_bad' in namespace 'NS'}}
  __builtin_unique_stable_name(NS::still_bad);
}

template <typename T>
void f2() {
  // expected-error@+1{{no member named 'bad_val' in 'St'}}
  __builtin_unique_stable_name(T::bad_val);
  // expected-error@+1{{no type named 'bad_type' in 'St'}}
  __builtin_unique_stable_name(typename T::bad_type);
}

struct St {};

void use() {
  // expected-note@+1{{in instantiation of}}
  f2<St>();
}
