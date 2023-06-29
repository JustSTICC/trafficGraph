#include"Connection.h"


namespace connection {

    Connection::Connection() {}
    Connection::Connection(int startId, int endId) {
        
        this->startId = startId;
        this->endId = endId;

    }

    Connection::Connection(int startId, int endId, double distance) {

        this->startId = startId;
        this->endId = endId;
        this->distance = distance;

    }

    Connection::~Connection() {}

    int Connection::get_startId() {
        return startId;
    }

    int Connection::get_endId() {
        return endId;
    }

    double Connection::get_distance() {
        return distance;
    }

    std::list<traffic_rules::TrafficRule> Connection::get_trafficRules() {
        return trafficRules;
    }

    void Connection::set_startId(int startId) {

        this->startId = startId;
    }

    void Connection::set_endId(int endId) {

        this->endId = endId;
    }

    void Connection::set_trafficRules(traffic_rules::TrafficRule trafficRule) {
        trafficRules.push_back(trafficRule);
    }

    void Connection::set_distance(double distance) {

        this->distance = distance;

        }
            
    }


