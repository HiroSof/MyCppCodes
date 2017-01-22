#pragma once
#include <Leap.h>

#pragma comment(lib,"leap.lib")

class CLMListener : public Leap::Listener{
public:
	CLMListener();
	~CLMListener();

	//Leap::Listener Method
	void onInit(const Leap::Controller &Control);
	void onConnect(const Leap::Controller &Control);
	void onDisconnect(const Leap::Controller &Control);
	void onExit(const Leap::Controller &Control);
	void onFrame(const Leap::Controller &Control);
	void onFocusGained(const Leap::Controller &Control);
	void onFocusLost(const Leap::Controller &Control);
	void onServiceConnect(const Leap::Controller &Control);
	void onServiceDisconnect(const Leap::Controller &Control);
	void onDeviceChange(const Leap::Controller &Control);

};

