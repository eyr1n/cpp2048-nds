#ifdef __GNUC__
#define _ATTRIBUTE(attrs) __attribute__(attrs)
#else
#define _ATTRIBUTE(attrs)
#endif

int iprintf(const char *__restrict, ...)
    _ATTRIBUTE((__format__(__printf__, 1, 2)));
