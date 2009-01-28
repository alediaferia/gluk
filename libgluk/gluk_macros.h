#include <kdemacros.h>

#ifndef GLUK_EXPORT
# if defined(MAKE_GLUKCORE_LIB)
/* We are building this library */
#  define GLUK_EXPORT KDE_EXPORT
# else
/* We are using this library */
#  define GLUK_EXPORT KDE_IMPORT
# endif
#endif 
