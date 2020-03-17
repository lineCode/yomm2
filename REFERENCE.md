# Reference

## header yorel/yomm2.hpp

#### Synopsis:
```
#include <yorel/yomm2.hpp>
```

This is the library's main header. It defines the YOMM2_* macros, and namespace
yorel::yomm2 and its content.

## header yorel/yomm2/cute.hpp

#### Synopsis:
```
#include <yorel/yomm2/cute.hpp>
```

Include `yorel/yomm2.hpp` and define nicer, lower case synonyms for the macros:

* `register_class` for `YOMM2_CLASS`
* `declare_method` for `YOMM2_DECLARE`
* `define_method` for `YOMM2_DEFINE`
* `end_method` for `YOMM2_END`

It is recommended to use the "cute" names unless they clash with names used in
existing code.

## namespace yorel::yomm2

#### Synopsis:
```
using yorel::yomm2::virtual_;
```

This is the library's public namespace. It contains only a five public names:
* template `virtual_`
* function `update_methods`
* struct `method_call_error`
* typedef `method_call_error_handler`
* function `set_method_call_error_handler`

There is little point in `using namespace yorel::yomm2`. It is recommended to
alias `virtual_` and access the other symbols by their fully qualified name.

## function yorel::yomm2::update_methods()

#### Synopsis:
```
int main(int argc, const char** argv) {
    yorel::yomm2::update_methods();
    // ...
}
```

Create tables used during method dispatch.

This function must be called before any method is called (typically in
`main`). It must also be called after a shared library is dynamically loaded or
unloaded, if the library adds method declarations, method definitions, or
classes derived from classes that are used as virtual arguments.

## macros YOMM2_CLASS, register_class

#### Synopsis:
```
YOMM2_CLASS(polymorphic_class);
YOMM2_CLASS(polymorphic_class, polymorphic_base_class...);
```

Register a class.

Every class that potentially takes part in a method call must be registered
with `YOMM2_CLASS`. This means all classes that are marked with `virtual_`, and
all their subclasses. Each class registration must list the base classes that
may be involved in method calls.

`register_class` is an alias for `YOMM2_CLASS` provided by header
`yorel/yomm2/cute.hpp`.

#### Examples:
```
struct Animal {
    virtual ~Animal() {}
};

struct Mammal    : virtual Animal { ... };
struct Carnivore : virtual Animal { ... };
struct Dog       : Mammal, Carnivore { .. };

YOMM2_CLASS(Animal);
YOMM2_CLASS(Carnivore, Animal);
YOMM2_CLASS(Mammal, Animal);
YOMM2_CLASS(Dog, Carnivore, Mammal);
```

## template virtual_

#### Synopsis:
```
virtual_<type>
```
Mark a parameter as virtual. Meaningful only inside a method declaration parameter list.

`type` must be a reference, a rvalue reference, a pointer or a
`std::shared_ptr` to a polymorphic type, possibly qualified with `const`.

## Examples:
```
using yorel::yomm2::virtual_;

struct Animal {
    virtual ~Animal();
};

YOMM2_DECLARE(void, kick, (virtual_<Animal*>));
YOMM2_DECLARE(void, kick, (virtual_<Animal&>));
YOMM2_DECLARE(void, kick, (virtual_<Animal&&>));
YOMM2_DECLARE(void, kick, (virtual_<shared_ptr<Animal>>));
YOMM2_DECLARE(void, kick, (virtual_<const shared_ptr<Animal>&>));
YOMM2_DECLARE(void, kick, (virtual_<const Animal*>));
YOMM2_DECLARE(void, kick, (virtual_<const Animal&>));
YOMM2_DECLARE(void, kick, (virtual_<shared_ptr<const Animal>>));
YOMM2_DECLARE(void, kick, (virtual_<const shared_ptr<const Animal>&>));

```

Given a polymorphic class `Animal`, these are all the valid ways of specifying
a virtual Animal argument in a method declaration.

## macros YOMM2_DECLARE, declare_method

#### Synopsis:
```
YOMM2_DECLARE(return_type, method, (type...));

return_type rv = method(unspecified_type... arg);

```

