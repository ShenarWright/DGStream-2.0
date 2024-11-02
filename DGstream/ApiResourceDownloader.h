#pragma once
#include "pch.h"
#include "Https.h"

namespace Net
{
	class ARD
	{
	public:
		ARD(std::string url,std::function<void(Json::Value)>func = [] (Json::Value) {});
		ARD();
		~ARD();

		void work();
	private:
		Json::Reader r;
		Json::Value val;
		std::string url;
		std::function<void(Json::Value)>callback;

		std::vector<std::shared_ptr<zoe::Zoe>> downloaders;
	};
}

