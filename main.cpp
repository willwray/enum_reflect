//#include <cstdio>
#include <iostream>

#include "enumio.hpp"

enum class X { Min32 = INT32_MIN, Min16 = INT16_MIN,
               Alicia=0, Boole, Bool=Boole, Stott, Doe=0xFF,
               Max16 = INT16_MAX, Max32 = INT32_MAX };
int main()
{
    using namespace enumio;

    std::cout << enumerator<X(0)>;

    std::cout << "\n\n" << enumerators<X,id_eq_hex,'\n'> << "\n\n";

    constexpr auto decl = enum_declaration<X>;
    std::puts( decl );
}
