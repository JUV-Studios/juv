#ifndef JUV_MACROS
#define JUV_MACROS

#include <type_traits>

#define JUV_DERIVED static_cast<std::conditional_t<std::is_const_v<std::remove_pointer_t<decltype(this)>>, Derived const&, Derived&>>(*this)

#endif