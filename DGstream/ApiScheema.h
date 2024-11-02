#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <json/json.h>

#include "Sys.h"

class ApiScheema
{
private:
	static Json::Value scheemaData;
public:
	ApiScheema();
	~ApiScheema();

	enum m_server {
		goGocdn = 0,
		streamSb,
		vidstreaming
	};

	static std::string getsearch(std::string data);
	static std::string getinfo(std::string id);
	static std::string getstreaminglinks(std::string episodeId,m_server s = goGocdn);
	static std::string getbase();
	static std::string gettopairing();
	static std::string getrecentepisodes();

	static void loadScheema(std::string path);
};

