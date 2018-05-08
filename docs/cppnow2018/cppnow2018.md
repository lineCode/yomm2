
# Open Is Good

===

## Case Study: AST

```C++
struct Node {
    virtual double value() = 0;
};
```

const, memory management etc elided, code formatted to "slide style"

---

## Case Study: AST

```C++
struct Number : Node {
  Number(double value) : val(value ) { }

  double value() override {
    return val;
  }

  double val;
};
```

---

## Case Study: AST

```C++
struct Plus : Node {
    Plus(Node& left, Node& right)
    : left(left), right(right) { }

  double value() override {
    return left.value() + right.value();
  }

  Node &left, &right;
};
```

---

## Case Study: AST

```C++
struct Times : Node {
    Times(Node& left, Node& right)
    : left(left), right(right) { }

    double value() override {
        return left.value() * right.value();
    }

    Node &left, &right;
};
```

---

## Case Study: AST

```C++
int main() {
    Node* expr =
        new Times(*new Number(2),
                  *new Plus(*new Number(3), *new Number(4)));

    cout << expr->value() << "\n"; // 14

    return 0;
}
```

---

## Case Study: AST

```C++
int main() {
    Node* expr =
        new Times(*new Number(2),
                  *new Plus(*new Number(3), *new Number(4)));

    cout /* << expr -> RPM */ << " = " << expr->value() << "\n";
    // 2 3 4 * + = 14

    return 0;
}
```

---

## AST: Add a Member Method?

```C++
struct Node {
    // as before
    virtual string toRPN() = 0;
};

struct Plus : Node {
    // as before
  string toRPN() override {
    return left.toRPN() + " " + right.toRPN() + " +";
  }
};

// same for Number and Times

```
_...if it needs to be virtual, make it a member function_ ???

banana -> gorilla -> jungle

---

## AST: Type Switch?

```C++
string toRPN(Node& node) {
    if (auto expr = dynamic_cast<Number*>(&node)) {
        return to_string(expr->value());
    } else if (auto expr = dynamic_cast<Plus*>(&node)) {
        return toRPN(expr->left) + " " + toRPN(expr->right) + " +";
    } else if (auto expr = dynamic_cast<Times*>(&node)) {
        return toRPN(expr->left) + " " + toRPN(expr->right) + " *";
    }
    throw runtime_error("unknown node type");
}
```
needs modification each time a new Node subtype is added

---

## AST: Visitor?

```C++

struct Node {
    // as before
    struct Visitor {
        virtual void accept(Number& expr) = 0;
        virtual void accept(Plus& expr) = 0;
        virtual void accept(Times& expr) = 0;
    };

    virtual void visit(Visitor& viz) = 0;
};
```

---

## AST: Visitor?

```C++
struct Number : Node {
    // as before
  void visit(Visitor& viz) override {
    viz.accept(*this);
  }
};

struct Plus : Node {
    // as before
  void visit(Visitor& viz) override {
    viz.accept(*this);
  }
};

struct Times : Node {
    // as before
    void visit(Visitor& viz) override {
        viz.accept(*this);
    }
};
```


---

## AST: Visitor?

```C++
struct RPNVisitor : Node::Visitor {
  void accept(Number& expr) {
    result = to_string(expr.val);
  }
  void accept(Plus& expr) {
    expr.left.visit(*this);
    string l = result;
    expr.right.visit(*this);
    result = l + " " + result + " +";
  }
  void accept(Times& expr) {
    expr.left.visit(*this);
    string l = result;
    expr.right.visit(*this);
    result = l + " " + result + " *";
  }
  string result;
};
```

ugh, yuck!

---

## AST: Visitor?

```C++

string toRPN(Node& node) {
  RPNVisitor viz;
  node.visit(viz);
  return viz.result;
}
```

my, that was a lot of work

and, what does it even gain us?

---

## AST: Function Table?

