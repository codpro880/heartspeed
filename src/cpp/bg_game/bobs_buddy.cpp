#include <iostream>
#include <fstream>
#include <string>

#include "bobs_buddy.hpp"

std::vector<std::pair<std::shared_ptr<Board>, std::shared_ptr<Board>>> BobsBuddy::parse_full_log() {
    std::string line;
    std::ifstream myfile (_power_log);
    if (myfile.is_open())
	{
	    std::vector<std::string> lines(100);
	    while ( getline (myfile,line) )
		{
		    lines.push_back(line);
		}
	    myfile.close();
	}
    else {
	std::cout << "Unable to open file: " << _power_log << std::endl;;
    }
    std::vector<std::shared_ptr<BgBaseCard> > our_cards { };
    auto our_board = std::make_shared<Board>(our_cards);
    std::vector<std::shared_ptr<BgBaseCard> > their_cards { };
    auto their_board = std::make_shared<Board>(their_cards);

    auto pair = std::make_pair(our_board, their_board);
    std::vector<std::pair<std::shared_ptr<Board>, std::shared_ptr<Board>>> res;
    res.push_back(pair);
    return res;
}
