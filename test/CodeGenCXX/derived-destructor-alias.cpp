// RUN: %clang_cc1 %s -O1 -mconstructor-aliases -emit-llvm -o - | FileCheck %s

namespace ns {
  struct Base {
    virtual ~Base();
  };

  Base::~Base() { }

  template<typename T>
  struct Template {
    struct NestedDerived : public Base {
      virtual ~NestedDerived() { }
    };
  };
}

namespace ns {
  template class Template<int>;
  // CHECK: @_ZN2ns8TemplateIiE13NestedDerivedD2Ev = alias weak_odr bitcast (void (%"struct.ns::Base"*)* @_ZN2ns4BaseD2Ev to void (%"struct.ns::Template<int>::NestedDerived"*)*)
}
