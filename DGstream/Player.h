#pragma once
#include <SFML/Graphics.hpp>
#include <mpv/client.h>
#include <mpv/render_gl.h>
#include <iostream>
#include <string>
#include <sstream>
#include <TGUI/TGUI.hpp>
#include <fstream>

struct M
{
	static int mx;
	static int my;
	static bool isMouseMoving()
	{
		if (sf::Mouse::getPosition().x != mx || sf::Mouse::getPosition().y != my)
		{
			mx = sf::Mouse::getPosition().x;
			my = sf::Mouse::getPosition().y;
			//std::cout << "True\n";
			return true;
		}

		//std::cout << "false\n";
		mx = sf::Mouse::getPosition().x;
		my = sf::Mouse::getPosition().y;
		return false;
	}
};

class Player
{
private:
	static void* get_proc_address_mpv(void* unused, const char* name);
	mpv_handle* mpv;
	mpv_opengl_init_params gl_init_params;
	mpv_render_context* context;
	mpv_opengl_fbo fbo;
	const char* cmd[4];
	mpv_render_param rparams[4];
	mpv_render_param cparams[4];
	int width, height;
	bool hasdata;
	bool paused;
	bool hud;

	long double pos;
	long double duration;

	tgui::ProgressBar::Ptr progress;
	tgui::Slider::Ptr slider;
	tgui::Label::Ptr startlabel;
	tgui::Label::Ptr endlabel;
	tgui::ToggleButton::Ptr tbtn;
	//tgui::ComboBox::Ptr qual;
	tgui::Button::Ptr forward;
	tgui::Button::Ptr backward;
	tgui::Group::Ptr widgets;
	tgui::Button::Ptr download;
	tgui::Button::Ptr Full;
	tgui::Button::Ptr settings;
	tgui::Button::Ptr volume;
	tgui::Button::Ptr sub;
	tgui::Button::Ptr dub;
	tgui::Button::Ptr back;
	tgui::Button::Ptr fullscreen;
	tgui::Picture::Ptr icons;
	tgui::Picture::Ptr background;
	tgui::Label::Ptr animetitle;

	sf::Clock hiddentimer;

	void initgui();
	void updateSize();
	void updatePosition();
	void updatelabel();
public:
	Player(std::string str);
	Player();
	~Player();

	void showHud(bool show);
	bool isHudshown();
	void render();
	void load(std::string str);
	void setSize(int x, int y);
	bool hasData();
	void pause();
	void play();
	void stop();
	bool ispaused();
	void seek(float offset);
	void goTo(float position);
	void goTop(float percent);
	void handleEvents();
	void takeScreenShot(std::string path);
	void addWidgetstoGui(tgui::Gui& gui);

	void addToPlaylist(std::string url);
	void setTitle(std::string title);

	void onBackButtonPressed(std::function<void(void)> func);
	void onDownloadButtonPressed(std::function<void(void)> func);
	void onSettingsButtonPressed(std::function<void(void)> func);
	void onSubButtonPressed(std::function<void(void)> func);
	void onDubButtonPressed(std::function<void(void)> func);
	void onNextButtonPressed(std::function<void(void)> func);
	void onPreviousButtonPressed(std::function<void(void)> func);

	long double getPos();
	long double getDuration();
};

