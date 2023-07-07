#pragma once
#include"config.h"
#include"Location.h"

class Scene {

public:
	Scene(std::list<location::Location> trianglePositions);

	std::vector<glm::vec3> trianglePosition;
	std::vector<glm::vec3> squarePosition;
	std::vector<glm::vec3> linePosition;
};