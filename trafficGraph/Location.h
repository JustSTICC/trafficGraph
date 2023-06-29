#pragma once
#include"Connection.h"

#include<string>
#include<list>



namespace location {

    class Location {

    private:

        static int CurrentId;
        int id;
        double x;
        double y;
        double z;
        std::string name;
        std::list<connection::Connection> connections;

    public:

        

        Location();
        Location(double x, double y, double z, std::string name);
        ~Location();

        int get_id();
        double get_x();
        double get_y();
        double get_z();
        std::string get_name();
        std::list<connection::Connection> get_list();

        void set_x(double x);
        void set_y(double y);
        void set_z(double z);
        void set_name(std::string name);

        void set_connection(connection::Connection connection);

    };


}