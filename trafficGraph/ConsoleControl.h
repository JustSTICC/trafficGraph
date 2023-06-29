#pragma once
#include<iostream>
#include<list>

#include "Location.h"
#include "Connection.h"
#include "TrafficRule.h"
#include "RuleType.h"

namespace console {

    void menu() {

        std::cout<< "------------------MENU------------------ " << std::endl;
        std::cout << "1. Create Location" << std::endl;
        std::cout << "2. Create Connection" << std::endl;
        std::cout << "3. Display Graph" << std::endl;
        std::cout << "4. Load Save" << std::endl;
        std::cout << "5. Override Save" << std::endl;
        std::cout << "6. Generate Graph" << std::endl;
        std::cout << "7. Find path between 2 locations(Dijkstra's)" << std::endl;
        std::cout << "8. Find path between 2 locations(Bidirectional)" << std::endl;
        std::cout << "9. Clear Graph" << std::endl;
        std::cout << "10. Menu help" << std::endl;
        std::cout << "0. Exit" << std::endl;


    }

    void add_Location(std::list<location::Location>& locations) {

        location::Location newLocation;
        double vertex;
        std::cout << "x= ";
        std::cin >> vertex;
        newLocation.set_x(vertex);
        std::cout << "y= ";
        std::cin >> vertex;
        newLocation.set_y(vertex);
        std::cout << "z= ";
        std::cin >> vertex;
        newLocation.set_z(vertex);

        std::string name;
        std::cout << "Name: ";
        std::cin >> name;
        newLocation.set_name(name);

        locations.push_back(newLocation);

    }

    void add_Connection(std::list<location::Location>& locations) {

        connection::Connection newConnection;

        int startId, endId;
        bool sameEnd = false;
        do {
            std::cout << "Start:";
            std::cin >> startId;
            std::cout << "End:";
            std::cin >> endId;
        } while (startId >= locations.size() || endId >= locations.size() || startId == endId);

        newConnection.set_startId(startId);
        newConnection.set_endId(endId);

        location::Location start;
        location::Location end;

        int br = 0;
        for (auto& location : locations) {
            if (location.get_id() == startId) {
                start = location;
                br++;
            }
            if (location.get_id() == endId) {
                for (auto& connection : location.get_list()) {
                    if (connection.get_endId() == endId) {
                        std::cout << "This connection already exists!" << std::endl;
                        return;
                    }
                }
                end = location;
                br++;
            }

        }
        if (br == 2)
        {
            double xStart = start.get_x(), yStart = start.get_y(), zStart = start.get_z();
            double xEnd = end.get_x(), yEnd = end.get_y(), zEnd = end.get_z();

            double distance = sqrt(pow((xEnd - xStart), 2) + pow((yEnd - yStart), 2) + pow((zEnd - zStart), 2));
            std::cout << "Distance: " << distance << std::endl;
            newConnection.set_distance(distance);

            for (auto& location : locations) {
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
        else std::cout << "Can't find id" << std::endl;



    }

    void display(std::list<location::Location>& locations) {
        for (auto& location : locations) {
            std::cout << "ID: " << location.get_id() << std::endl;
            std::cout << "X: " << location.get_x() << std::endl;
            std::cout << "Y: " << location.get_y() << std::endl;
            std::cout << "Z: " << location.get_z() << std::endl;
            std::cout << "Name: " << location.get_name() << std::endl;

            std::list<connection::Connection> connections = location.get_list();
            std::cout << "Connections:" << std::endl;
            for (auto& connection : connections) {
                std::cout << "StartID: " << connection.get_startId() << std::endl;
                std::cout << "EndID: " << connection.get_endId() << std::endl;
                std::cout << "Distance: " << connection.get_distance() << std::endl;
            }


        }
        std::cout << std::endl;
    }




}
