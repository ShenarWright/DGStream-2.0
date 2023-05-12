#include "Https.h"

namespace Net
{
    httplib::Result Https::sendrequest(std::string base, std::string path)
    {
        httplib::Client cli(base);
        return cli.Get(path);
    }

    bool Https::hasInternet()
    {
        httplib::Client cli("https://google.com");
        auto res = cli.Get("/search");
        if (res.error() == httplib::Error::Connection)
        {
            std::cout << httplib::to_string(res.error()) << '\n';
            return false;
        }
        return true;
    }

    void Https::sendrequestcb(std::string base, std::string path, std::function<void(Json::Value val)> cb)
    {
        std::cout << base << ',' << path << '\n';
        httplib::Client cli(base);
        auto res = cli.Get(path);
        if (res.error() != httplib::Error::Success)
        {
            std::cout << "Failed\n";
            std::cout << httplib::to_string(res.error());
            return;
        }
        Json::Value jv;
        Json::Reader r;
        r.parse(res->body,jv);
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
