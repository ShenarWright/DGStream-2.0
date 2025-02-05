#include "Player.h"

void* Player::get_proc_address_mpv(void* unused, const char* name)
{
	//std::cout << name << '\n';
	//std::cout << (sf::Context::isExtensionAvailable(name)  ? "True" : "False") << '\n';
	(void)unused;
	return ((void*)sf::Context::getFunction(name));
}

int M::mx = 0;
int M::my = 0;

void Player::initgui()
{
	//hud = false;
	widgets = tgui::Group::create();
	widgets->loadWidgetsFromFile("player.txt");

	slider = tgui::Slider::create();
	progress = widgets->get<tgui::ProgressBar>("PROGRESS BAR");
	startlabel = widgets->get<tgui::Label>("TIME STAMPS START");
	endlabel = widgets->get<tgui::Label>("TIME STAMP END");
	tbtn = widgets->get<tgui::ToggleButton>("PLAY BUTTON");
	forward = widgets->get<tgui::Button>("NEXT BUTTONS");
	backward = widgets->get<tgui::Button>("PREVIOUS BUTTON");
	settings = widgets->get<tgui::Button>("SETTINGS BUTTON");
	volume = widgets->get<tgui::Button>("VOLUME BUTTON");
	download = widgets->get<tgui::Button>("DOWNLOAD BUTTON");
	fullscreen = widgets->get<tgui::Button>("FULLSCREEN");
	sub = widgets->get<tgui::Button>("SUB");
	dub = widgets->get<tgui::Button>("DUB");
	back = widgets->get<tgui::Button>("BACK BUTTON");
	icons = widgets->get<tgui::Picture>("ICONS");
	background = widgets->get<tgui::Picture>("BACKGROUND");
	animetitle = widgets->get<tgui::Label>("ANIME TITLE");
	slider->setPosition(progress->getPosition());
	slider->setSize(progress->getSize());
	slider->getRenderer()->setOpacity(0.f);
	widgets->add(slider);
	slider->setMaximum(1000);
	progress->setMaximum(1000);
	tbtn->onClick([&]() {ispaused() ? play() : pause();  });
	slider->onValueChange([&]() {goTop(slider->getValue() / 10); });
	//
	/*forward->onClick([&]() {
		cmd[0] = "playlist-next";
		//cmd[1] = "force";
		cmd[1] = NULL;
		mpv_command_async(mpv, 0, cmd);

		});
	backward->onClick([&]() {
		cmd[0] = "playlist-prev";
		//cmd[1] = "force";
		cmd[1] = NULL;
		mpv_command_async(mpv, 0, cmd);
		});*/
}

void Player::updateSize(int width, int height)
{
	this->width = width;
	this->height = height;
}

void Player::updatePosition()
{

}

void Player::updatelabel()
{
	std::stringstream start;
	std::stringstream end;

	if (pos > 59)
	{
		int mins = (int)pos / 60;
		int secs = (int)pos % 60;

		if (mins > 59)
		{
			int hr = mins / 60;
			mins = mins % 60;
			start << hr << ':';
		}

		if (mins < 10)
			start << "0" << mins;
		else
			start << mins;

		if (secs < 10)
			start << ":0" << secs;
		else
			start << ':' << secs;
	}
	else
	{
		if (pos < 10)
			start << "00" << ":0" << (int)pos;
		else
			start << "00" << ':' << (int)pos;
	}

	//ss << " / ";
	if (duration > 59)
	{
		int mins = (int)duration / 60;
		int secs = (int)duration % 60;

		if (mins > 59)
		{
			int hr = mins / 60;
			mins = mins % 60;
			end << hr << ':';
		}

		if (mins < 10)
		{
			end << "0" << mins;
		}
		else
			end << mins;


		if (secs < 10)
			end << ":0" << secs;
		else
			end << ':' << secs;
	}
	else
	{
		if (duration < 10)
			end << "00:" << "0" << (int)duration;
		else
			end << "00:" << (int)duration;
	}


	startlabel->setText(start.str());
	endlabel->setText(end.str());
}

