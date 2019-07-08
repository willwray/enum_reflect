# **`enum_reflect`** demo repo for `enum_traits`

This repo contains `enumio.hpp`, ~100 lines of code to demonstrate usage of [`enum_traits`](https://github.com/willwray/enum_traits)  
library header `enumerator_traits.hpp` for reflecting enumerated values of an enum type.  
It also uses the [`type_name_pp`](https://github.com/willwray/type_name_pp) library to pretty-print type names and enum identifiers,  
which in turn depends on the [`ntbs`](https://github.com/willwray/ntbs) library for 'constexpr C-string' handling.

In total, there's ~1000 lines of code. C++17. GCC >= 9 or Clang (no MSVC yet).  
Here's a link to the v0.1 code on [Wandbox online compiler](https://wandbox.org/permlink/ENMqHo5uVxPncJ4b) for you to play with.  
It takes a couple of seconds to compile (may be antisocial to put on Godbolt).

Given this definition of enum X:

```c++
enum class X { Alicia, Boole, Bool=Boole, Stott, Doe=0xFF,
               Min16 = INT16_MIN, Max16 = INT16_MAX,
               Min32 = INT32_MIN, Max32 = INT32_MAX };
```

then this program:

```c++
#include "enumio.hpp"
constexpr auto decl_X = enumio::enum_declaration<X>;

#include <cstdio>
int main() { std::puts( decl_X ); }
```

prints this 'code-gen' for the enum definition:

```c++
enum class X {Min32=-2147483647-1,Min16=-32768,Alicia=0,Boole=1,
Stott=2,Doe=255,Max16=32767,Max32=2147483647}
```

Sorted!  
Here's the code for the `enum_declaration` variable template:

```c++
template <typename E, bool DecOrHex = true, char... s>
inline constexpr auto enum_declaration = []
{
    using U = std::underlying_type_t<E>;
    constexpr format f = DecOrHex ? id_eq_dec : id_eq_hex;

    constexpr auto ul_type_name = ltl::type_name_pp<U>;
    constexpr bool fixed = ltl::is_fixed_enum_v<E>
                            && ! std::is_same_v<U,int>;
    constexpr auto uqname = ltl::type_name_pu<E>;

    constexpr auto _class = cat_if<ltl::is_scoped_enum_v<E>>(" class");
    constexpr auto _name  = cat_if<sizeof(uqname)!=0>(cat(' ',uqname));
    constexpr auto _fixed = cat_if<fixed>(cat(" : ", ul_type_name));
    constexpr auto enum_ids = enumerators<E,f,',',s...>;

    return cat("enum",_class,_name,_fixed," {",enum_ids,'}');
}();
```

The optional template Args allow to print hexadecimal values or add spacing.

A few points to note:

* Completely constexpr; `decl_X` is compile-time generated.
* For enumerators with duplicate value, like `Boole` and `Bool`,  
only the first-declared id is reflected (on GCC & Clang -  
MSVC currently fails to reflect duplicate-value enumerators).
* `INT32_MIN` is output as `-INT32_MAX-1` (correct code-gen).
* It looks like the full range of a 32-bit underlying value is checked.  
Can this be true?  
No; read the docs.

----

## Build

Meson build script provided, e.g. use with ninja backend  
(Meson will clone the three dependency repos, assuming network access).

```bash
meson build
ninja -C build
```
[![Build Status](https://travis-ci.org/willwray/enum_traits.svg?branch=master)](https://travis-ci.org/willwray/enum_traits)  
Linux Travis gcc-9, clang-7-libc++, -std=c++17