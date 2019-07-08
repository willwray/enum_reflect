#include <iostream>
#include "enumio.hpp"

enum class X { Alicia, Boole, Bool=Boole, Stott, Doe=0xFF,
               Min16 = INT16_MIN, Max16 = INT16_MAX,
               Min32 = INT32_MIN, Max32 = INT32_MAX };

constexpr auto decl_X = enumio::enum_declaration<X>;

int main() { std::puts( decl_X ); }
