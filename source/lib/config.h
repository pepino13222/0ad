#if defined(_WIN32)
# define OS_WIN
#elif defined(linux)
# define OS_LINUX
#elif defined(macintosh)
# define OS_MACOS
#elif defined(__APPLE__)  && defined(__MACH__)
# define OS_MACOSX
#else
# error "unknown OS - add define here"
#endif


// HAVE_C99: check if compiler advertises support for C99
// (make sure it's #defined before testing value to avoid ICC warning)
#undef HAVE_C99
#ifdef __STDC_VERSION__
# if __STDC_VERSION__ >= 199901L
#  define HAVE_C99
# endif
#endif

#undef HAVE_GETTIMEOFDAY
#undef HAVE_X