void Player::showHud(bool show)
{
	if (!progress->isAnimationPlaying())
	{
		hud = show;
		if (hud)
		{
			if (!progress->isVisible())
			{
				progress->showWithEffect(tgui::ShowEffectType::SlideFromBottom, 45);
				tbtn->showWithEffect(tgui::ShowEffectType::SlideFromBottom, 45);
				forward->showWithEffect(tgui::ShowEffectType::SlideFromBottom, 45);
				backward->showWithEffect(tgui::ShowEffectType::SlideFromBottom, 45);
				startlabel->showWithEffect(tgui::ShowEffectType::SlideFromBottom, 45);
				endlabel->showWithEffect(tgui::ShowEffectType::SlideFromBottom, 45);
				download->showWithEffect(tgui::ShowEffectType::SlideFromBottom, 45);
				volume->showWithEffect(tgui::ShowEffectType::SlideFromBottom, 45);
				settings->showWithEffect(tgui::ShowEffectType::SlideFromBottom, 45);
				sub->showWithEffect(tgui::ShowEffectType::SlideFromBottom, 45);
				dub->showWithEffect(tgui::ShowEffectType::SlideFromBottom, 45);
				icons->showWithEffect(tgui::ShowEffectType::SlideFromBottom, 45);
				back->showWithEffect(tgui::ShowEffectType::SlideFromTop, 45);
				animetitle->showWithEffect(tgui::ShowEffectType::SlideFromTop, 45);
				fullscreen->showWithEffect(tgui::ShowEffectType::SlideFromBottom, 45);
				background->showWithEffect(tgui::ShowEffectType::SlideFromBottom, 45);
			}
			//std::cout << "HUD\n";
		}
		else
		{
			if (progress->isVisible())
			{
				progress->hideWithEffect(tgui::ShowEffectType::SlideToBottom, 100);
				tbtn->hideWithEffect(tgui::ShowEffectType::SlideToBottom, 100);
				forward->hideWithEffect(tgui::ShowEffectType::SlideToBottom, 100);
				backward->hideWithEffect(tgui::ShowEffectType::SlideToBottom, 100);
				startlabel->hideWithEffect(tgui::ShowEffectType::SlideToBottom, 100);
				endlabel->hideWithEffect(tgui::ShowEffectType::SlideToBottom, 100);
				download->hideWithEffect(tgui::ShowEffectType::SlideToBottom, 100);
				volume->hideWithEffect(tgui::ShowEffectType::SlideToBottom, 100);
				settings->hideWithEffect(tgui::ShowEffectType::SlideToBottom, 100);
				sub->hideWithEffect(tgui::ShowEffectType::SlideToBottom, 100);
				dub->hideWithEffect(tgui::ShowEffectType::SlideToBottom, 100);
				icons->hideWithEffect(tgui::ShowEffectType::SlideToBottom, 100);
				back->hideWithEffect(tgui::ShowEffectType::SlideToTop, 100);
				animetitle->hideWithEffect(tgui::ShowEffectType::SlideToTop, 100);
				fullscreen->hideWithEffect(tgui::ShowEffectType::SlideToBottom, 100);
				background->hideWithEffect(tgui::ShowEffectType::SlideToBottom, 100);
			}
		}
	}
	hiddentimer.restart();
}

bool Player::isHudshown()
{
	return hud;
}

