#pragma once
#include "pch.h"

namespace Net
{
    struct HttpResponce {
        char* response;
        size_t size;
        CURLcode code;
    };
}
