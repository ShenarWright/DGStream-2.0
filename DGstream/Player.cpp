#include "Player.h"

void* Player::get_proc_address_mpv(void* unused, const char* name)
{
	//std::cout << name << '\n';
	(void)unused;
	return ((void*)sf::Context::getFunction(name));
}


void Player::initgui()
{
	progress = tgui::ProgressBar::create();
	slider = tgui::Slider::create();
	posLabel = tgui::Label::create();
	tbtn = tgui::ToggleButton::create();
	qual = tgui::ComboBox::create();
	forward = tgui::Button::create("-->");
	backward = tgui::Button::create("<--");

	tbtn->setSize(60, 60);
	tbtn->setPosition(400, 400);
	posLabel->getRenderer()->setTextColor(tgui::Color::Red);

	progress->setMaximum(1000);
	slider->setMaximum(1000);

	slider->getRenderer()->setOpacity(0.1f);

	slider->onValueChange([&]() {goTop(slider->getValue() / 10); });
	tbtn->onClick([&]() {ispaused() ? play() : pause();  });
	qual->setSize(100, 50);
	qual->setPosition(200, height - 50);
	qual->addItem("Auto");
	qual->addItem("360p");
	qual->addItem("480p");
	qual->addItem("720p");
	qual->setSelectedItemByIndex(0);

	forward->onClick([&]() {seek(10); });
	backward->onClick([&]() {seek(-10); });
	hud = false;
}

void Player::updateSize()
{
	progress->setSize(width, 30);
	slider->setSize(progress->getSize());

	forward->setSize(30, 30);
	backward->setSize(30, 30);
}

void Player::updatePosition()
{
	tbtn->setPosition(400, height - 100);
	progress->setPosition(0, height - 150);
	slider->setPosition(progress->getPosition());
	posLabel->setPosition(0, height - 100);
	qual->setPosition(200, height - 100);
	forward->setPosition(550, height - 100);
	backward->setPosition(500, height - 100);
}

void Player::updatelabel()
{
	std::stringstream ss;
	posLabel->setTextSize(20);

	if (pos > 59)
	{
		int mins = pos / 60;
		int secs = (int)pos % 60;

		if (mins > 59)
		{
			int hr = mins / 60;
			mins = mins % 60;
			ss << hr << ':';
		}

		if (mins < 10)
			ss << "0" << mins;
		else
			ss << mins;

		if (secs < 10)
			ss << ":0" << secs;
		else
			ss << ':' << secs;
	}
	else
	{
		if (pos < 10)
			ss << "00" << ":0" << (int)pos;
		else
			ss << "00" << ':' << (int)pos;
	}

	ss << " / ";
	if (duration > 59)
	{
		int mins = duration / 60;
		int secs = (int)duration % 60;

		if (mins > 59)
		{
			int hr = mins / 60;
			mins = mins % 60;
			ss << hr << ':';
		}

		if (mins < 10)
		{
			ss << "0" << mins;
		}
		else
			ss << mins;


		if (secs < 10)
			ss << ":0" << secs;
		else
			ss << ':' << secs;
	}
	else
	{
		if (duration < 10)
			ss << "00:" << "0" << (int)duration;
		else
			ss << "00:" << (int)duration;
	}


	posLabel->setText(ss.str());
}

void Player::showHud(bool show)
{
	hud = show;
	
	if (hud)
	{
		progress->showWithEffect(tgui::ShowAnimationType::SlideFromBottom, 100.f);
		slider->showWithEffect(tgui::ShowAnimationType::SlideFromBottom, 100.f);
		posLabel->showWithEffect(tgui::ShowAnimationType::SlideFromBottom, 100.f);
		tbtn->showWithEffect(tgui::ShowAnimationType::SlideFromBottom, 100.f);
		qual->showWithEffect(tgui::ShowAnimationType::SlideFromBottom, 100.f);
		forward->showWithEffect(tgui::ShowAnimationType::SlideFromBottom ,100.f);
		backward->showWithEffect(tgui::ShowAnimationType::SlideFromBottom, 100.f);
	}
	else
	{
		progress->hideWithEffect(tgui::ShowAnimationType::SlideToBottom, 100.f);
			slider->hideWithEffect(tgui::ShowAnimationType::SlideToBottom, 100.f);
		posLabel->hideWithEffect(tgui::ShowAnimationType::SlideToBottom, 100.f);
		tbtn->hideWithEffect(tgui::ShowAnimationType::SlideToBottom, 100.f);
		qual->hideWithEffect(tgui::ShowAnimationType::SlideToBottom, 100.f);
		forward->hideWithEffect(tgui::ShowAnimationType::SlideToBottom, 100.f);
		backward->hideWithEffect(tgui::ShowAnimationType::SlideToBottom, 100.f);
	}
}

bool Player::isHudshown()
{
	return hud;
}

Player::Player(std::string str)
//mpv(NULL),width(800),height(640)
{
	duration = 0;
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
	int idk = 1;
	gl_init_params = { &get_proc_address_mpv, nullptr };

	cparams[0] = { MPV_RENDER_PARAM_API_TYPE, (void*)MPV_RENDER_API_TYPE_OPENGL };
	cparams[1] = { MPV_RENDER_PARAM_OPENGL_INIT_PARAMS, &gl_init_params };
	cparams[2] = { MPV_RENDER_PARAM_INVALID,NULL };
	//cparams[2] = { MPV_RENDER_PARAM_,NULL };

	context = NULL;
	if (mpv_render_context_create(&context, mpv, cparams) < 0)
	{
		std::cout << "Failed to create context\n";
		return;
	}

	//init();
	load(str);

}

Player::Player()
{
	duration = 0;
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

	cparams[0] = { MPV_RENDER_PARAM_API_TYPE, (void*)MPV_RENDER_API_TYPE_OPENGL };
	cparams[1] = { MPV_RENDER_PARAM_OPENGL_INIT_PARAMS, &gl_init_params };
	cparams[2] = { MPV_RENDER_PARAM_INVALID,NULL };

	context = NULL;
	if (mpv_render_context_create(&context, mpv, cparams) < 0)
	{
		std::cout << "Failed to create context\n";
		return;
	}

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
		int arr[2] = { width,height };

		mpv_opengl_fbo fbo = { 0,width,height,1 };
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
}

void Player::setSize(int x, int y)
{
	width = x;
	height = y;

	updateSize();
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
}

void Player::play()
{
	cmd[0] = "cycle";
	cmd[1] = "pause";
	cmd[2] = NULL;
	mpv_command_async(mpv, 0, cmd);
	paused = true;
}

bool Player::ispaused()
{
	return paused;
}

void Player::seek(float offset)
{
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
					std::cout << "time:" << a << '\n';
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

		}
	}
	long float a = (pos / duration) * 1000;
	progress->setValue(a);
	//slider->setValue(a);
	updatelabel();

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
	gui.add(progress);
	gui.add(slider);
	gui.add(posLabel);
	gui.add(tbtn);
	gui.add(qual);
	gui.add(forward);
	gui.add(backward);
}

long double Player::getPos()
{
	return pos;
}

long double Player::getDuration()
{
	//long double a = 0;
	//mpv_get_property(mpv, "duration", MPV_FORMAT_DOUBLE, &a);
	return duration;
}