Player::Player(std::string str)
//mpv(NULL),width(800),height(640)
{
	initgui();

	glGenFramebuffers(1, &framebufferobject);
	fbo.fbo = framebufferobject;

	paused = false;
	mpv = mpv_create();
	if (mpv_initialize(mpv) < 0)
	{
		std::cout << "Error init\n";
		return;
	}

	//mpv = mpv_create_client(mpv, "main");

	mpv_request_log_messages(mpv, "debug");
	mpv_set_option_string(mpv, "vd-lavc-threads", "4");
	mpv_set_option_string(mpv, "fbo-format", "rgba8");
	mpv_set_option_string(mpv, "opengl-pbo", "yes");
	mpv_set_option_string(mpv, "audio-channels", "stereo");
	mpv_observe_property(mpv, 0, "time-pos", MPV_FORMAT_DOUBLE);
	mpv_observe_property(mpv, 0, "duration", MPV_FORMAT_DOUBLE);
	mpv_observe_property(mpv, 0, "demuxer-cache-duration", MPV_FORMAT_DOUBLE);
	int idk = 1;
	gl_init_params = { &get_proc_address_mpv, nullptr };

	cparams[0] = { MPV_RENDER_PARAM_API_TYPE, const_cast<char*>(MPV_RENDER_API_TYPE_OPENGL) };
	cparams[1] = { MPV_RENDER_PARAM_OPENGL_INIT_PARAMS, &gl_init_params };
	//cparams[] = { MPV_RENDER_PARAM_ADVANCED_CONTROL,&idk };
	cparams[2] = { MPV_RENDER_PARAM_INVALID,nullptr };
	//cparams[2] = { MPV_RENDER_PARAM_,NULL };

	context = NULL;
	int err = 0;
	if ((err = mpv_render_context_create(&context, mpv, cparams)) < 0)
	{
		std::cout << mpv_error_string(err) << '\n';
		std::cout << "Failed to create context\n";
		return;
	}


	//init();
	load(str);

	duration = 0;

}

Player::Player()
{
	initgui();
	paused = false;
	mpv = mpv_create();
	if (mpv_initialize(mpv) < 0)
	{
		std::cout << "Error init\n";
		return;
	}

	mpv_request_log_messages(mpv, "debug");
	mpv_set_option_string(mpv, "vd-lavc-threads", "4");
	mpv_set_option_string(mpv, "fbo-format", "rgba8");
	mpv_set_option_string(mpv, "opengl-pbo", "yes");
	mpv_set_option_string(mpv, "audio-channels", "stereo");
	mpv_observe_property(mpv, 0, "time-pos", MPV_FORMAT_DOUBLE);
	mpv_observe_property(mpv, 0, "duration", MPV_FORMAT_DOUBLE);
	mpv_observe_property(mpv, 0, "demuxer-cache-duration", MPV_FORMAT_DOUBLE);
	mpv_observe_property(mpv, 0, "volume", MPV_FORMAT_DOUBLE);

	int idk = 1;
	gl_init_params = { &get_proc_address_mpv, nullptr };

	cparams[0] = { MPV_RENDER_PARAM_API_TYPE, const_cast<char*>(MPV_RENDER_API_TYPE_OPENGL) };
	cparams[1] = { MPV_RENDER_PARAM_OPENGL_INIT_PARAMS, &gl_init_params };
	//cparams[2] = { MPV_RENDER_PARAM_ADVANCED_CONTROL,&idk };
	cparams[2] = { MPV_RENDER_PARAM_INVALID, nullptr };

	context = NULL;
	int err = 0;
	if ((err = mpv_render_context_create(&context, mpv, cparams)) < 0)
	{
		std::cout << "Failed to create context\n";
		std::cout << mpv_error_string(err) << '\n';
		return;
	}

	duration = 0;
	//init();
}


Player::~Player()
{
	mpv_render_context_free(context);
	mpv_destroy(mpv);
}

void Player::render()
{
	if (hasdata)
	{
		void* data = NULL;
		mpv_render_context_set_update_callback(context, [](void* data) {}, data);
		//int arr[2] = { width,height };
		mpv_opengl_fbo fbo = { framebufferobject,width,height,0 };
		//fbo.fbo = 0;
		//fbo.w = 800;
		//fbo.h = 640;
		//fbo.internal_format = 0;
		int one = 1;
		rparams[0] = { MPV_RENDER_PARAM_OPENGL_FBO, &fbo };
		rparams[1] = { MPV_RENDER_PARAM_FLIP_Y, &one };
		//rparams[2] = {MPV_RENDER_PARAM_OPENGL_FBO,}


		//if (!context)
		{
			mpv_render_context_update(context);

			mpv_render_context_render(context, rparams);
		}
	}
	else
		std::cout << "No File Loaded\n";
}

