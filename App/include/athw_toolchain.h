#ifndef __athw_toolchain_h
#define __athw_toolchain_h

#include <string.h>

#if !defined(__GNUC__)   /* GCC        */ \
 && !defined(__clang__)  /* LLVM/Clang */ \
 && !defined(__ICCARM__) /* IAR        */
#error "This compiler is not yet supported."
#endif



/** ATHW_PACKED
 *  Pack a structure, preventing any padding from being added between fields.
 *
 *  @code
 *  #include "ATHW_toolchain.h"
 *
 *  ATHW_PACKED(struct) foo {
 *      char x;
 *      int y;
 *  };
 *  @endcode
 */
#ifndef ATHW_PACKED
#if defined(__ICCARM__)
#define ATHW_PACKED(struct) __packed struct
#else
#define ATHW_PACKED(struct) struct __attribute__((packed))
#endif
#endif


/** ATHW_ALIGN(N)
 *  Declare a variable to be aligned on an N-byte boundary.
 *
 *  @note
 *  IAR does not support alignment greater than word size on the stack
 *
 *  @code
 *  #include "ATHW_toolchain.h"
 *
 *  ATHW_ALIGN(16) char a;
 *  @endcode
 */
#ifndef ATHW_ALIGN
#if __cplusplus >= 201103
#define ATHW_ALIGN(N) alignas(N)
#elif __STDC_VERSION__ >= 201112
#define ATHW_ALIGN(N) _Alignas(N)
#elif defined(__ICCARM__)
#define ATHW_ALIGN(N) _Pragma(ATHW_STRINGIFY(data_alignment=N))
#else
#define ATHW_ALIGN(N) __attribute__((aligned(N)))
#endif
/** ATHW_UNUSED
 *  Declare a function argument to be unused, suppressing compiler warnings
 *
 *  @code
 *  #include "ATHW_toolchain.h"
 *
 *  void foo(ATHW_UNUSED int arg) {
 *
 *  }
 *  @endcode
 */
#ifndef ATHW_UNUSED
#if defined(__GNUC__) || defined(__clang__)
#define ATHW_UNUSED __attribute__((__unused__))
#else
#define ATHW_UNUSED
#endif
#endif

/** ATHW_USED
 *  Inform the compiler that a static variable is to be retained in the object file, even if it is unreferenced.
 *
 *  @code
 *  #include "ATHW_toolchain.h"
 *
 *  ATHW_USED int foo;
 *
 *  @endcode
 */
#ifndef ATHW_USED
#if defined(__GNUC__) || defined(__clang__)
#define ATHW_USED __attribute__((used))
#elif defined(__ICCARM__)
#define ATHW_USED __root
#else
#define ATHW_USED
#endif
#endif

/** ATHW_WEAK
 *  Mark a function as being weak.
 *
 *  @note
 *  Functions should only be marked as weak in the source file. The header file
 *  should contain a regular function declaration to insure the function is emitted.
 *  A function marked weak will not be emitted if an alternative non-weak
 *  implementation is defined.
 *
 *  @note
 *  Weak functions are not friendly to making code re-usable, as they can only
 *  be overridden once (and if they are multiply overridden the linker will emit
 *  no warning). You should not normally use weak symbols as part of the API to
 *  re-usable modules.
 *
 *  @code
 *  #include "ATHW_toolchain.h"
 *
 *  ATHW_WEAK void foo() {
 *      // a weak implementation of foo that can be overriden by a definition
 *      // without  __weak
 *  }
 *  @endcode
 */
#ifndef ATHW_WEAK
#if defined(__ICCARM__)
#define ATHW_WEAK __weak
#elif defined(__MINGW32__)
#define ATHW_WEAK
#else
#define ATHW_WEAK __attribute__((weak))
#endif
#endif

/** ATHW_PURE
 *  Hint to the compiler that a function depends only on parameters
 *
 *  @code
 *  #include "ATHW_toolchain.h"
 *
 *  ATHW_PURE int foo(int arg){
 *      // no access to global variables
 *  }
 *  @endcode
 */
#ifndef ATHW_PURE
#if defined(__GNUC__) || defined(__clang__)
#define ATHW_PURE __attribute__((const))
#else
#define ATHW_PURE
#endif
#endif

/** ATHW_NORETURN
 *  Declare a function that will never return.
 *
 *  @code
 *  #include "ATHW_toolchain.h"
 *
 *  ATHW_NORETURN void foo() {
 *      // must never return
 *      while (1) {}
 *  }
 *  @endcode
 */
#ifndef ATHW_NORETURN
#if __cplusplus >= 201103
#define ATHW_NORETURN [[noreturn]]
#elif __STDC_VERSION__ >= 201112
#define ATHW_NORETURN _Noreturn
#elif defined(__GNUC__) || defined(__clang__)
#define ATHW_NORETURN __attribute__((noreturn))
#elif defined(__ICCARM__)
#define ATHW_NORETURN __noreturn
#else
#define ATHW_NORETURN
#endif
#endif

#ifndef WEAK
#define WEAK ATHW_WEAK
#endif

#ifndef PACKED
#define PACKED ATHW_PACKED()
#endif

#ifndef EXTERN
#define EXTERN extern
#endif

#endif
