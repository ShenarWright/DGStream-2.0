#pragma once
#include "pch.h"
#include "ApiResourceDownloader.h"

namespace Net
{
	//download queue struct
	struct downloadQueue
	{
		std::string url;
		std::string targetpath;
		std::function<void(teemo::Result res)>					 result_callback;
		std::function<void(int64_t total, int64_t downloaded)>	 progress_callback;
		std::function<void(int64_t byte_per_secs)>				 speed_callback;
	};

	//Function definition to create a download queue
	downloadQueue createQueue(
		std::string url,
		std::string targetpath,
		std::function<void(teemo::Result res)>					 result_callback,
		std::function<void(int64_t total, int64_t downloaded)>	 progress_callback,
		std::function<void(int64_t byte_per_secs)>				 speed_callback
	);

	//downloader class 
	class Downloader
	{
	public:
		static bool Init();
		static bool addToQueue(downloadQueue queue);
		static bool deInit();
		static void Run();
		static bool shouldrun;
		static bool addard(std::string url, std::function<void(Json::Value)>func = [](Json::Value) {});
	private:
		static bool download();
		static bool downloadres();
		static std::mutex m_mutex;
		static std::vector<downloadQueue>m_queue;
		//static teemo::Teemo teemo;
		static std::vector<std::shared_ptr<teemo::Teemo>> downloaders;
		//std::vector<std::shared_ptr<teemo::Teemo>> d;
		static ARD ard;
		static ARD tempard;

		static bool shouldrunArd;
	};
}


