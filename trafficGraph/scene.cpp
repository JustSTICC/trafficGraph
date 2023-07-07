#include "scene.h"

Scene::Scene(std::list<location::Location> trianglePositions) {

	std::vector<location::Location> positions;
	for (location::Location location : trianglePositions) {
		positions.push_back(location);
	}

	float x, y, z;
	for (location::Location location : trianglePositions) {
		x = static_cast<float>(location.get_x()/10000);
		y = static_cast<float>(location.get_y()/10000);
		z = static_cast<float>(location.get_z()/100);
		trianglePosition.push_back(glm::vec3(x, y, z));

	}

	for (location::Location location : trianglePositions) {
		float x1, y1, z1;
		x1 = static_cast<float>(location.get_x() / 10000);
		y1 = static_cast<float>(location.get_y() / 10000);
		z1 = static_cast<float>(location.get_z() / 100);
		for (connection::Connection connection : location.get_list()) {
			if (connection.get_startId() == location.get_id()) {
				float x2, y2, z2;
				location::Location endLocation = positions[connection.get_endId()-1];
				x2 = static_cast<float>(endLocation.get_x() / 10000);
				y2 = static_cast<float>(endLocation.get_y() / 10000);
				z2 = static_cast<float>(endLocation.get_z() / 100);
				linePosition.push_back(glm::vec3(x1, y1, z1));
				linePosition.push_back(glm::vec3(x2, y2, z2));
			}
		}
	}

	for (int i = 0; i < trianglePosition.size(); i++) std::cout << "[ " << trianglePosition[i].x << ", " << trianglePosition[i].y << ", " << trianglePosition[i].z << " ], ";
	std::cout << std::endl;
	for (int i = 0; i < linePosition.size(); i++) std::cout << "[ " << linePosition[i].x << ", " << linePosition[i].y << ", " << linePosition[i].z << " ], ";
	std::cout << std::endl;
	/*
	float x = -0.6f;
	for (float y = -1.0f; y < 1.0f; y += 0.2f) {

		trianglePosition.push_back(glm::vec3(x, y, 0.0f));

	}*/
	/*x = 0.0f;
	for (float y = -1.0f; y < 1.0f; y += 0.2f) {

		squarePosition.push_back(glm::vec3(x, y,0.0f));

	}*/

}