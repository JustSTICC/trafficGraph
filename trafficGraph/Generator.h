#pragma once

#include<list>
#include<iostream>
#include<random>

#include"Location.h"
#include"Connection.h"
#include"TrafficRule.h"
#include"RuleType.h"


namespace build {


	void generateLocations(std::list<location::Location>& graph, int numLocations) {

		std::random_device rd;
		std::mt19937 generator(rd());


		for (int i = 0; i < numLocations; i++) {

			std::uniform_real_distribution<double>  generatorXY(0, 100);
			std::uniform_real_distribution<double>  generatorZ(0, 100);
			
			
			double x = generatorXY(generator);
			double y = generatorXY(generator);
			double z = generatorZ(generator);

			std::string name = "Location" + std::to_string(i);

			location::Location newLocation{x, y, z, name};

			graph.push_back(newLocation);
		}
		int numConnections, counter=1;
		//std::cout << numLocations << std::endl;
		while (numLocations != 0) {
			numLocations /= 10;
			counter++;
		}
		int maxConn= counter+(counter/2), minConn = counter;
		std::uniform_int_distribution<int>  generatorConn(minConn, maxConn);

		for (location::Location locations : graph) {
			numConnections = generatorConn(generator);
			//std::cout << numConnections << std::endl;
			for (int i = 0; i < numConnections; i++) {
				int max = graph.size();
				std::uniform_int_distribution<int> generatorPoint(1, max);


				int startId = locations.get_id();
				int endId;
				do {
					endId = generatorPoint(generator);
				} while (endId == startId);

				location::Location start = locations;
				location::Location end;

				int br = 0;
				bool check = false;
				
				while(check=false){
					check = true;
					if (!start.get_list().empty()) {
						for (auto& connection : start.get_list()) {
							if (connection.get_endId() == endId) {
								endId = generatorPoint(generator);
								check = false;
								std::cout << "regenerated\n";
								break;
							}
						}
					}

				}
				for (auto& location : graph) {
					if (location.get_id() == endId) {
						end = location;
						//std::cout << endId << std::endl;;
					}
				}
				
				double xStart = start.get_x(), yStart = start.get_y(), zStart = start.get_z();
				double xEnd = end.get_x(), yEnd = end.get_y(), zEnd = end.get_z();

				double distance = sqrt(pow((xEnd - xStart), 2) + pow((yEnd - yStart), 2) + pow((zEnd - zStart), 2));

				connection::Connection newConnection{startId, endId, distance};

				for (auto& location : graph) {
					if (location.get_id() == startId) {
						start = location;
						location.set_connection(newConnection);
					}
					if (location.get_id() == endId) {
						end = location;
						location.set_connection(newConnection);
					}

				}

			}






		}

		
	/*
		for (location::Location locations : graph) {
			numConnections = generatorConn(generator);
			std::cout << numConnections << std::endl;
			for (int i = 0; i < numConnections; i++) {
				int max = graph.size();
				std::uniform_int_distribution<int> generatorPoint(1, max);
				
				
				int startId = generatorPoint(generator);
				int endId;
				do {
					endId = generatorPoint(generator);
				} while (endId == startId);

				location::Location start;
				location::Location end;

				int br = 0;
				bool check = false;
				for (auto& location : graph) {
					if (location.get_id() == startId) {
						start = location;
						//std::cout << startId << std::endl;;
						br++;
					}
				}
				if (!start.get_list().empty()) {
					for (auto& connection : start.get_list()) {
						if (connection.get_endId() == endId) {
							endId = generatorPoint(generator);
							std::cout << "regenerated\n";
							break;
						}
					}


				}
				for (auto& location : graph) {
					if (location.get_id() == endId) {
						end = location;
						//std::cout << endId << std::endl;;
						br++;
					}
				}
				if (br == 2) {
					double xStart = start.get_x(), yStart = start.get_y(), zStart = start.get_z();
					double xEnd = end.get_x(), yEnd = end.get_y(), zEnd = end.get_z();

					double distance = sqrt(pow((xEnd - xStart), 2) + pow((yEnd - yStart), 2) + pow((zEnd - zStart), 2));

					connection::Connection newConnection{startId, endId, distance};

					for (auto& location : graph) {
						if (location.get_id() == startId) {
							start = location;
							location.set_connection(newConnection);
						}
						if (location.get_id() == endId) {
							end = location;
							location.set_connection(newConnection);
						}

					}

				}
			}






		}*/
	}

	void generateConnections(std::list<location::Location>& graph, int numConnections) {

		std::random_device rd;
		std::mt19937 generator(rd());


		for (int i = 0; i < numConnections; i++) {

			int max = graph.size();

			std::uniform_int_distribution<int> generatorPoint(1, max);

			int startId = generatorPoint(generator);
			int endId;
			do {
				endId = generatorPoint(generator);
			} while (endId == startId);
			
			location::Location start;
			location::Location end;

			int br = 0;
			bool check = false;
			for (auto& location : graph) {
				if (location.get_id() == startId) {
					start = location;
					//std::cout << startId << std::endl;;
					br++;
				}
			}
			if (!start.get_list().empty()) {
				for (auto& connection : start.get_list()) {
					if (connection.get_endId() == endId) {
						endId = generatorPoint(generator);
						std::cout << "regenerated\n";
						break;
					}
				}


			}
			for (auto& location : graph) {
				if (location.get_id() == endId) {
					end = location;
					//std::cout << endId << std::endl;;
					br++;
				}
			}
			if (br == 2) {
				double xStart = start.get_x(), yStart = start.get_y(), zStart = start.get_z();
				double xEnd = end.get_x(), yEnd = end.get_y(), zEnd = end.get_z();

				double distance = sqrt(pow((xEnd - xStart), 2) + pow((yEnd - yStart), 2) + pow((zEnd - zStart), 2));

				connection::Connection newConnection{startId, endId, distance};

				for (auto& location : graph) {
					if (location.get_id() == startId) {
						start = location;
						location.set_connection(newConnection);
					}
					if (location.get_id() == endId) {
						end = location;
						location.set_connection(newConnection);
					}

				}

			}
			std::cout << startId << " " << endId << "\n";




		}






	}
}



