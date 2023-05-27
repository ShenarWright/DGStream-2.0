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

	struct requestcb
	{
		std::string url;
		std::function<void(Json::Value v)> func;
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
		bool Init();
		bool addToQueue(downloadQueue queue);
		bool deInit();
		void Run();
		bool shouldrun;
		bool addard(std::string url, std::function<void(Json::Value)>func = [](Json::Value) {});
		bool addRequest(std::string url, std::function<void(Json::Value)>func = [](Json::Value) {});
	private:
		bool download();
		bool downloadres();
		bool handlerequest();
		std::mutex m_mutex;
		std::vector<downloadQueue>m_queue;
		std::vector<requestcb> requests;
		std::vector<std::shared_ptr<teemo::Teemo>> downloaders;
		ARD ard;
		ARD tempard;
		bool shouldrunArd;
		bool shouldrunReq;

	public:
		Downloader();
		Downloader(Downloader& next);
		~Downloader();
	};
}