```C++
using RPNFormatter = string (*)(Node&);
unordered_map<type_index, RPNFormatter> RPNformatters;

string toRPN(Node& node) {
  return RPNformatters[typeid(node)](node);
}
```

---

## AST: Function Table?

```C++
namespace { struct Init {
  Init() {
    RPNformatters[typeid(Number)] = [](Node& node) {
     return to_string(static_cast<Number&>(node).val); };
    RPNformatters[typeid(Plus)] = [](Node& node) {
     auto expr = static_cast<Plus&>(node);
     return toRPN(expr.left) + " " + toRPN(expr.right) + " +"; };
    RPNformatters[typeid(Times)] = [](Node& node) {
     auto expr = static_cast<Times&>(node);
     return toRPN(expr.left) + " " + toRPN(expr.right) + " *"; };
  }
};
Init init;
} }
```

not bad, actually

===

## The Expression Problem

behaviors += types

types += behavior

---

## Multi-Layer Architectures

---

## presentation
---
## domain
---
## persistence

---

* presentation: PersonDlg, CriminalCaseDlg

* domain: Person, CriminalCase

* persistence: persist to database, to json...

===

## AST: Open Methods

```C++
#include <yorel/yomm2/cute.hpp>

using yorel::yomm2::virtual_;

register_class(Node);
register_class(Plus, Node);
register_class(Times, Node);
register_class(Number, Node);
```

---

## AST: Open Methods

```C++
declare_method(string, toRPN, (virtual_<const Node&>));

define_method(string, toRPN, (const Number& expr)) {
  return std::to_string(expr.val);
}

define_method(string, toRPN, (const Plus& expr)) {
  return toRPN(expr.left) + " " + toRPN(expr.right) + " +";
}

define_method(string, toRPN, (const Times& expr)) {
  return toRPN(expr.left) + " " + toRPN(expr.right) + " *";
}
```

---

## AST: what about value?

* `value` in the node hierarchy screams interpreter

* the AST classes should _only_ represent the tree

```C++
declare_method(int, value, (virtual_<const Node&>));

define_method(int, value, (const Number&& expr)) {
  return expr.val;
}

define_method(int, value, (const Plus& expr)) {
  return value(expr.left) + value(expr.right);
}

define_method(int, value, (const Times& expr)) {
  return value(expr.left) * value(expr.right);
}
```

## Multiple Dispatch?

Yes.

---

## Occasionally Useful

```text
add(Matrix, Matrix)                 -> Matrix
                                       add all elements
add(DiagonalMatrix, DiagonalMatrix) -> DiagonalMatrix
                                       just add diagonals

fight(Human, Creature, Axe)    -> not agile enough to wield
fight(Warrior, Creature, Axe)  -> chop it into pieces
fight(Warrior, Dragon, Axe)    -> die a honorable death
fight(Human, Dragon, Hands)    -> you just killed a dragon
                                  with your bare hands!
                                  incredible isn't it?
```

---

## Syntax

Just use `virtual_<>` on several arguments:

```C++
declare_method(
    std::string,
    fight,
    (virtual_<Character&>, virtual_<Creature&>, virtual_<Device&>));

define_method(std::string, fight, (Human& x, Creature& y, Axe& z)) {
  return "not agile enough to wield";
}

define_method(std::string, fight, (Human& x, Dragon& y, Hands& z)) {
  return "you just killed a dragon with your bare hands."
         " Incredible isn't it?";
}
```

---

## Selecting the right specialization

* works just like selecting from set of overloads (but at runtime!)

* or partial template specialization

* ambiguities can arise

===

## Is This OOP?

* brief history of OOP: Simula, Smalltalk, C++/D/Java/...

* CLOS: not objects talking to each other a la Smalltalk

* algorithms retake the front stage

* no unnecessary breach of encapsulation

===

## How Does It Work?

---

## A Payroll Application

* _Role_
  * Employee
    * Manager
  * Founder
* _Expense (X)_
  * Cab, Jet
  * _Public_
    * Bus, Train

---

