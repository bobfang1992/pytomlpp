#ifndef PYTOMLPP_COMPILERS_HPP
#define PYTOMLPP_COMPILERS_HPP

// clang
#ifdef __clang__
#define PYTOMLPP_CLANG __clang_major__
#define PYTOMLPP_PRAGMA_CLANG(...) _Pragma(__VA_ARGS__)
#else
#define PYTOMLPP_CLANG 0
#define PYTOMLPP_PRAGMA_CLANG(...)
#endif

// intel icc
#ifdef __INTEL_COMPILER
#define PYTOMLPP_ICC __INTEL_COMPILER
#ifdef __ICL
#define PYTOMLPP_ICC_CL PYTOMLPP_ICC
#else
#define PYTOMLPP_ICC_CL 0
#endif
#else
#define PYTOMLPP_ICC 0
#define PYTOMLPP_ICC_CL 0
#endif

// msvc
// note: other compilers emulate MSVC; check must explicitly exclude them
#if defined(_MSC_VER) && !PYTOMLPP_CLANG && !PYTOMLPP_ICC
#define PYTOMLPP_MSVC _MSC_VER
#define PYTOMLPP_PRAGMA_MSVC(...) __pragma(__VA_ARGS__)
#else
#define PYTOMLPP_MSVC 0
#define PYTOMLPP_PRAGMA_MSVC(...)
#endif

// gcc
// note: other compilers emulate GCC; check must explicitly exclude them
#if defined(__GNUC__) && !PYTOMLPP_CLANG && !PYTOMLPP_ICC
#define PYTOMLPP_GCC __GNUC__
#define PYTOMLPP_PRAGMA_GCC(...) _Pragma(__VA_ARGS__)
#else
#define PYTOMLPP_GCC 0
#define PYTOMLPP_PRAGMA_GCC(...)
#endif

#define PYTOMLPP_PUSH_OPTIMIZATIONS                                            \
  PYTOMLPP_PRAGMA_MSVC(optimize("gt", on))                                     \
  PYTOMLPP_PRAGMA_MSVC(runtime_checks("", off))                                \
  PYTOMLPP_PRAGMA_MSVC(strict_gs_check(push, off))                             \
  PYTOMLPP_PRAGMA_MSVC(inline_recursion(on))                                   \
  PYTOMLPP_PRAGMA_MSVC(inline_depth(255))                                      \
  static_assert(true)

#define PYTOMLPP_POP_OPTIMIZATIONS                                             \
  PYTOMLPP_PRAGMA_MSVC(inline_recursion(off))                                  \
  PYTOMLPP_PRAGMA_MSVC(strict_gs_check(pop))                                   \
  PYTOMLPP_PRAGMA_MSVC(runtime_checks("", restore))                            \
  PYTOMLPP_PRAGMA_MSVC(optimize("", on))                                       \
  static_assert(true)

#endif // PYTOMLPP_COMPILERS_HPP