void Player::load(std::string str)
{
	cmd[0] = "loadfile";
	cmd[1] = str.c_str();
	cmd[2] = NULL;

	//const char* tcmd[] = { "loadfile", "C:/Users/dante/Downloads/ep.220.1656255387.4801.ts", NULL };

	mpv_command_async(mpv, 0, cmd);
	hasdata = true;
	showHud(true);
	hiddentimer.restart();
}

void Player::setSize(int x, int y)
{
	updateSize(x,y);
	updatePosition();
}

bool Player::hasData()
{
	return hasdata;
}

void Player::pause()
{
	//cmd = { "h","h",NULL };
	std::cout << "Paused\n";
	cmd[0] = "cycle";
	cmd[1] = "pause";
	cmd[2] = NULL;
	mpv_command_async(mpv, 0, cmd);
	paused = true;
	tbtn->setDown(true);
}

void Player::play()
{
	cmd[0] = "cycle";
	cmd[1] = "pause";
	cmd[2] = NULL;
	mpv_command_async(mpv, 0, cmd);
	paused = false;
	tbtn->setDown(false);

}

void Player::stop()
{
	cmd[0] = "stop";
	cmd[1] = NULL;

	mpv_command_async(mpv, 0, cmd);
}

bool Player::ispaused()
{
	return paused;
}

void Player::seek(float offset)
{
	hiddentimer.restart();
	std::stringstream ss;
	std::string s;
	ss << offset;
	s = ss.str();
	cmd[0] = "seek";
	cmd[1] = s.c_str();
	cmd[2] = "relative+exact";
	cmd[3] = NULL;
	mpv_command_async(mpv, 0, cmd);
}

void Player::goTo(float position)
{
	hiddentimer.restart();
	std::stringstream ss;
	std::string s;
	ss << position;
	s = ss.str();
	cmd[0] = "seek";
	cmd[1] = s.c_str();
	cmd[2] = "absolute";
	cmd[3] = NULL;
	std::cout << mpv_error_string(mpv_command_async(mpv, 0, cmd));
}

void Player::goTop(float percent)
{
	hiddentimer.restart();
	std::stringstream ss;
	std::string s;
	ss << percent;
	s = ss.str();
	cmd[0] = "seek";
	cmd[1] = s.c_str();
	cmd[2] = "absolute-percent";
	cmd[3] = NULL;
	std::cout << mpv_error_string(mpv_command_async(mpv, 0, cmd));
}

