#pragma once
#include "config.h"
#include "Engine.h"
#include "scene.h"
#include "Location.h"


class App {

private:
	Engine* graphicsEngine;
	GLFWwindow* window;
	Scene* scene;

	double lastTime, currentTime;
	int numFrames;
	float frameTime;

	void build_glfw_window(int width, int height, bool debugMode);

	void calculateFrameRate();

public:
	App(int width, int height, std::list<location::Location> locations , bool debug);
	~App();
	void run();


};
