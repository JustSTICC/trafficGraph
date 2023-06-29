

#include"ConsoleControl.h"
#include "Generator.h"
#include "Location.h"
#include "Connection.h"
#include "TrafficRule.h"
#include "RuleType.h"
#include "Write.h"
#include "writeRules.h"
#include "Read.h"
#include "PointToPoint.h"
#include "app.h"

#include"config.h"



int main() {


    std::list<location::Location> graph;
    std::list<ruletype::RuleType> ruleTypes;
    std::list<location::Location> route, route1;

    App* myApp = new App(640, 480, true);

    myApp->run();

    delete myApp;

    location::Location start, end;

    ruletype::RuleType newRule {"SpeedLimit"};

    ruleTypes.push_back(newRule);

    write_rule::write(ruleTypes);

    int locationNum, startId, endId;
    
    int input=1;

    console::menu();
    
    while (input != 0) {
        std::cout << ":>";
        std::cin >> input;
        switch (input)
        {
        case 1: console::add_Location(graph); break;
        case 2: console::add_Connection(graph); break;
        case 3: console::display(graph); break;
        case 4: graph = read::read_location(); std::cout << "Save loaded!\n"; break;
        case 5: write::write_locations(graph); break;
        case 6: {
            std::cout << "Locations num:\n";
            std::cin >> locationNum;
            build::generateLocations(graph, locationNum);
            break;
        }
        case 7: {do {
            std::cout << "Enter startID:";
            std::cin >> startId;
            std::cout << "Enter endID:";
            std::cin >> endId;
        } while (startId > graph.size() || endId > graph.size());
        {

        }
            for (location::Location location : graph) {

            if (location.get_id() == startId)  start = location;
            if (location.get_id() == endId)  end = location;
        }
              route = dijkstra(graph, start, end);
              if (!route.empty()) {
                  std::cout << "Path: ";
                  for (location::Location location : route) {
                      std::cout << location.get_id() << " ";
                  }
                  std::cout << std::endl;
              }
              else std::cout << "No path found!\n";
              break;   
        }
        case 8: {do {
            std::cout << "Enter startID:";
            std::cin >> startId;
            std::cout << "Enter endID:";
            std::cin >> endId;
        } while (startId > graph.size() || endId > graph.size());
            for (location::Location location : graph) {

                if (location.get_id() == startId)  start = location;
                if (location.get_id() == endId)  end = location;
            }
            std::cout << "------------------\n";
            route1 = bestRoute(graph, start, end);
            if (!route1.empty()) {
                std::cout << "Path: ";
                for (location::Location location : route1) {
                    std::cout << location.get_id() << " ";
                }
                std::cout << std::endl;
            }
            else std::cout << "No path found!\n";
            break;
        }
        case 9:graph.clear(); break;
        case 10:console::menu(); break;

        default:
            break;
        }
    }

    return 0;
}
