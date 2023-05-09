#include "Application.h"

void Application::update()
{

}

void Application::render()
{
	try
	{
		window.clear();
		mutex.lock();
		gui.draw();
		mutex.unlock();
		window.display();
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << '\n';
	}
}

void Application::handleEvents()
{
	while (window.pollEvent(ev))
	{
		if (ev.type == sf::Event::Closed)
			window.close();
		
		gui.handleEvent(ev);
	}
}

void Application::initWindow()
{
	window.create(sf::VideoMode(sf::VideoMode::getDesktopMode()), "Hello");
	window.setFramerateLimit(30);
	window.setPosition(sf::Vector2i(0, 0));
}

void Application::initGui()
{
	gui.setTarget(window);
}

void Application::loadMainMenu(std::vector<std::string> paths)
{
	mutex.lock();
	gui.removeAllWidgets();
	textures.clear();

	mainMenu = tgui::Group::create();
	gui.add(mainMenu);
	mainMenu->loadWidgetsFromFile("main.txt",false);
	for (auto& e : paths)
	{
		std::cout << e << '\n';
	}

	auto searchbar = mainMenu->get<tgui::EditBox>("SearchBar");
	searchbar->onReturnKeyPress([&](tgui::EditBox::Ptr s)
		{
			Net::Downloader::addard(gogourl + Sys::hayphenate(s->getText().toStdString()), [&](std::vector<std::string> paths) {displaySearch(paths); });
			//displaySearch("https://api.consumet.org/anime/gogoanime/" + s->getText().toStdString());
			s->setFocused(false);
		}, searchbar
	);
	auto settings = mainMenu->get<tgui::Button>("SettingButton");
	settings->onClick([]() {std::cout << "SETTINGS\n"; });
	auto mainpanel = mainMenu->get<tgui::ScrollablePanel>("Mainpanel");
	auto pic = mainMenu->get<tgui::Picture>("Popularpic");
	//pic->getRenderer()->getTexture();
	// 
	//tex.load("res/my hero academia1.png");
	//tgui::Picture::create(tgui::Texture("res/My hero academia0.png"));
	for (int i = 0; i < 1; i++)
	{
		auto row = tgui::ScrollablePanel::create({ "100%",300 });
		row->setPosition(0, i * 152);
		//row->getRenderer()->setBackgroundColor(tgui::Color::Red);
		
		for (int j = 0;j < paths.size(); j++)
		{
			//std::getline(fs, temp);
			textures.push_back(std::make_shared<sf::Texture>());
			textures[j]->loadFromFile(paths[j]);
			auto p = tgui::Picture::create();
			p->getRenderer()->setTexture(*textures[j].get());
			auto l = tgui::Label::create(paths[j].c_str());
			l->setTextSize(25);
			l->setSize(200, l->getSize().y);
			p->setSize(200, 200);
			p->setPosition(j * (p->getSize().x + 30), 10);
			p->onClick([](tgui::ScrollablePanel::Ptr panel)
				{
					panel->removeAllWidgets();
				}
			, row);
			l->setPosition(j * (p->getSize().x + 30), p->getSize().x + 10);
			row->add(p);
			row->add(l);
			//row->setHorizontalScrollbarPolicy()
		}
		//row->setVisible(true);
		mainpanel->add(row);
		
	}
	mutex.unlock();
}

void Application::displaySearch(std::vector<std::string> paths)
{
	mutex.lock();
	searchpanel = tgui::Group::create();
	gui.removeAllWidgets();
	textures.clear();

	gui.add(searchpanel);
	searchpanel->loadWidgetsFromFile("searchpanel.txt");
	auto scrollbar = searchpanel->get<tgui::ScrollablePanel>("panel");
	auto searchbar = searchpanel->get<tgui::EditBox>("searchbar");
	searchbar->onReturnKeyPress([&](tgui::EditBox::Ptr s) {
		Net::Downloader::addard(gogourl + Sys::hayphenate(s->getText().toStdString()), [&](std::vector<std::string> strs) {displaySearch(strs); });
		s->setFocused(false);
		},searchbar);
	//Net::APD apd(url);
	//apd.work();
	int x = 0;
	int y = 0;
	for (int i = 0; i < paths.size(); i++)
	{
		auto p = tgui::Picture::create(paths[i].c_str());
		scrollbar->add(p);
		p->setPosition(x * 320, y * 320);
		p->setSize(300, 300);
		std::cout << "X:" << x << ", Y:" << y << '\n';

		x++;
		if ((((i + 1) % 5) == 0) && i != 0)
		{
			x = 0;
			y++;
		}
	}
	mutex.unlock();
}

Application::Application()
{
	Net::Downloader::Init();
	this->downloadthread = std::thread(Net::Downloader::Run);
	initWindow();
	initGui();

	//Net::APD apd("https://api.consumet.org/anime/gogoanime/top-airing");
	//apd.work();
	if (Net::Https::hasInternet())
		Net::Downloader::addard("https://api.consumet.org/anime/gogoanime/top-airing", [&](std::vector<std::string> paths) {loadMainMenu(paths); });
		//loadMainMenu();
}

Application::~Application()
{
	Net::Downloader::shouldrun = false;
	Net::Downloader::deInit();
	downloadthread.join();
}

void Application::run()
{
	while (window.isOpen())
	{
		handleEvents();
		update();
		render();
	}
}