Declare a method.

Create an inline function `method` that returns a `return type` and takes an
argument list of `unspecified_type...`, which consist of `type...` without the
`virtual_` marker. At least one `type` (but not necessarily all) must be marked
with `virtual_`.

When `method` is called, the dynamic types of the arguments marked with
`virtual_` are examined, and the most specific definition compatible with
`unspecified_type...` is called. If no compatible definition exists, or if
several compatible definitions exist but none of them is more specific than
all the others, the call is illegal and an error handler is executed. By
default it writes a diagnostic on `std::cerr ` and terminates the program via
`abort`. The handler can be customized.

NOTE:

* The parameter list `type...` _must_ be surrounded by parentheses.

* The parameters in `type...` consist of _just_ a type, e.g. `int` is correct
  but `int i` is not.

`declare_method` is an alias for `YOMM2_DECLARE` provided by header
`yorel/yomm2/cute.hpp`.

## Examples:
```
YOMM2_DECLARE(std::string, kick, (virtual_<Animal&>));
YOMM2_DECLARE(std::string, meet, (virtual_<Animal&>, virtual_<Animal&>));
YOMM2_DECLARE(bool, approve, (virtual_<Role&>, virtual_<Expense&>), double);
```

## macros YOMM2_METHOD, define_method

#### Synopsis:
```
YOMM2_DEFINE(return_type, name, (unspecified_type... argument)) {
    ....
}

YOMM2_DEFINE(container, return_type, name, (unspecified_type... argument)) {
    ....
}
```

Add an implementation to a method.

Locate a method that can be called with the specified `unspecified_type...` list
and add the definition to the method's list of definitions. The method must
exist and must be unique. `return_type` must be covariant with the method's
return type. `return_type` may be `auto`.

NOTE that the types of the arguments are _not_ marked with `virtual_`.

If `container` is specified, the method definition is placed inside the said
container, which must have been declared with `YOMM2_DECLARE_METHOD_CONTAINER`
or `method_container`. See the documentation of
`YOMM2_DECLARE_METHOD_CONTAINER` for more information on method containers.


`define_method` is an alias for `YOMM2_DEFINE`, provided by header
`yorel/yomm2/cute.hpp`.

## Examples:
```
// implement 'kick' for dogs
YOMM2_DEFINE(std::string, kick, (Dog& dog)) {
  return "bark";
}

// implement 'kick' for bulldogs
YOMM2_DEFINE(std::string, kick, (Bulldog& dog)) {
    return next(dog) + " and bite";
}

// 'meet' catch-all implementation
YOMM2_DEFINE(std::string, meet, (Animal&, Animal&)) {
  return "ignore";
}

YOMM2_DEFINE(std::string, meet, (Dog& dog1, Dog& dog2)) {
  return "wag tail";
}

YOMM2_DEFINE(std::string, meet, (Dog& dog, Cat& cat)) {
  return "chase";
}

YOMM2_DEFINE(std::string, meet, (Cat& cat, Dog& dog)) {
  return "run";
}
```

## macros YOMM2_METHOD_INLINE, define_method_inline

#### Synopsis:
```
YOMM2_DEFINE_INLINE(container, return_type, name, (unspecified_type... argument)) {
    ....
}
```

Add an implementation to a method, inside a container, and make it inline.

Like the `YOMM2_DEFINE(container, ...)` macro, define a method inside the
specified container, which must have been declared with
`YOMM2_DECLARE_METHOD_CONTAINER` or `method_container`. The definition has the
`inline` storage class, and thus can be placed in a header file and is a
potential candidate for inlining.

See the documentation of `YOMM2_DECLARE_METHOD_CONTAINER` for more information
on method containers.

`define_method_inline` is an alias for `YOMM2_DEFINE_INLINE`, provided by
header `yorel/yomm2/cute.hpp`.

## macros YOMM2_DECLARE_METHOD_CONTAINER, method_container

#### Synopsis:

```
YOMM2_DECLARE_METHOD_CONTAINER(container)
```

Declare a method container.

