#include <iostream>
#include <fstream>
#include <string>

#include "bobs_buddy.hpp"

std::vector<std::pair<std::shared_ptr<Board>, std::shared_ptr<Board>>> BobsBuddy::parse_full_log() {
    std::string line;
    std::ifstream myfile (_power_log);
    if (myfile.is_open())
	{
	    while ( getline (myfile,line) )
		{
		    std::cout << line << '\n';
		}
	    myfile.close();
	}

    else std::cout << "Unable to open file";
    std::vector<std::pair<std::shared_ptr<Board>, std::shared_ptr<Board>>> res;
    return res;
}
