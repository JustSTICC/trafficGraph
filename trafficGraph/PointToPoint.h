#pragma once
#include<queue>
#include<set>
#include<vector>
#include<chrono>
#include<thread>


#define DEBUG false




std::list<location::Location> dijkstra(std::list<location::Location> locations, location::Location start, location::Location end) {


	std::list<location::Location> bestRoute;

	const int numLocations = locations.size();
	std::vector<double> dist(numLocations, std::numeric_limits<double>::infinity());
	std::vector<int> prev(numLocations, -1);
	std::set<int> unvisited;

    

	dist[start.get_id()-1] = 0.0;

	unvisited.insert(start.get_id());

	if (DEBUG) {
		std::cout << "Prev: ";
		for (int i = 0; i < numLocations; ++i) {
			std::cout << prev[i] << " ";
		}
		std::cout << std::endl;
	}

    auto startTime = std::chrono::steady_clock::now();

	while (!unvisited.empty()) {
		
		int currentId = *unvisited.begin();
		unvisited.erase(unvisited.begin());

		for (auto& location : locations) {
			if (location.get_id() == currentId) {
				for (auto& connection : location.get_list()) {
					if (connection.get_startId() == currentId ) {
						int neighborId = connection.get_endId();
						double weight = connection.get_distance();

						if (DEBUG) {
							std::cout << "NID: "<< neighborId <<"\n";
							std::cout << "CID: " << currentId << "\n";
						}

						
						if (dist[currentId-1] + weight < dist[neighborId-1]) {
							
							dist[neighborId-1] = dist[currentId-1] + weight;
							prev[neighborId-1] = currentId;

							
							auto it = unvisited.find(neighborId);
							if (it != unvisited.end())
								unvisited.erase(it);

		
							unvisited.insert(neighborId);
						}
					}
				}
				break;
			}

		}

        //std::this_thread::sleep_for(std::chrono::milliseconds(5));
		 if (DEBUG) {
            std::cout << "Prev: ";
            for (int i = 0; i < numLocations; ++i) {
                std::cout << prev[i] << " ";
            }
            std::cout << std::endl;
        }
		 if (DEBUG) {
			 if (unvisited.empty()) {
				 std::cout << "Empty!" << std::endl;
			 }
		 }
    
	}


	int currentId = end.get_id();
	if (prev[currentId - 1] == -1) {
		return bestRoute;
	}
	while (currentId != -1) {
		for (auto& location : locations) {
			if (location.get_id() == currentId) {
				bestRoute.push_front(location);
				//std::cout << currentId << std::endl;
				currentId = prev[currentId-1];
				break;
			}
		}
	}

    auto endTime = std::chrono::steady_clock::now();
    auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
    std::cout << "Algorithm executed in " << elapsedTime << " milliseconds." << std::endl;

	return bestRoute;

}

