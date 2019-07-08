//  Copyright (c) 2019 Will Wray https://keybase.io/willwray
//
//  Repo: https://github.com/willwray/enum_reflect

#ifndef ENUMIO_HPP
#define ENUMIO_HPP

#include "enumerator_traits.hpp"
#include "type_name_pp.hpp"

namespace enumio {

enum format : uint8_t { id=1, hex, id_eq_hex, dec, id_eq_dec };

using ltl::ntbs::cat; // concatenate strings, ntbs a dependency of type_name_pp

template <bool C, typename A, typename B = char const(&)[1]>
constexpr auto cat_if(A const& a, B const& b = "")
{
    if constexpr ( C ) return cat(a); else return cat(b);
}

constexpr int digits_count(uint32_t num, int base)
{
    for (uint64_t oom = base, d = 1; ; oom *= base, ++d)
        if ( num < oom )
            return d;
}

template <auto v, bool DecOrHex = true>
inline constexpr auto to_chars = []
{
    static_assert( std::is_integral_v<decltype(v)> && sizeof v <= 4);
    constexpr uint32_t u = v < 0 ? -v : v; // This is a safe 'abs' negate as
                                           // INT32_MIN is special-cased below
    constexpr int base = DecOrHex ? 10 : 0x10;
    constexpr int num_digits = digits_count(u,base);

    ltl::ntbs::array<num_digits + 1> ret{};
    for ( uint32_t x = u, i = num_digits; i; x /= base)
        ret[--i] = x % base + (x % base < 10 ? '0' : 'A' - 10);
    return ret;
}();

template<>inline constexpr auto to_chars<INT32_MIN,false> = cat("7FFFFFFF-1");
template<>inline constexpr auto to_chars<INT32_MIN,true> = cat("2147483647-1");

template <auto e, format f = id>
inline constexpr auto enumerator = []
{
    static_assert( std::is_enum_v<decltype(e)> && ltl::is_enumerated_v<e> );
    constexpr auto uv = +ltl::to_underlying(e); // special-case char ?

    auto enum_id = cat_if<bool(f & id)>( ltl::auto_name_pu<e> );
    auto equals = cat_if<f & id && (f & hex || f & dec)>( '=' );
    auto val_str = cat_if<f & hex || f & dec>( cat( cat_if<uv < 0>('-'),
                   cat_if<bool(f & hex)>("0x"),to_chars<uv,bool(f & dec)> ));

    return cat(enum_id, equals, val_str);
}();

template <typename E, format f, char... seps>
struct enumerators_t
{
  template <std::size_t... I>
  static constexpr auto str( std::index_sequence<I...> ) {
      return cat<seps...>( enumerator<ltl::enumerators_v<E>[I],f>... );
  }
  static constexpr auto str() {
      return str( typename ltl::enumerators<E>::index_sequence{} );
  }
};
template <typename E, format f, char... seps>
inline constexpr auto enumerators = enumerators_t<E,f,seps...>::str();

template <typename E, bool DecOrHex = true, char... s>
inline constexpr auto enum_declaration = []
{
    using U = std::underlying_type_t<E>;
    constexpr format f = DecOrHex ? id_eq_dec : id_eq_hex;

    constexpr auto ul_type_name = ltl::type_name_pp<U>;
    constexpr bool fixed = ltl::is_fixed_enum_v<E> && ! std::is_same_v<U,int>;
    constexpr auto uqname = ltl::type_name_pu<E>;

    constexpr auto _class = cat_if<ltl::is_scoped_enum_v<E>>(" class");
    constexpr auto _name  = cat_if<sizeof(uqname)!=0>(cat(' ',uqname));
    constexpr auto _fixed = cat_if<fixed>(cat(" : ", ul_type_name));
    constexpr auto enum_ids = enumerators<E,f,',',s...>;

    return cat("enum",_class,_name,_fixed," {",enum_ids,'}');
}();

} // namespace enumio

#endif // ENUMIO_HPP
