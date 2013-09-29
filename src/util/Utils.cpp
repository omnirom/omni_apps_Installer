#include "Utils.h"

//--------------------------
QString Utils::getBundlePath()
{
#if defined(WIN32)
    return "./bundles/bundle.win32/";
#elif defined(WIN64)
    return "./bundles/bundle.win64/";
#elif defined(POSIX)
    return "./bundles/bundle.linux/";
#else
#error "Unsupported platform: Cannot determine bundle path"
#endif
}
//--------------------------
