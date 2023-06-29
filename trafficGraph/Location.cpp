
#include "Location.h"
#include "Connection.h"



namespace location {

    


    int Location::CurrentId = 1;

    Location::Location() {}

    Location::Location(double x, double y, double z, std::string name)
    {
        id = CurrentId++;
        this->x = x;
        this->y = y;
        this->z = z;
        this->name = name;


    }

    Location::~Location() {}

    int Location::get_id() {
        return id;
    }


    double Location::get_x() {

        return x;

    }

    double Location::get_y() {

        return y;

    }

    double Location::get_z() {

        return z;

    }

    std::string Location::get_name() {

        return name;

    }

    std::list<connection::Connection> Location::get_list() {

        return connections;
    }

    void Location::set_x(double x) {

        this->x = x;

    }

    void Location::set_y(double y) {

        this->y = y;

    }

    void Location::set_z(double z) {

        this->z = z;

    }

    void Location::set_name(std::string name) {

        this->name = name;

    }

    void Location::set_connection(connection::Connection connection) {

        connections.push_back(connection);

    }


}