void Player::handleEvents()
{
	while (mpv)
	{
		mpv_event* event = mpv_wait_event(mpv, 0);
		if (event->event_id == MPV_EVENT_NONE)
			break;

		switch (event->event_id)
		{

		default:
			break;
		case MPV_EVENT_PROPERTY_CHANGE:
		{
			mpv_event_property* event_prop = (mpv_event_property*)event->data;
			//std::cout << event_prop->name << "\n";
			if (std::strcmp(event_prop->name, "time-pos") == 0)
			{
				if (event_prop->format == MPV_FORMAT_DOUBLE) {
					pos = *(double*)event_prop->data;
					//std::cout << "time:" << pos << '\n';
				}
				else if (event_prop->format == MPV_FORMAT_NONE) {
					std::cout << "Not yet\n";
				}

			}
			if (std::strcmp(event_prop->name, "duration") == 0)
			{
				if (event_prop->format == MPV_FORMAT_DOUBLE) {
					duration = *(double*)event_prop->data;
					//std::cout << "time:" << pos << '\n';
				}
				else if (event_prop->format == MPV_FORMAT_NONE) {
					std::cout << "Not yet\n";
				}
			}
			if (std::strcmp(event_prop->name, "demuxer-cache-duration") == 0)
			{
				if (event_prop->format == MPV_FORMAT_DOUBLE) {
					int a = *(double*)event_prop->data;
					//std::cout << "time:" << a << '\n';
				}
				else if (event_prop->format == MPV_FORMAT_NONE) {
					std::cout << "Not yet\n";
				}
			}
			if (std::strcmp(event_prop->name, "volume") == 0)
			{
				if (event_prop->format == MPV_FORMAT_DOUBLE) {
					int a = *(double*)event_prop->data;
					std::cout << "volume:" << a << '\n';
				}
				else if (event_prop->format == MPV_FORMAT_NONE) {
					std::cout << "Not yet\n";
				}
			}
			
		}
		break;
		case MPV_EVENT_LOG_MESSAGE:
			mpv_event_log_message* msg = (mpv_event_log_message*)event->data;
			std::fstream fs;
			//std::cout << "Prefix:" << msg->prefix << " /LEVEL/ " << msg->level << " /MESSAGE/ " <<msg->text << '\n';
			
			/*fs.open("config/logs.txt", std::ios_base::app);
			if (fs.is_open())
			{
				fs << "Prefix:" << msg->prefix << " /LEVEL/ " << msg->level << " /MESSAGE/ " << msg->text;

			}
			fs.close();*/
			break;

		}
	}
	int a = (pos / duration) * 1000;
	progress->setValue(a);
	//slider->setValue(a);
	updatelabel();

	if (hiddentimer.getElapsedTime().asSeconds() >= 5.f)
	{
		if(hud)
			showHud(false);
		//std::cout << "CALLED\n";
		hiddentimer.restart();	
	}
	if (M::isMouseMoving())
	{
		if (!hud)
		{
			showHud(true);
		}
		hiddentimer.restart();
	}
	hud = progress->isVisible();

	//std::cout << (hud ? "True" : "False") << "\n";
	//else hiddentimer.restart();

}

void Player::takeScreenShot(std::string path)
{
	//std::cout << "Screenshot\n";
	//cmd[0] = "screenshot";
	cmd[0] = "screenshot-to-file";
	cmd[1] = path.c_str();
	cmd[2] = "subtitles";
	cmd[3] = NULL;

	std::cout << mpv_error_string(mpv_command_async(mpv, 0, cmd));
}

void Player::addWidgetstoGui(tgui::Gui& gui)
{
	//gui.add(progress);
	//gui.add(slider);
	//gui.add(posLabel);
	//gui.add(tbtn);
	//gui.add(qual);
	//gui.add(forward);
	//gui.add(backward);
	gui.add(widgets);
}

long double Player::getPos()
{
	return pos;
}

void Player::setTitle(std::string title)
{
	animetitle->setText(title);
}

void Player::onBackButtonPressed(std::function<void(void)> func)
{
	back->onClick(func);
}

void Player::onDownloadButtonPressed(std::function<void(void)> func)
{
	download->onClick(func);
}

void Player::onSettingsButtonPressed(std::function<void(void)> func)
{
	settings->onClick(func);
}

void Player::onDubButtonPressed(std::function<void(void)> func)
{
	dub->onClick(func);
}

void Player::onNextButtonPressed(std::function<void(void)> func)
{
	forward->onClick(func);
}

void Player::onPreviousButtonPressed(std::function<void(void)> func)
{
	backward->onClick(func);
}

void Player::onSubButtonPressed(std::function<void(void)> func)
{
	sub->onClick(func);
}

void Player::addToPlaylist(std::string url)
{
	cmd[0] = "loadfile";
	cmd[1] = url.c_str();
	cmd[2] = "append";
	cmd[3] = NULL;
	mpv_error_string(mpv_command_async(mpv, 0, cmd));
}

long double Player::getDuration()
{
	//long double a = 0;
	//mpv_get_property(mpv, "duration", MPV_FORMAT_DOUBLE, &a);
	return duration;
}
