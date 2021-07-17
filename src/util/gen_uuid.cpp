#include "../../spman/util/gen_uuid.hpp"
#include "../../spman/astd/Platform.h"
#ifdef P_WINDOWS
    #include <Objbase.h>
#endif

namespace astd
{

    astd::string createUUID() {
        char uuid[38];
	    unsigned char bytes[16];
    #ifdef P_WINDOWS
        CoCreateGuid((GUID*)bytes);
    #elif defined P_UNIX
        int result;
        FILE* rnd = fopen("/dev/urandom", "rb");
	    result = fread(bytes, 16, 1, rnd);
	    fclose(rnd);
    #endif
        sprintf(uuid, "%02X%02X%02X%02X-%02X%02X-%02X%02X-%02X%02X-%02X%02X%02X%02X%02X%02X",
		    bytes[0], bytes[1], bytes[2], bytes[3],
		    bytes[4], bytes[5],
		    bytes[6], bytes[7],
		    bytes[8], bytes[9],
		    bytes[10], bytes[11], bytes[12], bytes[13], bytes[14], bytes[15]);
        return uuid;
    }
} // namespace astd
