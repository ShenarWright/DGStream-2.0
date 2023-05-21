#include "Application.h"

void Application::update()
{

}

void Application::render()
{
	try
	{
		window.clear();

		if (playvideo)
			player->render();


		window.resetGLStates();

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
	if (playvideo)
		player->handleEvents();
}

void Application::playVideo(std::string url)
{
	gui.removeAllWidgets();
	player = std::make_shared<Player>(url);
	player->setSize(1920, 1080);
	player->addWidgetstoGui(gui);
	playvideo = true;
	player->setTitle(jsons["animeInfo"]["title"].asString());
	player->onBackButtonPressed([&]()
		{
			player = nullptr;
			playvideo = false;

			if (jsons.find("animeInfo") != jsons.end())
			{
				displayAnimeInfo();
			}
			else
				loadMainMenu();
		}
	);
	
}

void Application::initWindow()
{
	window.create(sf::VideoMode(sf::VideoMode::getDesktopMode()), "DGStream");
	window.setFramerateLimit(30);
	window.setPosition(sf::Vector2i(0, 0));
}

void Application::initGui()
{
	gui.setTarget(window);
}

void Application::loadMainMenu()
{
	mutex.lock();
	gui.removeAllWidgets();
	textures.clear();
	//temporary
	auto val = jsons["top-airing"];

	mainMenu = tgui::Group::create();
	gui.add(mainMenu);
	mainMenu->loadWidgetsFromFile("main.txt",false);
	mainMenu->showWithEffect(tgui::ShowAnimationType::SlideFromLeft, 600);
	for (auto& e : val["results"])
	{
		std::cout << e["image"] << '\n';
	}

	auto searchbar = mainMenu->get<tgui::EditBox>("SearchBar");
	searchbar->onReturnKeyPress([&](tgui::EditBox::Ptr s)
		{
			Net::Downloader::addard(gogourl  "/" + Sys::hayphenate(s->getText().toStdString()), [&](Json::Value val) 
				{
					if (jsons.find("search") != jsons.end())
					{
						jsons["search"] = val;
					}
					else
						jsons.insert({ "search",val });

					displaySearch(); 
				});
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
		auto row = tgui::ScrollablePanel::create({ "100%",350 });
		row->setPosition(0, i * 152);
		//row->getRenderer()->setBackgroundColor(tgui::Color::Red);
		
		for (unsigned int j = 0;j < val["results"].size(); j++)
		{
			//std::getline(fs, temp);
			textures.push_back(std::make_shared<sf::Texture>());
			textures[j]->loadFromFile(val["results"][j]["image"].asCString());
			auto p = tgui::Picture::create();
			p->getRenderer()->setTexture(*textures[j].get());
			auto l = tgui::Label::create(val["results"][j]["title"].asCString());
			l->setTextSize(20);
			l->setSize(200, l->getSize().y * 2);
			p->setSize(200, 200);
			p->setPosition(j * (p->getSize().x + 30) + 10, 10);
			p->onClick([&](int count)
				{
					Net::Https::sendrequestcb(consumet, gogo info + jsons["top-airing"]["results"][count]["id"].asString(), [&](Json::Value v)
						{
							if (jsons.find("animeInfo") != jsons.end())
							{
								jsons["animeInfo"] = v;
							}
							else
								jsons.insert({ "animeInfo",v });

							displayAnimeInfo(count);
						}
							);
				},j
			);
			l->setPosition(j * (p->getSize().x + 30) + 10, p->getSize().x + 10);
			row->add(p);
			row->add(l);
			//row->setHorizontalScrollbarPolicy()
		}
		//row->setVisible(true);
		mainpanel->add(row);
		
	}
	mutex.unlock();
}

void Application::displaySearch()
{
	mutex.lock();
	searchpanel = tgui::Group::create();
	gui.removeAllWidgets();
	textures.clear();
	std::cout << jsons["search"] << '\n';
	
	gui.add(searchpanel);
	//searchpanel->showWithEffect(tgui::ShowAnimationType::SlideFromTop,1000);
	Json::Value val = jsons["search"];

	searchpanel->loadWidgetsFromFile("searchpanel.txt");
	auto scrollbar = searchpanel->get<tgui::ScrollablePanel>("panel");
	auto searchbar = searchpanel->get<tgui::EditBox>("searchbar");
	auto back = searchpanel->get<tgui::Button>("back");
	back->onClick([&]()
		{
			loadMainMenu();
		}
	);
	searchbar->onReturnKeyPress([&](tgui::EditBox::Ptr s) {
		Net::Downloader::addard(gogourl "/" + Sys::hayphenate(s->getText().toStdString()), [&](Json::Value val) {
			if (jsons.find("search") != jsons.end())
			{
				jsons["search"] = val;
			}
			else
				jsons.insert({ "search",val });
			displaySearch(); 
		});
		s->setFocused(false);
		},searchbar);
	//Net::APD apd(url);
	//apd.work();
	int x = 0;
	int y = 0;
	for (unsigned int i = 0; i < val["results"].size(); i++)
	{
		//std::cout << "iteration:" << i << " Path:" << val["results"][i]["image"];
		textures.push_back(std::make_shared<sf::Texture>());
		textures.back()->loadFromFile(val["results"][i]["image"].asCString());
		auto p = tgui::Picture::create(*textures.back().get());
		auto label = tgui::Label::create(val["results"][i]["title"].asCString());
		scrollbar->add(p);
		scrollbar->add(label);
		p->setPosition(x * 320, y * 340);
		p->setSize(300, 300);
		
		label->setSize(300, label->getSize().y);
		label->setPosition(p->getPosition().x, p->getPosition().y + p->getSize().y);
		std::cout << "pic X:" << p->getPosition().x << ", pic Y:" << p->getPosition().y << '\n';
		std::cout << "label X:" << label->getPosition().x << ", lavel Y:" << label->getPosition().y << '\n';
		
		p->onClick([&](int a)
			{
				std::cout << consumet gogo info + jsons["search"]["results"][a]["id"].asString();
				Net::Https::sendrequestcb(consumet, gogo info + jsons["search"]["results"][a]["id"].asString(), [&](Json::Value v)
					{
						std::cout << v << '\n';
						if (jsons.find("animeInfo") != jsons.end())
						{
							jsons["animeInfo"] = v;
						}
						else
							jsons.insert({ "animeInfo",v });
						displayAnimeInfo(a);
					}
				);
		},i);

		x++;
		if ((((i + 1) % 5) == 0) && i != 0)
		{
			x = 0;
			y++;
		}
	}
	mutex.unlock();
}

void Application::displayAnimeInfo(int count)
{
	mutex.lock();
	gui.removeAllWidgets();
	Json::Value val = jsons["animeInfo"];
	animeInfo = tgui::Group::create();
	animeInfo->loadWidgetsFromFile("animeinfo.txt");
	//mainMenu->setVisible(false);
	gui.add(animeInfo);
	auto pic = animeInfo->get<tgui::Picture>("Animepic");
	auto label = animeInfo->get<tgui::Label>("Description");
	auto name = animeInfo->get<tgui::Label>("Name");
	auto back = animeInfo->get<tgui::Button>("back");
	auto play = animeInfo->get<tgui::Button>("play");
	auto btn1 = animeInfo->get<tgui::Button>("download");

	play->onClick([&](Json::Value v)
		{
			Net::Https::sendrequestcb(consumet, gogo watch + v["episodes"][0]["id"].asString() + server gogocdn,
			[&](Json::Value newvalue)
				{
					std::cout << newvalue << '\n';

					playVideo(newvalue["sources"][3]["url"].asString());
				}
			);
		},val
	);

	back->onClick([&]()
		{
			if (jsons.find("search") != jsons.end())
			{
				
				displaySearch();
			}
			else
				loadMainMenu();

			jsons.erase("animeInfo");
		});

	btn1->onClick([&](Json::Value v)
		{
			Net::Https::sendrequestcb(consumet, gogo servers + v["episodes"][0]["id"].asString(),
			[&](Json::Value newvalue)
				{
					std::cout << newvalue << '\n';

					for (unsigned int i = 0; i < newvalue.size(); i++)
					{
						if (newvalue[i]["name"].asString() == "Mp4Upload")
						{
							std::string url = newvalue[i]["url"].asString();
							std::string base = url.substr(0,url.find(".com") + 4);
							std::string path = url.substr(url.find(".com") + 4);
							auto res = Net::Https::sendrequest(base,path);

							std::cout << "base:" << base << '\n';
							std::cout << "path:" << path << '\n';

							std::cout << res->body << '\n';
						}

					}
				});
		},val
	);
	label->setText(val["description"].asString());
	name->setText(val["title"].asString());
	pic->getRenderer()->setTexture(*textures[count].get());
	mutex.unlock();
}

Application::Application()
{
	Net::Downloader::Init();
	this->downloadthread = std::thread(Net::Downloader::Run);
	initWindow();
	initGui();
	playvideo = false;

	//Net::APD apd("https://api.consumet.org/anime/gogoanime/top-airing");
	//apd.work();
	if (Net::Https::hasInternet())
		Net::Downloader::addard(gogourl topairing, [&](Json::Value val) 
		{
			std::cout << val << '\n';
			jsons.insert({ "top-airing",val });
			loadMainMenu(); 
		});
		//loadMainMenu();
	
	//tex.loadFromFile("res/logo.png");
	
}

Application::~Application()
{
	Net::Downloader::shouldrun = false;
	downloadthread.join();
	Net::Downloader::deInit();
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
