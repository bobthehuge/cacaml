#ifndef BTH_LOG_H 
#define BTH_LOG_H

#include <err.h>
#include <stdio.h>


#ifndef NOLOG
#define LOG(...) \
{\
    fprintf(stderr, "[LOG] ");\
    warn(__VA_ARGS__);\
}

#define LOGX(...) \
{\
    fprintf(stderr, "[LOG] ");\
    warnx(__VA_ARGS__);\
}
#endif

#ifndef NOWARN
#define WARN(...) \
{\
    fprintf(\
        stderr,\
        "[WARNING] at '%s':'%s': %d\n-> ",\
        __FILE__, __func__, __LINE__);\
    warn(__VA_ARGS__);\
}

#define WARNX(...) \
{\
    fprintf(\
        stderr,\
        "[WARNING] at '%s':'%s': %d\n-> ",\
        __FILE__, __func__, __LINE__);\
    warnx(__VA_ARGS__);\
}
#endif

#ifndef NOERR
#define ERR(...) \
{\
    fprintf(\
        stderr,\
        "[FATAL] at '%s':'%s': %d\n-> ",\
        __FILE__, __func__, __LINE__\
    );\
    err(__VA_ARGS__);\
}

#define ERRX(...) \
{\
    fprintf(\
        stderr,\
        "[FATAL] at '%s':'%s': %d\n-> ",\
        __FILE__, __func__, __LINE__);\
    errx(__VA_ARGS__);\
}
#endif

#ifndef NOTODO
#define TODOX(...) \
{\
    fprintf(\
        stderr,\
        "[TODO] at '%s':'%s': %d\n-> ",\
        __FILE__, __func__, __LINE__);\
    errx(__VA_ARGS__);\
}

#define TODO() TODOX(1, "todo!")
#endif

#endif /* ! */
