#pragma once

#include<fstream>
#include<list>

#include "Location.h"
#include "Connection.h"
#include "TrafficRule.h"


namespace write {

    void write_locations(std::list<location::Location>& wrt_locations) {

        std::ofstream file;
        file.open("location.txt");

        if (!file) {
            std::cerr << "Failed to open 'location.txt' for writing." << std::endl;
            return;
        }

        for (auto& location : wrt_locations) {
            file << "ID: " << location.get_id() << "\n";
            file << "X: " << location.get_x() << "\n";
            file << "Y: " << location.get_y() << "\n";
            file << "Z: " << location.get_z() << "\n";
            file << "Name: " << location.get_name() << "\n";

            file << "Connections:\n";

            const auto& connections = location.get_list();
            for (connection::Connection connection : connections) {
                file << "StartID:" << connection.get_startId() << "\n";
                file << "EndID:" << connection.get_endId() << "\n";
                file << "Distance:" << connection.get_distance() << "\n";
               

                const auto& trafficRules = connection.get_trafficRules();
                if (!trafficRules.empty())file << "Traffic Rules:\n";

                for (traffic_rules::TrafficRule rules : trafficRules) {
                    file << "Begin at:" << rules.get_beginAt() << "\n";
                    file << "Valid distance:" << rules.get_validDistance()<< "\n";
                    file << "TypeID:" << rules.get_typeId() << "\n";
                    file << "Value:" << rules.get_value() << "\n";

                }

            }
            file << "\n"; 
        }
        file.close();






    }

    void write_rules() {

    }

}