Method containers are collections of method definitions that can be addressed
by their name, return type and signature. This makes it possible for a class to
grant friendship to all the methods inside a container, or to a single method
with a specific name, return type and signature - see `YOMM2_FRIEND`. It also
makes it possible to retrieve a specific method, in order to call it or create
a pointer to it - see `YOMM2_DEFINITION`. See [friendship](examples/friendship)
for an example.

Method containers are implemented as templates, and method definitions scoped
inside containers are implemented as template specializations. Thus methods can
only be added to a container defined in the same namespace, or a namespace
nested inside the namespace where the container has been declared.

`method_container` is an alias for `YOMM2_DECLARE_METHOD_CONTAINER`, provided
by header `yorel/yomm2/cute.hpp`.

## macros YOMM2_FRIEND, friend_method

#### Synopsis:

```
YOMM2_FRIEND(container)
YOMM2_FRIEND(container, return_type, (unspecified_type... argument))
```

Grant friendship to all the methods inside a container friend of a class, or to
a specific method. See [friendship](examples/friendship) for an example.

`friend_method_container` is an alias for `YOMM2_FRIEND`, provided by header
`yorel/yomm2/cute.hpp`.

## macros YOMM2_DEFINITION, method_definition

#### Synopsis:

```
YOMM2_DEFINITION(container, return_type, (unspecified_type... argument))
```

Retrieve a method definition with a given return type and signature from a
container.

The resulting method can be used as a normal function reference. It can be
called, or its address can be taken. In particular, this makes it possible for
a method definition to call a base method as part of its implementation, in the
same manner as an ordinary virtual function can call a specific base function
by prefixing its name with a base class name.

Note that the preferred way of calling the overriden method is via `next`. In
normal circumstances, a method definition cannot assume which "super" or "base"
function is the best choice, since the set of methods pertaining to the same
declaration is open.

See [friendship](examples/friendship) for an example.

`method_definition` is an alias for `YOMM2_DEFINITION`, provided by header
`yorel/yomm2/cute.hpp`.

## function next

#### Synopsis:
```
YOMM2_DEFINE(return_type, name, (unspecified_type... arg)) {
    ....
    next(arg...);
    ...
}
```

Call the next most specific implementation, if it exists. Valid only inside a
method implementation.

`next` calls the implementation that would have been called if the calling
implementation had not been added. The next implementation may not exist,
either because no implementation was registered for a less specific set of
virtual arguments, or because several such implementations exist but none of
them is more specific than all the others. In such cases, calling `next` is
illegal and an error handler is executed. By default it writes a diagnostic on
`std::cerr` and terminates the program via `abort`. The handler can be
customized. Note that if is _not_ possible to detect whether calling `next` is
legal by testing `next` against `nullptr`.

#### Example:

```
// Executive is-a Employee

YOMM2_DEFINE(double, pay, (const Employee&)) {
    return 3000;
}

YOMM2_DEFINE(double, pay, (const Executive& exec)) {
    return next(exec) // call pay(const Employee&)
           + 2000;    // bonus
}

const Employee& elon = Executive();
double paycheck = pay(elon); // 3000 + 2000
```

## struct method_call_error

If a method call cannot be dispatched, an error handler is called with a
reference to a `method_call_error` structure. It contains one documented
field - `code` - that can take two values: `not_implemented` and `ambiguous`,
which are symbolic constants declared inside `method_call_error`.

## type method_call_error_handler

This is the type of the function called in case a method call fails.

## function set_method_call_error_handler

Set the function to call in case a method cannot be dispatched. Take a
`method_call_error_handler` and return the previous
`method_call_error_handler`.

The signature of the handler must be `void(const yorel::yomm2::method_call_error& error)`.

The handler _may_ _not_ return; it must either terminate the process, or throw an exception.

#### Example
```
void my_handler(const yorel::yomm2::method_call_error& error) {
    if (error_code == yorel::yomm2::method_call_error::not_implemented) {
        throw std::runtime_error("not implemented!");
    } else {
        // error_code == yorel::yomm2::method_call_error::ambiguous
        throw std::runtime_error("ambiguous!");
    }
}
```
