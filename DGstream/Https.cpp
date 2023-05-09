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
            return false;
            //std::cout << httplib::to_string(res.error()) << '\n';
        }
        return true;
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
