#pragma once
#include "pch.h"
#include "HttpResponce.h"

namespace Net
{
    class Https
    {
    public:
        static httplib::Result sendrequest(std::string base = "https://example.com",std::string path = "/search");
        static bool hasInternet();
    private:
        //static size_t cb(void* data, size_t size, size_t nmemb, void* userp);
    };
}
