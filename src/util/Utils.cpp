#include "Utils.h"

//--------------------------
QString Utils::getBundlePath()
{
#if defined(WIN32)
    return "./bundles/bundle.win32/";
#elif defined(WIN64)
    return "./bundles/bundle.win64/";
#elif defined(__LP64__)  || defined(_LP64)
    return "./bundles/bundle.linux_x86_64/";
#else
#error "Unsupported platform: Cannot determine bundle path"
#endif
}
//--------------------------
