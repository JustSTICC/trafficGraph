#include "TrafficRule.h"


namespace traffic_rules {

    TrafficRule::TrafficRule(double begin_at, double valid_distance, int type_id, double value) {

        this->begin_at = begin_at;
        this->valid_distance = valid_distance;
        this->type_id = type_id;
        this->value = value;

    }
    TrafficRule::TrafficRule(double begin_at, double valid_distance, int type_id) {

        this->begin_at = begin_at;
        this->valid_distance = valid_distance;
        this->type_id = type_id;

    }
    TrafficRule::~TrafficRule() {}

    double TrafficRule::get_beginAt() {
        return begin_at;
    }

    double TrafficRule::get_validDistance() {
        return valid_distance;
    }

    int TrafficRule::get_typeId() {
        return type_id;
    }

    double TrafficRule::get_value() {
        return value;
    }

    
    void TrafficRule::set_beginAt(double begin_at) {
        this->begin_at = begin_at;
    }

    void TrafficRule::set_validDistance(double valid_distance) {
        this->valid_distance = valid_distance;
    }

    void TrafficRule::set_typeId(int type_id) {
        this->type_id = type_id;
    }

    void TrafficRule::set_value(double value) {
        this->value = value;
    }
}