#pragma once
#include "pch.h"
#include "Downloader.h"
#include "Https.h"
#include "urls.h"
#include "Player.h"
#include "ApiScheema.h"

#define APD ApiResourceDownloader


class Application
{
private:
	sf::RenderWindow window;
	sf::Event ev;
	sf::Texture tex;

	tgui::Gui gui;
	tgui::Group::Ptr mainMenu;
	tgui::Group::Ptr searchpanel;
	tgui::Group::Ptr animeInfo;

	sf::Clock pausecooldown;
	sf::Clock skipcooldown;

	std::thread downloadthread;
	//std::vector<std::shared_ptr<sf::Texture>> textures;
	std::map<std::string, Json::Value> jsons;
	std::mutex mutex;
	std::shared_ptr<Player> player;

	int currentepisode;
	//Json::Value value;

	Net::Downloader downloader;

	bool playvideo;

	void update();
	void render();
	void handleEvents();
	void playVideo(int episode = 0);

	void initWindow();
	void initGui();
	void loadMainMenu();
	void displaySearch();
	void displayAnimeInfo(int count = 0);
	void saveAnimeInfo(Json::Value data);

public:

	Application();
	~Application();
	void run();

	void runDownloader();
};

