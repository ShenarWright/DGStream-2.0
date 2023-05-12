#pragma once
#include "pch.h"
#include "Downloader.h"
#include "Https.h"
#include "urls.h"

#define APD ApiResourceDownloader

class Application
{
private:
	sf::RenderWindow window;
	sf::Event ev;
	tgui::Gui gui;
	tgui::Group::Ptr mainMenu;
	tgui::Group::Ptr searchpanel;
	tgui::Group::Ptr animeInfo;
	std::thread downloadthread;
	std::vector<std::shared_ptr<sf::Texture>> textures;
	std::mutex mutex;
	sf::Texture tex;
	Json::Value value;

	void update();
	void render();
	void handleEvents();

	void initWindow();
	void initGui();
	void loadMainMenu(Json::Value val);
	void displaySearch(Json::Value val);
	void displayAnimeInfo(Json::Value val,int count = 0);
public:

	Application();
	~Application();
	void run();
};