std::list<location::Location> bestRoute(std::list<location::Location> locations, location::Location start, location::Location end) {
    std::list<location::Location> bestRoute;
    
    // Initialize distance and previous arrays
    const int numLocations = locations.size();
    std::vector<double> forwardDist(numLocations, std::numeric_limits<double>::infinity());
    std::vector<double> backwardDist(numLocations, std::numeric_limits<double>::infinity());
    std::vector<int> forwardPrev(numLocations, -1);
    std::vector<int> backwardPrev(numLocations, -1);
    std::set<int> forwardUnvisited;
    std::set<int> backwardUnvisited;
    double bestDistance = std::numeric_limits<double>::infinity();

    // Set the distance of the start and end locations to 0
    forwardDist[start.get_id()-1] = 0.0;
    backwardDist[end.get_id()-1] = 0.0;

    // Add the start and end locations to the set of unvisited locations
    forwardUnvisited.insert(start.get_id());
    backwardUnvisited.insert(end.get_id());

    auto startTime = std::chrono::steady_clock::now();

    if (DEBUG) {
        std::cout << "Prev: ";
        for (int i = 0; i < numLocations; ++i) {
            std::cout << forwardPrev[i] << " ";
        }
        std::cout << std::endl;
    }
    if (DEBUG) {
        for (int locId : forwardUnvisited) {
            std::cout << locId << " ";
        }
        std::cout << "\n";
    }
    if (DEBUG) {
        std::cout << "Dist: ";
        for (int i = 0; i < numLocations; ++i) {
            std::cout << forwardDist[i] << " ";
        }
        std::cout << std::endl;
    }
    if (DEBUG) {
        if (forwardUnvisited.empty()) {
            std::cout << "Empty!" << std::endl;
        }
    }
    int counter = 0;
    // Bidirectional Dijkstra's algorithm
    while (!forwardUnvisited.empty() && !backwardUnvisited.empty()) {
        // Forward search: Find the location with the smallest distance among the unvisited forward locations
        int forwardCurrentId = *forwardUnvisited.begin();
        forwardUnvisited.erase(forwardUnvisited.begin());


        for (auto& location : locations) {
            if (location.get_id() == forwardCurrentId) {
                for (auto& connection : location.get_list()) {
                    if (connection.get_startId() == forwardCurrentId) {
                        int neighborId = connection.get_endId();
                        double weight = connection.get_distance();
                        
                        if (DEBUG)std::cout<< "Neighbor ID: " << neighborId << "\n";

                        // Relax the edge (forwardCurrentId, neighborId)
                        if (forwardDist[forwardCurrentId - 1] + weight < forwardDist[neighborId - 1]) {
                            forwardDist[neighborId - 1] = forwardDist[forwardCurrentId - 1] + weight;
                            forwardPrev[neighborId - 1] = forwardCurrentId;

                            // Remove the previous occurrence of neighborId from forwardUnvisited
                            auto it = forwardUnvisited.find(neighborId);
                            if (it != forwardUnvisited.end())
                                forwardUnvisited.erase(it);

                            // Add neighborId to forwardUnvisited to be processed later
                            forwardUnvisited.insert(neighborId);
                        }
                    }
                }
                break;
            }
        }
        if (DEBUG) {
            std::cout << "Prev: ";
            for (int i = 0; i < numLocations; ++i) {
                std::cout << forwardPrev[i] << " ";
            }
            std::cout << std::endl;
        }
        if (DEBUG) {
            for (int locId : forwardUnvisited) {
                std::cout << locId << " ";
            }
            std::cout << "\n";
        }
        if (DEBUG) {
            std::cout << "Dist: ";
            for (int i = 0; i < numLocations; ++i) {
                std::cout << forwardDist[i] << " ";
            }
            std::cout << std::endl;
        }
        if (DEBUG) {
            if (forwardUnvisited.empty()) {
                std::cout << "Empty!" << std::endl;
            }
        }

        // Backward search: Find the location with the smallest distance among the unvisited backward locations
        int backwardCurrentId = *backwardUnvisited.begin();
        backwardUnvisited.erase(backwardUnvisited.begin());

        for (auto& location : locations) {
            if (location.get_id() == backwardCurrentId) {
                for (auto& connection : location.get_list()) {
                    if (connection.get_endId() == backwardCurrentId) {
                        int neighborId = connection.get_startId();
                        double weight = connection.get_distance();

                        // Relax the edge (neighborId, backwardCurrentId)
                        if (backwardDist[backwardCurrentId - 1] + weight < backwardDist[neighborId - 1]) {
                            backwardDist[neighborId - 1] = backwardDist[backwardCurrentId - 1] + weight;
                            backwardPrev[neighborId - 1] = backwardCurrentId;

                            // Remove the previous occurrence of neighborId from backwardUnvisited
                            auto it = backwardUnvisited.find(neighborId);
                            if (it != backwardUnvisited.end())
                                backwardUnvisited.erase(it);

                            // Add neighborId to backwardUnvisited to be processed later
                            backwardUnvisited.insert(neighborId);
                        }
                    }
                }
                break;
            }
        }
        if (DEBUG) {
            std::cout << "Prev: ";
            for (int i = 0; i < numLocations; ++i) {
                std::cout << backwardPrev[i] << " ";
            }
            std::cout << std::endl;
        }
        if (DEBUG) {
            std::cout << "Dist: ";
            for (int i = 0; i < numLocations; ++i) {
                std::cout << backwardDist[i] << " ";
            }
            std::cout << std::endl;
        }
        if (DEBUG) {
            if (backwardUnvisited.empty()) {
                std::cout << "Empty!" << std::endl;
            }
        }
        if (DEBUG) {
            for (int locId : backwardUnvisited) {
                std::cout << locId << " ";
            }
            std::cout << "\n";
        }
        //std::this_thread::sleep_for(std::chrono::milliseconds(5));
        // Check for meeting point
        for (int locId : forwardUnvisited) {
            if (DEBUG) std::cout << backwardDist[locId-1] << "\n";
            if (backwardDist[locId-1] < std::numeric_limits<double>::infinity()) {
                // Meeting point found
                double totalDist = forwardDist[locId-1] + backwardDist[locId-1];
                if (totalDist < bestDistance) {
                    counter++;
                    bestDistance = totalDist;

                    bestRoute.clear();
                    // Debugging output
                    if (DEBUG) {
                        std::cout << "Meeting point: " << locId << std::endl;
                        std::cout << "Total distance: " << totalDist << std::endl;
                    }

                    // Reconstruct the best route from start to the meeting point
                    int currentId = locId;
                    while (currentId != -1) {
                        for (auto& location : locations) {
                            if (location.get_id() == currentId) {
                                if (DEBUG) std::cout << location.get_id() << "\n";
                                bestRoute.push_front(location);
                                currentId = forwardPrev[currentId - 1];
                                break;
                            }
                        }
                    }

                    // Reconstruct the best route from the meeting point to end
                    currentId = locId;
                    while (currentId != -1) {
                        for (auto& location : locations) {
                            if (location.get_id() == currentId) {
                                if (DEBUG) std::cout << location.get_id() << "\n";
                                if (currentId != locId) {
                                    bestRoute.push_back(location);
                                }
                                currentId = backwardPrev[currentId - 1];
                                break;
                            }
                        }
                    }
                }
                if (counter == 3) {
                    auto endTime = std::chrono::steady_clock::now();
                    auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
                    std::cout << "Algorithm executed in " << elapsedTime << " milliseconds." << std::endl;
                    return bestRoute;
                }
                

            }
        }
        /*double minTotalDist = std::numeric_limits<double>::infinity();
        int forwardId = -1;
        int backwardId = -1;

        for (int forwardLocId : forwardUnvisited) {
            for (int backwardLocId : backwardUnvisited) {
                double totalDist = forwardDist[forwardLocId] + backwardDist[backwardLocId];
                if (totalDist < minTotalDist) {
                    minTotalDist = totalDist;
                    forwardId = forwardLocId;
                    backwardId = backwardLocId;
                }
            }
        }
        if (minTotalDist >= std::numeric_limits<double>::infinity())
            break;*/

        
    }
    auto endTime = std::chrono::steady_clock::now();
    auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
    std::cout << "Algorithm executed in " << elapsedTime << " milliseconds." << std::endl;
    
    // No meeting point found
    return bestRoute;
}