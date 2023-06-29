#pragma once

#include <fstream>
#include <list>
#include <iostream>

#include "Location.h"
#include "Connection.h"

namespace read {

    std::list<location::Location> read_location() {
        std::ifstream file("location.txt");
        if (!file) {
            std::cerr << "Failed to open 'location.txt' for reading." << std::endl;
        }

        std::list<location::Location> locations;
        std::string line;

        try{

        while (std::getline(file, line)) {

            //std::cout << line<<"\n";
            int id = std::stoi(line.substr(4));

            std::getline(file, line);
            //std::cout << line<<"\n";
            double x = std::stod(line.substr(3));

            std::getline(file, line);
            //std::cout << line << "\n";
            double y = std::stod(line.substr(3));

            std::getline(file, line);
            //std::cout << line << "\n";
            double z = std::stod(line.substr(3));

            std::getline(file, line);
            //std::cout << line << "\n";
            std::string name = line.substr(6);

                location::Location location(x, y, z, name);

                std::getline(file, line);
                while (std::getline(file, line)) {

                    if (line.compare("Traffic Rules:") == 0) {
                        while (std::getline(file, line)) {

                            if (line.empty()) {
                                break;
                            }

                            //std::cout << line << "\n";
                            double beginAt = std::stod(line.substr(9));

                            std::getline(file, line);
                            //std::cout << line << "\n";
                            double validDistance = std::stod(line.substr(15));

                            std::getline(file, line);
                            //std::cout << line << "\n";
                            int typeId = std::stoi(line.substr(7));

                            std::getline(file, line);
                            //std::cout << line << "\n";
                            double value = std::stod(line.substr(6));


                        }
                        std::getline(file, line);
                        //std::cout << line << "\n";
                    }

                    if (line.empty()) {
                        break;
                }

                //std::cout << line << "\n";
                int startId = std::stoi(line.substr(8));

                std::getline(file, line);
                //std::cout << line << "\n";
                int endId = std::stoi(line.substr(6));

                std::getline(file, line);
                //std::cout << line << "\n";
                double distance = std::stod(line.substr(9));

                

                connection::Connection connection(startId, endId, distance);



                location.set_connection(connection);
            }

            locations.push_back(location);
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error reading location data: " << e.what() << std::endl;
    }
        file.close();
        return locations;
    }

} 
