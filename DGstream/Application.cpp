#include "Application.h"

void Application::update()
{
	if (window.hasFocus())
	{
		if (playvideo)
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && pausecooldown.getElapsedTime().asSeconds() > .15f)
			{
				player->ispaused() ? player->play() : player->pause();
				player->showHud(true);
				pausecooldown.restart();
			}
			
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && skipcooldown.getElapsedTime().asSeconds() > .1f)
			{
				player->seek(5);
				skipcooldown.restart();
				player->showHud(true);
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && skipcooldown.getElapsedTime().asSeconds() > .1f)
			{
				player->seek(-5);
				skipcooldown.restart();
				player->showHud(true);
			}

		}
	}
	else
	{
		player->showHud(player->isHudshown());
	}
}

void Application::render()
{
	try
	{
		window.clear();

		if (playvideo)
		{
			player->render();
			window.resetGLStates();
		}

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
		{
			player->stop();
			playvideo = false;
			gui.removeAllWidgets();
			window.close();
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		{
			player->stop();
			playvideo = false;
			gui.removeAllWidgets();
			window.close();
		}
		
		if (ev.type == sf::Event::Resized)
		{
			initGui();
			player->updateSize(window.getSize().x,window.getSize().y);
		}


		gui.handleEvent(ev);
	}

	while (downloader.pollEvents())
	{
		std::cout << "Called\n";
	}

	if (playvideo)
		player->handleEvents();
}

void Application::playVideo(int episode)
{
	mutex.lock();
	gui.removeAllWidgets();
	auto val = jsons["stream-links"];
	for (int i = 0; i < val["sources"].size(); i++)
	{
		if (val["sources"][i]["quality"].asString() == "default")
		{
			player->load(val["sources"][i]["url"].asString());
		}
	}
	player->addWidgetstoGui(gui);
	playvideo = true;
	player->setTitle(jsons["animeInfo"]["title"].asString());

	player->onNextButtonPressed([=]()
		{
			downloader.addRequest(ApiScheema::getstreaminglinks(jsons["animeInfo"]["episodes"][currentepisode]["id"].asString()), [=](Json::Value newvalue)
				{
					if (player->getPos() / player->getDuration() > .8)
					{
						std::stringstream ss;
						ss << "ep. " << currentepisode;

						jsons["animeInfo"]["episodes"][currentepisode - 1]["watched"] = true;
						auto scroll = animeInfo->get<tgui::ScrollablePanel>("EPISODES ScrollablePanel");
						auto btn = scroll->get<tgui::Button>(ss.str());

						btn->getRenderer()->setTexture("res/animeInfo/EPISODE DOWNLOAD BUTTON.png");
						saveAnimeInfo(jsons["animeInfo"]);
					}

					jsons["stream-links"] = newvalue;
					currentepisode++;
					playVideo();
				}
			);
		}
	);

	player->onPreviousButtonPressed([&]()
		{

		}
	);

	player->onBackButtonPressed([&]()
		{
			player->stop();
			//player = nullptr;
			playvideo = false;
			gui.removeAllWidgets();

			if (jsons.find("animeInfo") != jsons.end())
			{
				//displayAnimeInfo();
				if (player->getPos() / player->getDuration() > .8)
				{
					std::stringstream ss;
					ss << "ep. " << currentepisode;

					jsons["animeInfo"]["episodes"][currentepisode - 1]["watched"] = true;
					auto scroll = animeInfo->get<tgui::ScrollablePanel>("EPISODES ScrollablePanel");
					auto btn = scroll->get<tgui::Button>(ss.str());
					
					btn->getRenderer()->setTexture("res/animeInfo/EPISODE DOWNLOAD BUTTON.png");
					saveAnimeInfo(jsons["animeInfo"]);
				}

				std::cout << "current episode Back button:" << currentepisode << '\n';
				gui.add(animeInfo);
			}
			else
			{
				//loadMainMenu();
				gui.add(mainMenu);
			}

			
		}
	);
	mutex.unlock();
}

void Application::initWindow()
{
	std::fstream fs;
	fs.open("config/window.ini");
	std::string buffer;
	std::string temp;

	if (fs.is_open())
	{
		std::cout << "Opened\n";
	}

	while (std::getline(fs, temp))
	{
		buffer += temp;
	}
	fs.close();

	Json::Value v;
	Json::Reader r;
	r.parse(buffer, v);
	std::cout << v << '\n';
	if (v["fullscreen"].asBool())
	{
		window.create(sf::VideoMode::getDesktopMode(), "DGStream", sf::Style::Fullscreen);
	}
	else
	{
		if (v["windowsize"][0].asInt() > sf::VideoMode::getDesktopMode().width)
		{
			window.create(sf::VideoMode::getDesktopMode(),"DGStream");
		}
		else
			window.create(sf::VideoMode(v["windowsize"][0].asInt(), v["windowsize"][1].asInt()), "DGStream", sf::Style::Default);
	}

	if (v["Vsync"].asBool())
	{
		window.setVerticalSyncEnabled(v["Vsync"].asBool());
	}
	else
	{
		window.setFramerateLimit(v["Fps"].asInt());
	}
	window.setVerticalSyncEnabled(true);
	window.setPosition(sf::Vector2i(0, 0));
}

void Application::initGui()
{
	gui.setTarget(window);
	gui.setRelativeView({ 0,0,1920.f / (float)window.getSize().x,1080.f / (float)window.getSize().y});
}

void Application::loadMainMenu()
{
	mutex.lock();
	gui.removeAllWidgets();
	//temporary
	auto val = jsons["top-airing"];

	mainMenu = tgui::Group::create();
	gui.add(mainMenu);
	try
	{
		mainMenu->loadWidgetsFromFile("mainscreen.txt");
	}
	catch (tgui::Exception e)
	{
		std::cout << e.what() << '\n';
	}

	auto searchbar = mainMenu->get<tgui::EditBox>("SEARCH BAR");
	searchbar->onReturnKeyPress([&](tgui::EditBox::Ptr s)
		{
			downloader.addard(ApiScheema::getsearch(Sys::hayphenate(s->getText().toStdString())), [&](Json::Value val)
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

	auto scrollbar = mainMenu->get<tgui::ScrollablePanel>("TRENDING ScrollablePanel");
	for (unsigned int i = 0; i < val["results"].size(); i++)
	{

		//textures[i]->loadFromFile(val["results"][i]["image"].asCString());

		auto pic = tgui::Picture::create();
		pic->setSize(204, 296);
		//auto tex = tgui::Texture::Texture();
		//tex.loadFromPixelData(textures[i].get()->getSize(), textures[i].get()->copyToImage().getPixelsPtr());
		pic->getRenderer()->setTexture(val["results"][i]["image"].asCString());
		pic->setPosition(100 + i * 300, 40);
		pic->onClick([&](int count)
			{
				downloader.addRequest(ApiScheema::getinfo(jsons["top-airing"]["results"][count]["id"].asString()),
				[&](Json::Value v)
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
			}, i
		);

		auto label = tgui::Label::create(val["results"][i]["title"].asCString());
		label->setPosition(pic->getPosition().x, 350);
		label->setTextSize(24);
		label->setScrollbarPolicy(tgui::Scrollbar::Policy::Never);
		label->setSize(204, label->getSize().y);
		label->getRenderer()->setTextColor(tgui::Color::White);
		scrollbar->add(pic);
		scrollbar->add(label);
		//scrollbar->getRenderer()->setTextureBackground("C:\\Users\\dante\\OneDrive\\Pictures\\Untitled.png");
	}
	mutex.unlock();
	//mainMenu->showWithEffect(tgui::ShowAnimationType::SlideFromLeft, 600);
}

void Application::displaySearch()
{
	searchpanel = tgui::Group::create();
	try
	{
		searchpanel->loadWidgetsFromFile("searchpanel.txt");
	}
	catch (tgui::Exception e)
	{
		std::cout << e.what() << '\n';
	}

	//std::cout << jsons["search"] << '\n';
	

	
	//searchpanel->showWithEffect(tgui::ShowAnimationType::SlideFromTop,1000);
	Json::Value val = jsons["search"];

	auto searchbar = searchpanel->get<tgui::EditBox>("SEARCH BAR");
	searchbar->onReturnKeyPress([&](tgui::EditBox::Ptr s) {

		downloader.addard(ApiScheema::getsearch(Sys::hayphenate(s->getText().toStdString())), [&](Json::Value val) {

			if (jsons.find("search") != jsons.end())
			{
				jsons["search"] = val;
			}
			else
				jsons.insert({ "search",val });
			displaySearch();
			});
		s->setFocused(false);
	}, searchbar);

	auto scrollbar = searchpanel->get<tgui::ScrollablePanel>("BROWSE PANEL");
	scrollbar->getRenderer()->setScrollbarWidth(0.1f);
	int x = 0, y = 0;
	for (unsigned int i = 0; i < val["results"].size(); i++)
	{
		//std::cout << "iteration:" << i << " Path:" << val["results"][i]["image"];
		//textures.push_back(std::make_shared<sf::Texture>());
		//textures.back()->loadFromFile(val["results"][i]["image"].asCString());
		//auto tex = tgui::Texture();
		//tex.loadFromPixelData(textures.back().get()->getSize(), textures.back().get()->copyToImage().getPixelsPtr());
		auto p = tgui::Picture::create(val["results"][i]["image"].asCString());
		auto label = tgui::Label::create(val["results"][i]["title"].asCString());
		scrollbar->add(p);
		scrollbar->add(label);
		p->setPosition(64 + (x * 350), 80 + (y * 620));
		p->setSize(290, 414);

		label->setSize(281, 189);
		label->setPosition(p->getPosition().x, p->getPosition().y + p->getSize().y);
		label->setTextSize(36);
		label->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Center);
		label->getRenderer()->setTextColor(tgui::Color::White);
		label->getRenderer()->setScrollbarWidth(0.1f);

		std::cout << "pic X:" << p->getPosition().x << ", pic Y:" << p->getPosition().y << '\n';
		std::cout << "label X:" << label->getPosition().x << ", lavel Y:" << label->getPosition().y << '\n';

		p->onClick([&](int a)
			{
				std::cout << ApiScheema::getinfo(jsons["search"]["results"][a]["id"].asString());
				downloader.addRequest(ApiScheema::getinfo(jsons["search"]["results"][a]["id"].asString()),
					[&](Json::Value v)
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
		}, i);

		x++;
		if ((((i + 1) % 5) == 0) && i != 0)
		{
			x = 0;
			y++;
		}
	}
	auto back = searchpanel->get<tgui::Button>("BACK BUTTON");
	back->onClick([&]()
		{
			jsons.erase("search");
			gui.removeAllWidgets();
			gui.add(mainMenu);
		}
	);

	mutex.lock();
	gui.removeAllWidgets();
	gui.add(searchpanel);
	mutex.unlock();
}

void Application::displayAnimeInfo(int count)
{
	gui.removeAllWidgets();
	animeInfo = tgui::Group::create();
	
	Json::Value data;
	Json::Value savedval;
	Json::Reader reader;
	std::fstream fs;
	std::string buffer;
	//std::string temp;
	auto val = jsons["animeInfo"];

	if (tgui::Filesystem::fileExists("data/animeInfo/" + val["id"].asString() + ".json"))
	{
		std::cout << "File Exists:" << "data/animeInfo/" + val["id"].asString() + ".json" << "\n";
		fs.open("data/animeInfo/" + val["id"].asString() + ".json");
		if (fs.is_open())
		{
			std::string buffer;
			buffer = Sys::readWholeFile(&fs);

			Json::Reader reader;
			Json::Value fileData;

			reader.parse(buffer,fileData);

			//IF new information is found
			if (fileData["episodes"].size() != val["episodes"].size())
			{
				//Iterates through the new data and adds it to the file
				for (int i = fileData["episodes"].size() - 1; i < val["episodes"].size() - 1; i++)
				{
					fileData["episodes"][i] = val["episodes"][i];
					fileData["episodes"][i]["watched"] = false;
					fileData["episodes"][i]["downloaded"] = false;
				}
				saveAnimeInfo(fileData);
			}
			else
			{
				val = fileData;
			}
			fs.close();
		}
		else
		{
			std::cout << "Failed to open file\n";
		}
	}
	else
	{
		Json::Value fileData;
		fileData = val;

		for (int i = 0; i < fileData["episodes"].size() - 1; i++)
		{
			fileData["episodes"][i]["watched"] = false;
			fileData["episodes"][i]["downloaded"] = false;
		}
		saveAnimeInfo(fileData);
		val = fileData;
	}
	/*
	fs.open("data/animeInfo.json");
	if (fs.is_open())
	{
		buffer = Sys::readWholeFile(&fs);
		fs.close();

		reader.parse(buffer,data);

		for (int i = 0; i < data["NumAnime"].asInt(); i++)
		{
			if (data["Sources"][i]["id"].asString() == val["id"].asString())
			{
				std::cout << "CALLED AND I IS:" << i << '\n';
				fs.open("data/animeInfo/" + data["Sources"][i]["id"].asString() + ".json");
				if (fs.is_open())
				{
					 buffer = Sys::readWholeFile(&fs);
					 reader.parse(buffer,savedval);
					 std::cout << savedval << '\n';
					 if (val["totalEpisodes"].asInt() != savedval["totalEpisodes"].asInt())
					 {
						 saveAnimeInfo(val);
					 }
					 else
						val = savedval;
				}
				fs.close();
				break;
			}
			else if(i == (data["NumAnime"].asInt() - 1))
			{
				/*val["watched"] = 0;
				fs.open("data/animeInfo/" + val["id"].asString() + ".json",std::ios::out);
				if (fs.is_open())
				{
					Json::StyledWriter writer;
					fs << writer.write(val);
				}
				fs.close();

				Json::Value v;
				v["name"] = val["title"].asString();
				v["id"] = val["id"].asString();
				data["Sources"].append(v);
				data["NumAnime"] = data["NumAnime"].asInt() + 1;

				fs.open("data/animeInfo.json",std::ios::trunc | std::ios::out);
				if (fs.is_open())
				{
					std::cout << "THIS RUNS\n";
					Json::StyledWriter writer;
					fs << writer.write(data);
				}
				fs.close();
				saveAnimeInfo(data);
			}
		}
	}*/

	try
	{
		animeInfo->loadWidgetsFromFile("Info Screen.txt");
	}
	catch (tgui::Exception e)
	{
		std::cout << e.what() << '\n';
	}


	val["image"] = "res/img/" + val["id"].asString() + ".png";
	auto pic = animeInfo->get<tgui::Picture>("ANIME COVER");

	//auto tex = tgui::Texture();
	//tex.loadFromPixelData(textures.back().get()->getSize(), textures.back().get()->copyToImage().getPixelsPtr());
	pic->getRenderer()->setTexture(val["image"].asCString());

	auto title = animeInfo->get<tgui::Label>("ANIME TITLE");
	title->setText(val["title"].asString());
	title->setScrollbarPolicy(tgui::Scrollbar::Policy::Automatic);
	title->getRenderer()->setScrollbarWidth(0.1f);

	auto status = animeInfo->get<tgui::Label>("STATUS");
	status->setText(val["status"].asString());

	auto released = animeInfo->get<tgui::Label>("YEAR OF RELEASE");
	released->setText(val["releaseDate"].asString());

	auto episodebtn = animeInfo->get<tgui::Button>("EPISODE BUTTON");
	auto episodelabel = animeInfo->get<tgui::Label>("Label1");
	auto downloadbtn = animeInfo->get<tgui::Button>("EPISODE DOWNLOAD BUTTON");

	auto scrollpanel = animeInfo->get<tgui::ScrollablePanel>("EPISODES ScrollablePanel");
	scrollpanel->getRenderer()->setScrollbarWidth(0.1f);
	int x = 0, y = 0;
	int firstUnwatched = -1;

	for (int i = 0; i < val["totalEpisodes"].asInt(); i++)
	{
		auto btn = tgui::Button::copy(episodebtn);
		auto eplabel = tgui::Label::copy(episodelabel);
		auto dbtn = tgui::Button::copy(downloadbtn);
		std::stringstream ss;

		btn->setVisible(true);
		dbtn->setVisible(true);
		eplabel->setVisible(true);
		btn->setPosition(30 + (x * 230), 122 + (y * 107));
		eplabel->setPosition(30 + (x * 230), 122 + (y *  107));
		dbtn->setPosition(btn->getPosition().x + 173, btn->getPosition().y + 18);
		if (val["episodes"][i]["watched"].asBool())
		{
			btn->getRenderer()->setTexture("res/animeInfo/EPISODE DOWNLOAD BUTTON.png");
		}
		else
		{
			if (firstUnwatched < 0)
				firstUnwatched = i;
		}

		ss << "ep. " << (val["episodes"][i]["number"].asInt());
		eplabel->setText(ss.str());
		scrollpanel->add(eplabel);
		scrollpanel->add(btn,ss.str());
		scrollpanel->add(dbtn);

		btn->onClick([&](int AA)
		{
				std::cout << "AA in function: " << AA << '\n';
				downloader.addRequest(ApiScheema::getstreaminglinks(jsons["animeInfo"]["episodes"][AA]["id"].asString(),ApiScheema::goGocdn), [=](Json::Value newvalue)
				{
					currentepisode = AA + 1;
					std::cout << "current episode: " << currentepisode << "on button: " << AA <<'\n';
					std::cout << newvalue << '\n';
					jsons["stream-links"] = newvalue;
					playVideo();
				});
				std::cout << "AA end function: " << AA << '\n';
		}, i);

		x++;
		if ((((i + 1) % 5) == 0) && i != 0)
		{
			x = 0;
			y++;
		}

	}

	auto back = animeInfo->get<tgui::Button>("BACK BUTTON");
	back->onClick([&]()
		{
			try
			{
					gui.removeAllWidgets();
				if (jsons.find("search") != jsons.end())
				{
					gui.add(searchpanel);
				}
				else
				{
					gui.add(mainMenu);
				}
			}
			catch (std::exception e)
			{
				std::cout << e.what() << '\n';
			}

		}
	);

	auto play = animeInfo->get<tgui::Button>("PLAY BUTTON");
	play->onPress([=]()
		{
			try 
			{
			downloader.addRequest(ApiScheema::getstreaminglinks(val["episodes"][firstUnwatched]["id"].asString()), [=](Json::Value newvalue)
				{
					currentepisode = firstUnwatched + 1;
					std::cout << "current episode play button: " << currentepisode << '\n';
					std::cout << newvalue << '\n';
					jsons["stream-links"] = newvalue;
					playVideo(firstUnwatched + 1);
				});

			}
			catch (Json::Exception e)
			{
				std::cout << e.what() << '\n';
			}
		}
	);

	auto description = animeInfo->get<tgui::Label>("DESCRIPTION");
	description->setText(val["description"].asString());
	description->setScrollbarPolicy(tgui::Scrollbar::Policy::Automatic);
	description->getRenderer()->setScrollbarWidth(0.1f);

	auto totalepisodes = animeInfo->get<tgui::Label>("AMOUNT OF EPISODES");
	totalepisodes->setText(val["totalEpisodes"].asString());
	
	mutex.lock();
	gui.add(animeInfo);
	mutex.unlock();
}

void Application::saveAnimeInfo(Json::Value data)
{
	std::fstream fs;
	auto val = jsons["animeInfo"];

	val["watched"] = 0;

	for (int i = 0; i < val["totalEpisodes"].asInt(); i++)
	{
		if (!val["episodes"][i].isMember("watched"))
		{
			val["episodes"][i]["watched"] = false;
			val["episodes"][i]["downloaded"] = false;
		}
		std::cout << "Called\n";
	}

	fs.open("data/animeInfo/" + val["id"].asString() + ".json", std::ios::out);
	if (fs.is_open())
	{
		Json::StyledWriter writer;
		fs << writer.write(val);
	}
	fs.close();

	Json::Value v;
	v["name"] = val["title"].asString();
	v["id"] = val["id"].asString();
	data["Sources"].append(v);
	data["NumAnime"] = data["NumAnime"].asInt() + 1;

	fs.open("data/animeInfo.json", std::ios::trunc | std::ios::out);
	if (fs.is_open())
	{
		std::cout << "THIS RUNS\n";
		Json::StyledWriter writer;
		fs << writer.write(data);
	}
	fs.close();
}

void Application::runDownloader()
{
	downloader.Run();
}

Application::Application()
{
	downloader.Init();
	//Net::Downloader::Init();
	this->downloadthread = std::thread([&] {runDownloader(); });
	playvideo = false;
	ApiScheema::loadScheema("config/scheema.json");

	currentepisode = 0;
	std::cout << "current episode: " << currentepisode << '\n';
	initWindow();
	initGui();



		if(Net::Https::hasInternet())
		{
			//std::cout << ApiScheema::getrecentepisodes() << '\n';
			downloader.addard(ApiScheema::getrecentepisodes(), [&](Json::Value val)
				{
					std::cout << val << '\n';
					jsons.insert({ "top-airing",val });
					loadMainMenu();
				});
		}
	

	//else
	//	loadMainMenu();
		//loadMainMenu();
	
	//tex.loadFromFile("res/logo.png");
	tgui::Picture::Ptr pic = tgui::Picture::create("res/LOGO.png");
	pic->setPosition("50% - 175", "50% - 175");
	pic->setSize(350, 350);
	//pic->setPosition(pic->getPosition().x - pic->getSize().x, pic->getPosition().y - pic->getSize().y);
	gui.add(pic);

	player = std::make_shared<Player>();
	player->updateSize(window.getSize().x, window.getSize().y);

}

Application::~Application()
{
	downloader.shouldrun = false;
	downloadthread.join();
	downloader.deInit();
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

