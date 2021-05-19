// RUN: %clang_cc1 -triple spir64-unknown-unknown-sycldevice -fsycl-is-device -disable-llvm-passes -emit-llvm %s -o - | FileCheck %s
// CHECK: @[[LAMBDA_KERNEL3:[^\w]+]] = private unnamed_addr constant [[LAMBDA_K3_SIZE:\[[0-9]+ x i8\]]] c"_ZTSZ4mainEUlPZ4mainEUlvE10002_E10001_\00"
// CHECK: @[[INT1:[^\w]+]] = private unnamed_addr constant [[INT1_SIZE:\[[0-9]+ x i8\]]] c"_ZTSi\00"
// CHECK: @[[STRING:[^\w]+]] = private unnamed_addr constant [[STRING_SIZE:\[[0-9]+ x i8\]]] c"_ZTSAppL_ZZ4mainE1jE_i\00",
// CHECK: @[[INT:[^\w]+]] = private unnamed_addr constant [[INT_SIZE:\[[0-9]+ x i8\]]] c"_ZTSi\00"
// CHECK: @[[LAMBDA_X:[^\w]+]] = private unnamed_addr constant [[LAMBDA_X_SIZE:\[[0-9]+ x i8\]]] c"_ZTSZZ4mainENKUlvE10000_clEvEUlvE_\00"
// CHECK: @[[LAMBDA_Y:[^\w]+]] = private unnamed_addr constant [[LAMBDA_Y_SIZE:\[[0-9]+ x i8\]]] c"_ZTSZZ4mainENKUlvE10000_clEvEUlvE_\00"
// CHECK: @[[MACRO_X:[^\w]+]] = private unnamed_addr constant [[MACRO_SIZE:\[[0-9]+ x i8\]]] c"_ZTSZZ4mainENKUlvE10000_clEvEUlvE0_\00"
// CHECK: @[[MACRO_Y:[^\w]+]] =  private unnamed_addr constant [[MACRO_SIZE]] c"_ZTSZZ4mainENKUlvE10000_clEvEUlvE1_\00"
// CHECK: @usn_str.8 = private unnamed_addr constant [36 x i8] c"_ZTSZZ4mainENKUlvE10000_clEvEUlvE2_\00", align 1
// CHECK: @usn_str.9 = private unnamed_addr constant [36 x i8] c"_ZTSZZ4mainENKUlvE10000_clEvEUlvE3_\00", align 1
// CHECK: @[[MACRO_MACRO_X:[^\w]+]] = private unnamed_addr constant [[MACRO_MACRO_SIZE:\[[0-9]+ x i8\]]] c"_ZTSZZ4mainENKUlvE10000_clEvEUlvE4_\00"
// CHECK: @[[MACRO_MACRO_Y:[^\w]+]] = private unnamed_addr constant [[MACRO_MACRO_SIZE]] c"_ZTSZZ4mainENKUlvE10000_clEvEUlvE5_\00"
// CHECK: @[[LAMBDA:[^\w]+]] = private unnamed_addr constant [[LAMBDA_SIZE:\[[0-9]+ x i8\]]] c"_ZTSZZ4mainENKUlvE10000_clEvEUlvE_\00"
// CHECK: @[[LAMBDA_IN_DEP_INT:[^\w]+]] = private unnamed_addr constant [[DEP_INT_SIZE:\[[0-9]+ x i8\]]] c"_ZTSZ28lambda_in_dependent_functionIiEvvEUlvE_\00",
// CHECK: @[[LAMBDA_IN_DEP_X:[^\w]+]] = private unnamed_addr constant [[DEP_LAMBDA_SIZE:\[[0-9]+ x i8\]]] c"_ZTSZ28lambda_in_dependent_functionIZZ4mainENKUlvE10000_clEvEUlvE_EvvEUlvE_\00",
// CHECK: @[[LAMBDA_NO_DEP:[^\w]+]] = private unnamed_addr constant [[NO_DEP_LAMBDA_SIZE:\[[0-9]+ x i8\]]] c"_ZTSZ13lambda_no_depIidEvT_T0_EUlidE_\00",
// CHECK: @[[LAMBDA_TWO_DEP:[^\w]+]] = private unnamed_addr constant [[DEP_LAMBDA1_SIZE:\[[0-9]+ x i8\]]] c"_ZTSZ14lambda_two_depIZZ4mainENKUlvE10000_clEvEUliE_ZZ4mainENKS0_clEvEUldE_EvvEUlvE_\00",
// CHECK: @[[LAMBDA_TWO_DEP2:[^\w]+]] = private unnamed_addr constant [[DEP_LAMBDA2_SIZE:\[[0-9]+ x i8\]]] c"_ZTSZ14lambda_two_depIZZ4mainENKUlvE10000_clEvEUldE_ZZ4mainENKS0_clEvEUliE_EvvEUlvE_\00",

