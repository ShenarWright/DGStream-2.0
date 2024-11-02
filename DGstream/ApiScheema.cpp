#include "ApiScheema.h"

Json::Value ApiScheema::scheemaData;

ApiScheema::ApiScheema()
{
  
}

ApiScheema::~ApiScheema()
{
}

std::string ApiScheema::getsearch(std::string data)
{
    return scheemaData["base"].asString() + scheemaData["search"].asString() + data;
}

std::string ApiScheema::getinfo(std::string id)
{
    return scheemaData["base"].asString() + scheemaData["info"].asString() + id;
}

std::string ApiScheema::getstreaminglinks(std::string episodeId, m_server s)
{
    std::string serverstr;

    switch(s)
    {
    case goGocdn:
        serverstr = "gogocdn";
        break;
    case vidstreaming:
        serverstr = "vidstreaming";
        break;
    case streamSb:
        serverstr = "streamsb";
        break;
    }

    return scheemaData["base"].asString() + scheemaData["streaminglinks"].asString() + episodeId + "?server=" + serverstr;
}

std::string ApiScheema::getbase()
{
    return scheemaData["base"].asString();
}

std::string ApiScheema::gettopairing()
{
    return scheemaData["base"].asString() + scheemaData["topairing"].asString();
}

std::string ApiScheema::getrecentepisodes()
{
    return scheemaData["base"].asString() + scheemaData["recentepisodes"].asString();
}

void ApiScheema::loadScheema(std::string path)
{
    std::string buffer;
    std::fstream fs;
    fs.open(path);
    if (fs.is_open())
    {
        std::cout << "scheema loaded\n";
        buffer = Sys::readWholeFile(&fs);
        Json::Reader reader;

        reader.parse(buffer,scheemaData);
        fs.close();
    }
    else
    {
        std::cout << "Failed to open:" << path;
        return;
    }
}
