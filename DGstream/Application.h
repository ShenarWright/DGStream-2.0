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
	std::thread downloadthread;
	std::vector<std::shared_ptr<sf::Texture>> textures;
	std::mutex mutex;

	void update();
	void render();
	void handleEvents();

	void initWindow();
	void initGui();
	void loadMainMenu(std::vector<std::string> paths);
	void displaySearch(std::vector<std::string> paths);
public:

	Application();
	~Application();
	void run();
};

