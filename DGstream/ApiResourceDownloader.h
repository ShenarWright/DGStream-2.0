#pragma once
#include "pch.h"
#include "Https.h"

namespace Net
{
	class ARD
	{
	public:
		ARD(std::string url,std::function<void(std::vector<std::string>)>func = [] (std::vector<std::string>) {});
		ARD();
		~ARD();

		void work();
	private:
		Json::Reader r;
		Json::Value val;
		std::string url;
		std::function<void(std::vector<std::string>)>callback;

		std::vector<std::shared_ptr<teemo::Teemo>> downloaders;
	};
}

