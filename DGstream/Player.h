#pragma once
#include <SFML/Graphics.hpp>
#include <mpv/client.h>
#include <mpv/render_gl.h>
#include <iostream>
#include <string>
#include <sstream>
#include <TGUI/TGUI.hpp>


class Player
{
private:
	static void* get_proc_address_mpv(void* unused, const char* name);
	mpv_handle* mpv;
	mpv_opengl_init_params gl_init_params;
	mpv_render_context* context;
	mpv_opengl_fbo fbo;
	const char* cmd[4];
	mpv_render_param rparams[3];
	mpv_render_param cparams[3];
	int width, height;
	bool hasdata;
	bool paused;
	bool hud;

	long double pos;
	long double duration;

	tgui::ProgressBar::Ptr progress;
	tgui::Slider::Ptr slider;
	tgui::Label::Ptr posLabel;
	tgui::ToggleButton::Ptr tbtn;
	tgui::ComboBox::Ptr qual;
	tgui::Button::Ptr forward;
	tgui::Button::Ptr backward;

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
	bool ispaused();
	void seek(float offset);
	void goTo(float position);
	void goTop(float percent);
	void handleEvents();
	void takeScreenShot(std::string path);
	void addWidgetstoGui(tgui::Gui& gui);

	long double getPos();
	long double getDuration();
};

