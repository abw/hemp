#ifndef HEMP_OS_H
#define HEMP_OS_H

#ifdef WIN32
# include <hemp/os/win32.h>
#else
# include <hemp/os/unix.h>
#endif

/* bits adapted from LuaJIT */
#define HEMP_ARCH_X86           32
#define HEMP_ARCH_X64           64
#define HEMP_ARCH_LITTLE_ENDIAN 1234
#define HEMP_ARCH_BIG_ENDIAN    4321


#ifndef HEMP_TARGET
    #if defined(i386) || defined(__i386) || defined(__i386__) || defined(_M_IX86)
        #define HEMP_TARGET     HEMP_ARCH_X86
    #elif defined(__x86_64__) || defined(__x86_64) || defined(_M_X64) || defined(_M_AMD64) || defined (_M_IA64)
        #define HEMP_TARGET     HEMP_ARCH_X64
    #else
        #error "No support for this architecture (yet)"
    #endif
#endif

#if HEMP_TARGET == HEMP_ARCH_X86
    #define HEMP_ARCHITECTURE   "x86"
    #define HEMP_WORD_LENGTH    32
#elif HEMP_TARGET == HEMP_ARCH_X64
    #define HEMP_ARCHITECTURE   "x64"
    #define HEMP_WORD_LENGTH    64
#else
    #error "No target architecture defined"
#endif

#if __LITTLE_ENDIAN__ || HEMP_TARGET == HEMP_ARCH_X86 || HEMP_TARGET == HEMP_ARCH_X64
    #define HEMP_BYTE_ORDER     HEMP_ARCH_LITTLE_ENDIAN
#elif __BIG_ENDIAN__
    #define HEMP_BYTE_ORDER     HEMP_ARCH_BIG_ENDIAN
#else
    #error "Can't determine the endian-ness of the target architecture"
#endif

#if HEMP_BYTE_ORDER == HEMP_ARCH_LITTLE_ENDIAN
    #define HEMP_LITTLE_ENDIAN  1
    #define HEMP_BIG_ENDIAN     0
    #define HEMP_ENDIAN         "little"
#elif HEMP_BYTE_ORDER == HEMP_ARCH_BIG_ENDIAN
    #define HEMP_LITTLE_ENDIAN  0
    #define HEMP_BIG_ENDIAN     1
    #define HEMP_ENDIAN         "big"
#else
    #error "No endianness defined for the target architecture"
#endif


#ifndef HEMP_INLINE
    #if defined __cplusplus
        #define HEMP_INLINE     inline
    #elif defined __GNUC__
        #define HEMP_INLINE     __inline__
    #elif defined _MSC_VER
        #define HEMP_INLINE     __inline
    #else
        #define HEMP_INLINE     inline
    # endif
#endif

#ifndef HEMP_DO_INLINE
    #if defined __GNUC__
        #define HEMP_DO_INLINE  __attribute__((always_inline)) HEMP_INLINE
    #elif defined _MSC_VER
        #define HEMP_DO_INLINE  __forceinline
    #else
        #define HEMP_DO_INLINE  HEMP_INLINE
    #endif
#endif

#ifndef HEMP_NO_INLINE
    #if defined __GNUC__
        #define HEMP_NO_INLINE  __attribute__((noinline))
    #elif defined _MSC_VER
        #define HEMP_NO_INLINE  __declspec(noinline)
    #else
        #define HEMP_NO_INLINE
    #endif
#endif


/*--------------------------------------------------------------------------
 * Thread locking.  Encapsulates a block of code with a locked mutex.
 * Cargo-culto from Nik Clayton's TAP library, but not really used.  Thread
 * safety is something that (will|may) come later.
 *--------------------------------------------------------------------------*/

#ifdef HEMP_HAVE_LIBPTHREAD
    #include <pthread.h>
    static  pthread_mutex_t M = PTHREAD_MUTEX_INITIALIZER;
    #define HEMP_MUTEX_LOCK     pthread_mutex_lock(&M);
    #define HEMP_MUTEX_UNLOCK   pthread_mutex_unlock(&M);
#else
    #define HEMP_MUTEX_LOCK
    #define HEMP_MUTEX_UNLOCK
#endif

#endif /* HEMP_OS_H */