## the `pay` (Uni-) Method

```C++`
declare_method(bool, approve,
  (virtual_<const Role&>, virtual_<const Expense&>, double));

define_method(double, pay, (const Employee&)) {
  return 3000;
}

define_method(double, pay, (const Manager& manager)) {
  return next(manager) + 2000;
}
```

---

## the `approve` (Multi-) Method

```C++
declare_method(bool, approve,
  (virtual_<const Role&>, virtual_<const Expense&>, double));

define_method(bool, approve,
  (const Role& r, const Expense& e, double amount))
{ return false; }

define_method(bool, approve,
  (const Employee& r, const Public& e, double amount))
{ return true; }

define_method(bool, approve,
  (const Manager& r, const Taxi& e, double amount))
{ return true; }

define_method(bool, approve,
  (const Founder& r, const Expense& e, double amount))
{ return true; }
```

---

## `declare_method`

```C++
declare_method(double, pay, (virtual_<const Employee&>));
```


```C++
struct _yomm2_method_pay;

namespace {
namespace YoMm2_nS_10 {
using _yOMM2_method =
  method<void, _yomm2_method_pay,
    double(virtual_<const Employee &>),
    default_policy>;
_yOMM2_method::init_method init;
}
}
```

---

## `declare_method`

```C++
declare_method(double, pay, (virtual_<const Employee&>));
```


```C++
YoMm2_nS_10::_yOMM2_method
pay(discriminator, const Employee & a0);

inline double
pay(const Employee & a0) {
  auto pf = reinterpret_cast<double (*)(
    const Employee & a0)>(
    YoMm2_nS_10::_yOMM2_method::resolve(a0));
  return pf(std::forward<const Employee &>(a0));
};

//
```

---

## `define_method`

```C++
define_method(double, pay, (const Employee&))
{ return 3000; }
```

```C++
namespace { namespace YoMm2_nS_12 {
template <typename T> struct select;
template <typename... A> struct select<void(A...)> {
  using type = decltype(
    pay(discriminator(), std::declval<A>()...));
};

using _yOMM2_method =
  select<void(const Employee &)>::type;
using _yOMM2_return_t = _yOMM2_method::return_type;
_yOMM2_return_t (*next)(const Employee &);
```

---

## `define_method`

```C++
struct _yOMM2_spec {
  static double body(const Employee &);
};
register_spec<_yOMM2_return_t, _yOMM2_method,
                    _yOMM2_spec, void(const Employee &)>
_yOMM2_init((void **)&next, "(const Employee&)");
} }

double YoMm2_nS_12::_yOMM2_spec::body(const Employee &)
{ return 3000; }
```

---

## update_methods

* scoops all the info registered by static ctors

* find all the relevant classes and interfaces via RT introspection

* builds all the dispatch data inside a single vector

* pointers to method tables are stored in rarely used `deallocator`

---

## Dispatching a Uni-Method

* pretty much like virtual member functions in fact

* method table contains a pointer to the effective function

* only it is not at a fixed offset in the method table

---

## Dispatching a Uni-Method

```C++
Method!(..., "pay", ...).slotStrides = { 1 };

// method table for Employee
typeid(Employee).deallocator = {
  ..., // used by approve,
  _pay(Employee)
};

// method table for Manager
typeid(Manager).deallocator = {
  ..., // used by approve,
  _pay(Manager)
};
```


---

## Dispatching a Uni-Method

```C++
pay(r)
```
=>
```C++
typeid(r).deallocator[Method!(pay).slotStrides[0]](e)
                      ^^^^^^^^^^^^^^^^^^^^^^^^^^^
                                = 1