// CHECK: call spir_func void @_Z18kernel_single_taskIZ4mainE7kernel2PFPKcvEEvT0_(i8* ()* @_Z4funcI4DerpEDTu20__unique_stable_nameXsrT_3strEEEv)
// CHECK: call spir_func void @_Z18kernel_single_taskIZ4mainE7kernel3Z4mainEUlPZ4mainEUlvE0_E_EvT0_
// CHECK: call spir_func void @puts(i8* getelementptr inbounds ([[LAMBDA_K3_SIZE]], [[LAMBDA_K3_SIZE]]* @[[LAMBDA_KERNEL3]]
// CHECK: call spir_func void @puts(i8* getelementptr inbounds ([[INT1_SIZE]], [[INT1_SIZE]]* @[[INT1]]
// CHECK: call spir_func void @puts(i8* getelementptr inbounds ([[STRING_SIZE]], [[STRING_SIZE]]* @[[STRING]]

// CHECK: define internal spir_func void @_Z18kernel_single_taskIZ4mainE6kernelZ4mainEUlvE_EvT0_
// CHECK: define internal spir_func void @_Z18kernel_single_taskIZ4mainE7kernel2PFPKcvEEvT0_
// CHECK: declare spir_func i8* @_Z4funcI4DerpEDTu20__unique_stable_nameXsrT_3strEEEv
// CHECK: define internal spir_func void @_Z18kernel_single_taskIZ4mainE7kernel3Z4mainEUlPZ4mainEUlvE0_E_EvT0_

extern "C" void puts(const char *) {}

template <typename T>
void template_param() {
  puts(__builtin_unique_stable_name(T));
}

template <typename T>
void lambda_in_dependent_function() {
  auto y = [] {};
  puts(__builtin_unique_stable_name(y));
}

template <typename Tw, typename Tz>
void lambda_two_dep() {
  auto z = [] {};
  puts(__builtin_unique_stable_name(z));
}

template <typename Tw, typename Tz>
void lambda_no_dep(Tw a, Tz b) {
  auto p = [](Tw a, Tz b) { return ((Tz)a + b); };
  puts(__builtin_unique_stable_name(p));
}

#define DEF_IN_MACRO()                           \
  auto MACRO_X = []() {};                        \
  auto MACRO_Y = []() {};                        \
  puts(__builtin_unique_stable_name(MACRO_X)); \
  puts(__builtin_unique_stable_name(MACRO_Y));

#define MACRO_CALLS_MACRO() \
  { DEF_IN_MACRO(); }       \
  { DEF_IN_MACRO(); }

template <typename Ty>
auto func() -> decltype(__builtin_unique_stable_name(Ty::str));

struct Derp {
  static constexpr const char str[] = "derp derp derp";
};

template <typename KernelName, typename KernelType>
[[clang::sycl_kernel]] void kernel_single_task(KernelType kernelFunc) {
  kernelFunc();
}

