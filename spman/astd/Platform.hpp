#ifndef SPM_PLATFORM_HPP
#define SPM_PLATFORM_HPP

#if defined(__unix__) \
    || defined(linux) \
    || defined(__unix) \
    || defined(__linux__) \
    || defined(__APPLE__) \
    || defined(__MACH__) \
    || defined(__ANDROID__) \
    || defined(__MINGW32__) \
    || defined(__MINGW64__) \
    || defined(__GNUC__) \
    || defined(__CYGWIN__) \
    || defined(__CYGWIN32__)
    #define P_UNIX
#elif defined(WIN32) \
    || defined(_WIN32) \
    || defined(_WIN64) \
    || defined(__NT__) \
    || defined(WINDOWS_PHONE)
    #define P_WINDOWS
#else
    #define P_UP // UP - unknown platform.
#endif

#if defined(__MINGW32__) || defined(__MINGW64__)
    #define P_MINGW
    #define P_WIN_UNIX
#elif defined(__CYGWIN__) || defined(__CYGWIN32__)
    #define P_CYGWIN
    #define P_WIN_UNIX
#elif defined(__ANDROID__) || defined(WINDOWS_PHONE) 
    #define P_PHONE
#elif defined(__APPLE__) || defined(__MACH__) 
    #include <TargetConditionals.h>
    // https://github.com/theos/sdks/blob/master/iPhoneOS14.4.sdk/usr/include/TargetConditionals.h
    // In str 64-78.

    #if defined(TARGET_OS_MAC)      // - Generated code will run under Mac OS X variant

        #if defined(TARGET_OS_OSX) || defined(TARGET_OS_MACCATALYST)
            #define P_NOTEBOOK
        #endif

        // - Generated code for firmware, devices, or simulator
        #ifdef TARGET_OS_IPHONE && !defined(TARGET_OS_MACCATALYST) 
            #define P_PHONE
        #elif defined(TARGET_OS_NANO)
            #define P_PHONE
        #endif

        // - Generated code for firmware, devices, or simulator 
        #if defined(TARGET_OS_EMBEDDED) && !defined(TARGET_OS_MACCATALYST)
            //!Dangerous!
            // I don't know about this:
            #define P_PHONE
            // Or this:
            #define P_SIMULATOR
        #endif
        #ifdef TARGET_OS_SIMULATOR || defined(TARGET_IPHONE_SIMULATOR)
            #define P_SIMULATOR
        #endif

    #endif // defined(TARGET_OS_MAC)
#endif // defined(__APPLE__) || defined(__MACH__)
#endif // SPM_PLATFORM_HPP