#pragma once

namespace traffic_rules {

    class TrafficRule {

        double begin_at;
        double valid_distance;
        int type_id;
        double value;

    public:

        TrafficRule(double begin_at, double valid_distance, int type_id, double value);
        TrafficRule(double begin_at, double valid_distance, int type_id);
        ~TrafficRule();

        double get_beginAt();
        double get_validDistance();
        int get_typeId();
        double get_value();

        void set_beginAt(double);
        void set_validDistance(double);
        void set_typeId(int);
        void set_value(double);

    };


}