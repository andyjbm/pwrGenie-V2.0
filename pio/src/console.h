#pragma once  // Console/debug defs.
#ifndef CONSOLE_H
   #define CONSOLE_H




   // Turn off console output globally. 
    #ifdef NO_CONSOLE
      #define CONSOLE(...) do {} while (0)
      #define CONSOLELN CONSOLE
      #define CONSOLEF  CONSOLE
    #else

      #ifdef PWRGENIE_CONSOLE_DEST
        #define PWRGENIE_CONSOLE      PWRGENIE_CONSOLE_DEST
      #else
        #define PWRGENIE_CONSOLE      Serial
      #endif

      #define PP_THIRD_ARG(a,b,c,...) c
      #define VA_OPT_SUPPORTED_I(...) PP_THIRD_ARG(__VA_OPT__(,),true,false,)
      #define VA_OPT_SUPPORTED VA_OPT_SUPPORTED_I(?)

      static_assert(VA_OPT_SUPPORTED);

      // Really knatty code for recursing a macro. See here:  
      // https://www.scs.stanford.edu/~dm/blog/va-opt.html
      #define PARENS ()
      #define EXPAND(...) EXPAND4(EXPAND4(EXPAND4(EXPAND4(__VA_ARGS__))))
      #define EXPAND4(...) EXPAND3(EXPAND3(EXPAND3(EXPAND3(__VA_ARGS__))))
      #define EXPAND3(...) EXPAND2(EXPAND2(EXPAND2(EXPAND2(__VA_ARGS__))))
      #define EXPAND2(...) EXPAND1(EXPAND1(EXPAND1(EXPAND1(__VA_ARGS__))))
      #define EXPAND1(...) __VA_ARGS__
    
      #define FOR_EACH(macro, ...) __VA_OPT__(EXPAND(FOR_EACH_HELPER(macro, __VA_ARGS__)))
      #define FOR_EACH_HELPER(macro, a1, ...) macro(a1) __VA_OPT__(PG_SEPARATOR FOR_EACH_AGAIN PARENS (macro, __VA_ARGS__))
      #define FOR_EACH_AGAIN() FOR_EACH_HELPER
      
      //Now my implementation.
      #define PG_SEPARATOR FE_PRINT(" ")
      #define FE_PRINT(x) PWRGENIE_CONSOLE.print(x);

      // These will expand 342? times printing " " (by executing PG_SEPARATOR) between each arg.
      // The expansion can make compiler warning output look messy if you don't use CONSOLE() correctly.

      // Print to 
      #define CONSOLE(...)   FOR_EACH(FE_PRINT, __VA_ARGS__) 
      #define CONSOLELN(...) FOR_EACH(FE_PRINT, __VA_ARGS__) PWRGENIE_CONSOLE.println();
    #endif
     
    // 0: DISABLED: no logging
    // 1: ERROR:    error
    // 2: WARN:     error, warning
    // 3: INFO:     error, warning, info
    // 4: DEBUG:    error, warning, info, debug
    #ifndef PWRGENIE_LOGLEVEL
        #define PWRGENIE_LOGLEVEL     4
    #endif

    #if PWRGENIE_LOGLEVEL>0           //=1,2,3,4
        #define PGLOGERROR(...)       CONSOLE(__VA_ARGS__)
        #define PGLOGERRORLN(...)     CONSOLELN(__VA_ARGS__)
    #else 
        #define PGLOGERROR(...)
        #define PGLOGERRORLN(...)
    #endif

    #if PWRGENIE_LOGLEVEL>1         //=2,3,4
        #define PGLOGWARN(...)      CONSOLE(__VA_ARGS__)
        #define PGLOGWARNLN(...)    CONSOLELN(__VA_ARGS__)
    #else
        #define PGLOGWARN(...)
        #define PGLOGWARNLN(...)
    #endif
    
    #if PWRGENIE_LOGLEVEL>2         //=3,4
        #define PGLOGINFO(...)      CONSOLE(__VA_ARGS__)
        #define PGLOGINFOLN(...)    CONSOLELN(__VA_ARGS__)
    #else
        #define PGLOGINFO(...)
        #define PGLOGINFOLN(...)
    #endif
    
    #if PWRGENIE_LOGLEVEL>3         //=4
        #define PGLOGDEBUG(...)     CONSOLE(__VA_ARGS__)
        #define PGLOGDEBUGLN(...)   CONSOLELN(__VA_ARGS__)
    #else
        #define PGLOGDEBUG(...)
        #define PGLOGDEBUGLN(...)
    #endif
    
#endif