int main() {
  kernel_single_task<class kernel>(
      []() {
        puts(__builtin_unique_stable_name(int));
        // CHECK: call spir_func void @puts(i8* getelementptr inbounds ([[INT_SIZE]], [[INT_SIZE]]* @[[INT]]

        auto x = []() {};
        puts(__builtin_unique_stable_name(x));
        puts(__builtin_unique_stable_name(decltype(x)));
        // CHECK: call spir_func void @puts(i8* getelementptr inbounds ([[LAMBDA_X_SIZE]], [[LAMBDA_X_SIZE]]* @[[LAMBDA_X]]
        // CHECK: call spir_func void @puts(i8* getelementptr inbounds ([[LAMBDA_Y_SIZE]], [[LAMBDA_Y_SIZE]]* @[[LAMBDA_Y]]

        DEF_IN_MACRO();
        // CHECK: call spir_func void @puts(i8* getelementptr inbounds ([[MACRO_SIZE]], [[MACRO_SIZE]]* @[[MACRO_X]]
        // CHECK: call spir_func void @puts(i8* getelementptr inbounds ([[MACRO_SIZE]], [[MACRO_SIZE]]* @[[MACRO_Y]]

        MACRO_CALLS_MACRO();
        // CHECK: call spir_func void @puts(i8* getelementptr inbounds ([[MACRO_MACRO_SIZE]], [[MACRO_MACRO_SIZE]]* @[[MACRO_MACRO_X]]
        // CHECK: call spir_func void @puts(i8* getelementptr inbounds ([[MACRO_MACRO_SIZE]], [[MACRO_MACRO_SIZE]]* @[[MACRO_MACRO_Y]]

        // CHECK: call spir_func void @_Z14template_paramIiEvv
        // CHECK: call spir_func void @_Z14template_paramIZZ4mainENKUlvE_clEvEUlvE_Evv
        // CHECK: call spir_func void @_Z28lambda_in_dependent_functionIiEvv
        // CHECK: call spir_func void @_Z28lambda_in_dependent_functionIZZ4mainENKUlvE_clEvEUlvE_Evv
        // CHECK: call spir_func void @_Z13lambda_no_depIidEvT_T0_(i32 3, double 5.500000e+00)
        // CHECK: store i32 5, i32* %a, align 4
        // CHECK: store double 1.070000e+01, double* %b, align 8
        // CHECK: call spir_func void @_Z14lambda_two_depIZZ4mainENKUlvE_clEvEUliE_ZZ4mainENKS0_clEvEUldE_Evv
        // CHECK: call spir_func void @_Z14lambda_two_depIZZ4mainENKUlvE_clEvEUldE_ZZ4mainENKS0_clEvEUliE_Evv
        template_param<int>();
        // CHECK: define linkonce_odr spir_func void @_Z14template_paramIiEvv
        // CHECK: call spir_func void @puts(i8* getelementptr inbounds ([[INT1_SIZE]], [[INT1_SIZE]]* @[[INT1]]

        template_param<decltype(x)>();
        // CHECK: define internal spir_func void @_Z14template_paramIZZ4mainENKUlvE_clEvEUlvE_Evv
        // CHECK: call spir_func void @puts(i8* getelementptr inbounds ([[LAMBDA_SIZE]], [[LAMBDA_SIZE]]* @[[LAMBDA]]

        lambda_in_dependent_function<int>();
        // CHECK: define linkonce_odr spir_func void @_Z28lambda_in_dependent_functionIiEvv
        // CHECK: call spir_func void @puts(i8* getelementptr inbounds ([[DEP_INT_SIZE]], [[DEP_INT_SIZE]]* @[[LAMBDA_IN_DEP_INT]]

        lambda_in_dependent_function<decltype(x)>();
        // CHECK: define internal spir_func void @_Z28lambda_in_dependent_functionIZZ4mainENKUlvE_clEvEUlvE_Evv
        // CHECK: call spir_func void @puts(i8* getelementptr inbounds ([[DEP_LAMBDA_SIZE]], [[DEP_LAMBDA_SIZE]]* @[[LAMBDA_IN_DEP_X]]

        lambda_no_dep<int, double>(3, 5.5);
        // CHECK: define linkonce_odr spir_func void @_Z13lambda_no_depIidEvT_T0_(i32 %a, double %b)
        // CHECK: call spir_func void @puts(i8* getelementptr inbounds ([[NO_DEP_LAMBDA_SIZE]], [[NO_DEP_LAMBDA_SIZE]]* @[[LAMBDA_NO_DEP]]

        int a = 5;
        double b = 10.7;
        auto y = [](int a) { return a; };
        auto z = [](double b) { return b; };
        lambda_two_dep<decltype(y), decltype(z)>();
        // CHECK: define internal spir_func void @_Z14lambda_two_depIZZ4mainENKUlvE_clEvEUliE_ZZ4mainENKS0_clEvEUldE_Evv
        // CHECK: call spir_func void @puts(i8* getelementptr inbounds ([[DEP_LAMBDA1_SIZE]], [[DEP_LAMBDA1_SIZE]]* @[[LAMBDA_TWO_DEP]]

        lambda_two_dep<decltype(z), decltype(y)>();
        // CHECK: define internal spir_func void @_Z14lambda_two_depIZZ4mainENKUlvE_clEvEUldE_ZZ4mainENKS0_clEvEUliE_Evv
        // CHECK: call spir_func void @puts(i8* getelementptr inbounds ([[DEP_LAMBDA2_SIZE]], [[DEP_LAMBDA2_SIZE]]* @[[LAMBDA_TWO_DEP2]]
      });

  kernel_single_task<class kernel2>(func<Derp>);

  auto l1 = []() { return 1; };
  auto l2 = [](decltype(l1) *l = nullptr) { return 2; };
  kernel_single_task<class kernel3>(l2);
  puts(__builtin_unique_stable_name(l2));

  // TO-DO
  constexpr const char str[] = "lalala";
  static_assert(__builtin_strcmp(__builtin_unique_stable_name(str), "_ZTSA7_Kc\00") == 0, "unexpected mangling");

  // FIXME: Warn about expression with side effects in unevaluated context
  int i = 0;
  puts(__builtin_unique_stable_name(i++));

  // FIXME: Ensure that j is incremented because VLAs are terrible
  int j = 55;
  puts(__builtin_unique_stable_name(int[++j])); // No warning



}
