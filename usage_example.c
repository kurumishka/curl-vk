
#include "src/vk_api.h"
#include <stdio.h>


signed main(int argc, char* argv[])
{
    pair params[2] = {
        *(mpair("access_token", "token")),
        *(mpair("v", API_VER))
    };

    byte_str_ptr response = call_method("messages.send", params, 2);
    printf("response = %s\n", response->data);

    return 0;
}

