#include "Downloader.h"

namespace Net
{
	//std::vector<downloadQueue> Downloader::m_queue;
	//bool Downloader::shouldrun;
	//std::mutex Downloader::m_mutex;
	////teemo::Teemo Downloader::teemo;
	//std::vector <std::shared_ptr<teemo::Teemo>> Downloader::downloaders;
	//ARD Downloader::ard;
	//ARD Downloader::tempard;
	//bool Downloader::shouldrunArd;
	//bool Downloader::shouldrunReq;
	//std::vector<requestcb> Downloader::requests;

	bool Downloader::Init()
	{
		teemo::Teemo::GlobalInit();
		//teemo.setThreadNum(10);
		shouldrun = true;
		shouldrunArd = false;
		shouldrunReq = false;
		std::cout << "Downloader succfully Initialized\n";
		return true;
	}

	bool Downloader::addToQueue(downloadQueue queue)
	{
		m_mutex.lock();
		m_queue.push_back(queue);
		m_mutex.unlock();

		return true;
	}


	bool Downloader::deInit()
	{
		teemo::Teemo::GlobalUnInit();

		downloaders.clear();
		requests.clear();

		//succLog("Downloader Successfully DeInitialized");
		return true;
	}
	void Downloader::Run()
	{
		while (shouldrun)
		{
			//std::cout << "HERE\n";
			downloadres();
			download();
			handlerequest();
		}
	}
	bool Downloader::addard(std::string url, std::function<void(Json::Value)> func)
	{
		m_mutex.lock();
		tempard = ARD(url, func);
		m_mutex.unlock();
		shouldrunArd = true;
		return true;
	}
	bool Downloader::addRequest(std::string url, std::function<void(Json::Value)> func)
	{
		std::cout << "ADDED\n\n\n\n\n\n";
		m_mutex.lock();
		requests.push_back(createrequests(url,func));
		m_mutex.unlock();
		shouldrunReq = true;

		return true;
	}
	bool Downloader::addRequests(std::vector<requestCb> requests)
	{
		m_mutex.lock();
		for (auto& e : requests)
		{
			this->requests.push_back(e);
		}
		m_mutex.unlock();
		shouldrunReq = true;
		return false;
	}
	bool Downloader::download()
	{
		std::vector<downloadQueue>tempqueue;

		m_mutex.lock();
		tempqueue = m_queue;
		//tempqueue.emplace_back(m_queue);
		m_queue.clear();
		m_mutex.unlock();
		//std::cout << "HERE";
		for (auto& e : tempqueue)
		{

			//std::shared_future<teemo::Result> async_task = teemo.start(e.url, e.targetpath, e.result_callback, e.progress_callback, e.speed_callback);
			//async_task.wait();

			downloaders.push_back(std::make_shared<teemo::Teemo>());
			auto r = downloaders.back()->start(e.url, e.targetpath, e.result_callback, e.progress_callback, e.speed_callback);
			size_t count = downloaders.size();
			
			for (auto& e : downloaders)
			{
				Sleep(5000);
			}

			//System::succLog("Successfully downloaded file");
			std::cout << "Successfully downloaded file\n";
		}

		//Sleep(5000);

		//for (auto& e : downloaders)
		//{
			
		//}
		downloaders.clear();

		//downloaders.clear();

		tempqueue.clear();
		return true;
	}

	bool Downloader::downloadres()
	{
		if (shouldrunArd)
		{
			m_mutex.lock();
			ard = tempard;
			m_mutex.unlock();
			ard.work();
			shouldrunArd = false;
		}
		return true;
	}

	bool Downloader::handlerequest()
	{
		if (shouldrunReq)
		{
			if (requests.size() <= 0)
			{
				shouldrunReq = false;

			}
			else
			{
				m_mutex.lock();
				Net::Https::sendrequestcb(requests.front().url, requests.front().func);
				requests.erase(requests.begin());
				m_mutex.unlock();
			}
		}
		return true;
	}

	requestCb createrequests(std::string url, std::function<void(Json::Value)> func)
	{
		requestCb cb;
		cb.url = url;
		cb.func = func;
		return cb;
	}

	downloadQueue createQueue(std::string url, std::string targetpath, std::function<void(teemo::Result res)> result_callback, std::function<void(int64_t total, int64_t downloaded)> progress_callback, std::function<void(int64_t byte_per_secs)> speed_callback)
	{
		downloadQueue q;
		q.url = url;
		q.targetpath = targetpath;
		q.result_callback = result_callback;
		q.progress_callback = progress_callback;
		q.speed_callback = speed_callback;
		return q;
	}


	Downloader::Downloader()
	{
		//std::vector<downloadQueue> Downloader::m_queue;
		shouldrun = true;
		//std::mutex Downloader::m_mutex;
		////teemo::Teemo Downloader::teemo;
		//std::vector <std::shared_ptr<teemo::Teemo>> Downloader::downloaders;
		//ARD Downloader::ard;
		//ARD Downloader::tempard;
		//bool Downloader::shouldrunArd;
		//bool Downloader::shouldrunReq;
		//std::vector<requestcb> Downloader::requests;
	}


	Downloader::~Downloader()
	{

	}
}

