#include "Https.h"

namespace Net
{
    cpr::Response Https::sendrequest(std::string url)
    {
        return cpr::Get(cpr::Url(url));
    }

    bool Https::hasInternet()
    {
        cpr::Response r = cpr::Get(cpr::Url("https://google.com"),cpr::Timeout(1000));

        if (r.error.code == cpr::ErrorCode::CONNECTION_FAILURE)
        {
            std::cout << r.error.message << '\n';
            return false;
        }
        return true;
    }

    void Https::sendrequestcb(std::string url , std::function<void(Json::Value val)> cb)
    {
        
        std::cout << url << '\n';
        auto res = cpr::Get(cpr::Url(url));
        if (res.error.code != cpr::ErrorCode::OK)
        {
            std::cout << "Failed\n";
            std::cout << res.error.message << '\n';
            return;
        }
        Json::Value jv;
        Json::Reader r;
        r.parse(res.text,jv);
        std::cout << jv << '\n';
        cb(jv);
    }

    /*size_t Https::cb(void* data, size_t size, size_t nmemb, void* userp)
    {
        size_t realsize = size * nmemb;
        struct HttpResponce* mem = (struct HttpResponce*)userp;

        char* ptr = (char*)realloc(mem->response, mem->size + realsize + 1);
        if (ptr == NULL)
            return 0;  /* out of memory! 

        mem->response = ptr;
        memcpy(&(mem->response[mem->size]), data, realsize);
        mem->size += realsize;
        mem->response[mem->size] = 0;

        return realsize;
    }*/
}