```

---

## Performance?

```C++d
string payday(Employee e) { return pay(e); }
```

```asm
;;; g++-6 -DNDEBUG -O2 ...
   	movq	dispatch_data(%rip), %rax              ; hash table
	movb	dispatch_data+32(%rip), %cl            ; shift factor
	movslq	method<pay>::slots_strides(%rip), %rdx ; slot
	movq	(%rdi), %rsi                           ; vptr
	movq	-8(%rsi), %rsi                         ; type_info ptr
	imulq	dispatch_data+24(%rip), %rsi           ; hash: multiply
	shrq	%cl, %rsi                              ; hash: shift
	movq	(%rax,%rsi,8), %rax                    ; method table
	jmpq	*(%rax,%rdx,8)          # TAILCALL     ; call function
```

---

## Dispatching a Multi-Method

* it's a little more complicated

* need a multi-dimensional dispatch table

* size can grow very quickly

* the table must be "compressed", devoid of redundancies

* in fact the "uncompressed" table never exists

* `updateMethods` works in terms of class _groups_, not classes

---

## Dispatching a Multi-Method

|          | Jet+Expense  | Bus+Train     | Cab |
|----------|:------------:|:-------------:|:---:|
| Role     | R,X          | R,X           | R,X |
| Employee | R,X          | E,P           | R,X |
| Manager  | R,X          | E,P           | M,C |
| Founder  | F,X          | F,X           | F,X |

(column major)

---

## Dispatching a Multi-Method

```C++
Method!(..., "approve", ...).slotStrides = { 0, 3, 0 };

typeid(Employee).deallocator = {
  // & of (Employee,Jet) cell
  // used by pay
};

typeid(Manager).deallocator = {
  // & of (Manager,Jet) cell
  // used by pay
};

typeid(Cab).deallocator = { 2 };
```

---

## Dispatching a Multi-Method

```C++
approve(r, x)
```
=>
```C++
auto slotStrides = Method!(..., "approve", ...).slotStrides;

typeid(r).deallocator[slotStrides[0]]
  [ typeid(x).deallocator[slotStrides[2]]
    * slotStrides[1] ](r, x)
```

---

## Performance Summary

|                   | versus       |      via  | delta |
|-------------------|--------------|-----------|-------|
| virtual function  | 1-method     | object    |   37% |
| virtual function  | 1-method     | interface |  125% |
| double dispatch   | 2-method     | objects   |   28% |

---

## What if `deallocator` goes away?

* plan B: use perfect integer hash of vptr

* yes, it exists for any set of ints

* H(x) = (x * M) >> N

* mptr = mptrs[H(\*(cast(void**) obj))]

* find M and N via random search

* performance: quite good (+85%, +140%)

===

## What's Next?

---

## improve speed

* in particular calls via interface

* in fact I spotted an opportunity for speeding up calls to uni-methods while
  making these slides...maybe

---

## nice syntax for templatized methods

* ~~please fix static foreach on module members (issue 18698)~~ thanks Timon

* richer compile time reflection for templates (PR 9608)

---

## get rid of mixin(registerMethods)

* need new language feature

* DIP: call function at compile time in each importer module (a la Perl's `sub
  import`)

* could be used to move runtime info support out of the compiler?  (Ali)

---

## get rid of updateMethods

* DIP: ordering of static ctors?

* or try <font>Lu&iacute;s'</font> idea using crt_constructors

---

## allow cyclic dependencies?

* one of  <font>Lu&iacute;s'</font> suggestions - hmmm

* get rid of all static ctors

---

## dispatch hooks

* one of <font>Lu&iacute;s'</font> ideas - he wants to dispatch based, not on
  the type of an object, but on the type of one of its fields

* sounds very specific to me, but a mechanism for interfacing with smart
  pointers would work just like this

* is it possible to specialize a template from outside of its module?

---

## Inheritance-less Polymorphism?

* golang-style duck typing

* probably fairly easy to implement

---

## Remove Magic from D

remove virtual functions!

===

## links

* dub: https://code.dlang.org/packages/openmethods

* github: https://github.com/jll63/openmethods.d

* article on D blog: https://dlang.org/blog/2017/08/28/open-methods-from-c-to-d/

* this presentation: https://jll63.github.io/openmethods.d/dconf2018/
