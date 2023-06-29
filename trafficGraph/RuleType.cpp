#include "RuleType.h"
#include<string>

namespace ruletype {

    int RuleType::CurrentId = 1;


    RuleType::RuleType(){}
    RuleType::RuleType( std::string name) {

        this->id = CurrentId++;
        this->name = name;
    }

    RuleType::~RuleType() {}


    int RuleType::get_id() {
        return id;
    }

    std::string RuleType::get_name() {
        return name;
    }

    void RuleType::set_name(std::string name) {
        this->name = name;
    }
}