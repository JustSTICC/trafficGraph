#pragma once
#include<list>
#include<fstream>
#include<iostream>



#include "RuleType.h"

namespace write_rule {

	void write(std::list<ruletype::RuleType>& rules) 
	{
		
		std::ofstream file;
		file.open("Rules.txt");

		if (!file) {
			std::cerr << "Failed to open 'location.txt' for writing." << std::endl;
			return;
		}

		for (auto& rule : rules) {
			file << "ID: " << rule.get_id() << "\n";
			file << "Name: " << rule.get_name() << "\n";
		}
		file << "\n";

		file.close();
	}
		

}