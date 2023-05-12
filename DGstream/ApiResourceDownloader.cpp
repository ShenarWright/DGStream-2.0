#include "ApiResourceDownloader.h"

namespace Net
{

	ARD::ARD(std::string url, std::function<void(std::vector<std::string>)> func)
	{
		this->url = url;
		callback = func;
	}

	ARD::ARD()
	{
		url = "";
		callback = [](std::vector<std::string>) {};
	}

	ARD::~ARD()
	{

	}

	void ARD::work()
	{
		std::string base, dir;
		//Net::HttpResponce r = Net::Https::sendrequest(url);
		
		if(url.find(".org") != std::string::npos)
		{
			base = url.substr(0, url.find(".org") + 4);
			dir = url.substr(url.find(".org") + 4);
			std::cout << base << '\n';
			std::cout << dir << '\n';
		}

		//httplib::Client cli(base);
		auto res = Net::Https::sendrequest(base,dir);
		//std::cout << res->body << '\n';
		r.parse(res->body, val);

		std::vector<std::string> paths;
		std::cout << val << '\n';
		//std::cout << val["results"].size() << '\n';
		for (int i = 0; i < val["results"].size(); i++)
		{
			std::string temp = "res/img/" + val["results"][i]["id"].asString() + ".png";
			std::string Furl = val["results"][i]["image"].asString();
			std::string src;
			src = val["results"][i]["image"].asString();
			while (src.find(' ') != std::string::npos)
			{
				int pos = src.find(' ');
				src.erase(pos, 1);
				src.insert(pos, "%20");
				std::cout << src << '\n';
				Furl = src;
			}

			paths.push_back(temp);
			//std::cout << "res/img/" + val["results"][i]["id"].asString() + ".png" << '\n';
			downloaders.push_back(std::make_shared<teemo::Teemo>());
			downloaders.back()->start(Furl, "res/img/" + val["results"][i]["id"].asString() + ".png",
				[&](teemo::Result res)
				{
					if (res == teemo::Result::SUCCESSED)
					{
						//TODO: Make sure that you check to see if the path exists b4 addin
						//to the vector
						std::cout << "success\n";
					}
					else
						std::cout << teemo::GetResultString(res) << '\n';
				}, [&](int64_t total, int64_t downloaded)
				{
				}, [&](int64_t byte_per_secs)
				{
				});
		}
		for (auto& e : downloaders)
		{
			e->futureResult().wait();
		}
		for (auto& e : paths)
		{
			std::cout << e << '\n';
		}

		this->callback(paths);
	}
}
