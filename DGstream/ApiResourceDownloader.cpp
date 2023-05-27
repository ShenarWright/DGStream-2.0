#include "ApiResourceDownloader.h"

namespace Net
{

	ARD::ARD(std::string url, std::function<void(Json::Value)> func)
	{
		this->url = url;
		callback = func;
	}

	ARD::ARD()
	{
		url = "";
		callback = [](Json::Value) {};
	}

	ARD::~ARD()
	{

	}

	void ARD::work()
	{
		//std::string base, dir;
		//Net::HttpResponce r = Net::Https::sendrequest(url);
		
		//if(url.find("3000") != std::string::npos)
		//{
		//	//base = url.substr(0, url.find(".org") + 4);
		//	base = "http://localhost:3000";//url.substr(0, url.find(".org") + 4);
		//	//dir = url.substr(url.find(".org") + 4);
		//	dir = url.substr(url.find("3000") + 4);
		//	std::cout << base << '\n';
		//	std::cout << dir << '\n';
		//}

		//httplib::Client cli(base);
		auto res = Net::Https::sendrequest(url);
		if (res.error.code !=cpr::ErrorCode::OK)
		{
			std::cout << res.error.message<< '\n';
			return;
		}
		//std::cout << res->body << '\n';
		r.parse(res.text, val);

		Json::Value outval = val;
		//std::cout <<  << '\n';
		//std::cout << val["results"].size() << '\n';
		for (unsigned int i = 0; i < val["results"].size(); i++)
		{
			//Gets the name of the picture that will be saved
			std::string temp = "res/img/" + val["results"][i]["id"].asString() + ".png";
			outval["results"][i]["image"] = temp;

			//gets the url of the image
			std::string Furl = val["results"][i]["image"].asString();
			std::string src;
			src = val["results"][i]["image"].asString();

			//If there is a space in the url it will add a %20
			while (src.find(' ') != std::string::npos)
			{
				size_t pos = src.find(' ');
				src.erase(pos, 1);
				src.insert(pos, "%20");
				std::cout << src << '\n';
				Furl = src;
			}

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
		//for (auto& e : outval["results"])
		//{
			//std::cout << e["image"] << '\n';
		//}

		this->callback(outval);
	}
}
