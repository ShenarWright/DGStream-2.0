#pragma once
#include "pch.h"
#include "HttpResponce.h"

namespace Net
{
    class Https
    {
    public:
        static cpr::Response sendrequest(std::string url = "https://example.com");
        static bool hasInternet();
        static void sendrequestcb(std::string url = "https://example.com", std::function<void(Json::Value val)>cb = [&] (Json::Value) {});
        static Json::Value sendrequestjson(std::string url = "https://example.com");
    private:
        //static size_t cb(void* data, size_t size, size_t nmemb, void* userp);
    };
}
