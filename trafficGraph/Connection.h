#pragma once
#include"TrafficRule.h"
#include<list>


namespace connection {

    class Connection {

        int startId;
        int endId;
        double distance;
        std::list<traffic_rules::TrafficRule> trafficRules;

    public:

        Connection();
        Connection(int start, int end);
        Connection(int start, int end, double distance);
        ~Connection();

        int get_startId();
        int  get_endId();
        double get_distance();
        std::list<traffic_rules::TrafficRule> get_trafficRules();

        void set_trafficRules(traffic_rules::TrafficRule);
        void set_startId(int startId);
        void  set_endId( int endId);
        void set_distance(double distance);



    };


}