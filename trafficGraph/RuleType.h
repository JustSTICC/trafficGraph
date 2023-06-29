#pragma once

#include<string>

namespace ruletype {

    class RuleType {

        static int CurrentId;
        int id;
        std::string name;

    public:

        RuleType();
        RuleType( std::string name);
        ~RuleType();

        int get_id();
        std::string get_name();

        void set_name(std::string);


    };





}