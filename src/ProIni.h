#ifndef PROINI_INCLUDED
#  define PROINI_INCLUDED
#  if defined _WIN32 || defined __CYGWIN__
#    ifdef PROINI_EXPORTS
       // Exporting...
#      ifdef __GNUC__
#        define PROINI_API __attribute__ ((dllexport))
#      else
#        define PROINI_API __declspec(dllexport)
#      endif
#    else
#      ifdef __GNUC__
#        define PROINI_API __attribute__ ((dllimport))
#      else
#        define PROINI_API __declspec(dllimport)
#      endif
#    endif
#    define PROINI_NO_EXPORT
#  else
#    if __GNUC__ >= 4
#      define PROINI_API __attribute__ ((visibility ("default")))
#      define PROINI_NO_EXPORT  __attribute__ ((visibility ("hidden")))
#    else
#      define PROINI_API
#      define PROINI_NO_EXPORT
#    endif
#  endif
#endif