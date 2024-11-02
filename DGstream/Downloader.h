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
		std::function<void(zoe::Result res)>					 result_callback;
		std::function<void(int64_t total, int64_t downloaded)>	 progress_callback;
		std::function<void(int64_t byte_per_secs)>				 speed_callback;
	};

	struct requestCb
	{
		requestCb(std::string url, std::function<void(Json::Value)>func)
		{
			this->url = url;
			this->func = func;
		}

		requestCb()
		{

		}
		std::string url;
		std::function<void(Json::Value)> func;
	};

	requestCb createrequests(std::string url,std::function<void(Json::Value)> func);

	//Function definition to create a download queue
	downloadQueue createQueue(
		std::string url,
		std::string targetpath,
		std::function<void(zoe::Result res)>					 result_callback,
		std::function<void(int64_t total, int64_t downloaded)>	 progress_callback,
		std::function<void(int64_t byte_per_secs)>				 speed_callback
	);

	struct events
	{
		Json::Value input;
		std::function<void(Json::Value)> callback;
	};

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
		bool addRequest(std::string url, std::function<void(Json::Value)>func = [](Json::Value) {}, bool startrequest = true);
		bool addRequests(std::vector <requestCb>requests,bool startrequest = true);
		bool pollEvents();

	private:
		bool download();
		bool downloadres();
		bool handlerequest();
		std::mutex m_mutex;
		std::vector<downloadQueue>m_queue;
		std::vector<requestCb> requests;
		std::vector<std::shared_ptr<zoe::Zoe>> downloaders;
		std::vector<events> ev;
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


