#include <cstdlib>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <utility>

#include "bobs_buddy.hpp"

std::vector<std::pair<std::shared_ptr<Board>, std::shared_ptr<Board>>> BobsBuddy::parse_full_log() {
    std::vector<std::string> file_contents = get_file_contents();
    std::cerr << "CONTENTS" << std::endl;
    std::vector<std::vector<std::string>> chunks = get_chunks(file_contents);
    std::cerr << "CHUNKS" << std::endl;
    std::vector<std::pair<std::shared_ptr<Board>, std::shared_ptr<Board>>> res;
    std::cerr << "DECLARE RES" << std::endl;
    auto count = 0;
    for (auto chunk : chunks) {
	std::cerr << "LOOP" << std::endl;
	std::pair<std::shared_ptr<Board>, std::shared_ptr<Board>> pair = parse_chunk(chunk);
	std::cerr << "ADD PAIR" << std::endl;
	res.push_back(pair);
	count++;
	if (count == 2) break;
    }
    std::cerr << "RETURN" << std::endl;
    return res;
}

std::vector<std::string> BobsBuddy::get_file_contents() {
    std::string line;
    std::ifstream myfile (_power_log);
    std::vector<std::string> lines(100);
    if (myfile.is_open())
	{
	    while ( getline (myfile,line) )
		{
		    lines.push_back(line);
		}
	    myfile.close();
	}
    else {
	throw std::runtime_error("Unable to open power log at: '" + _power_log + "'");
    }
    return lines;
}

std::vector<std::vector<std::string>> BobsBuddy::get_chunks(std::vector<std::string> file_contents) {
    int line_count = 0;
    std::vector<int> starts;
    std::vector<int> real_starts;
    std::vector<int> ends;
    for (auto line : file_contents) {
	if (line.find("BLOCK_START") != std::string::npos) {
	    starts.push_back(line_count);
	}
	else if (line.find("BLOCK_END") != std::string::npos) {
	    if (starts.size() > 0) {
		real_starts.push_back(starts.back());
		ends.push_back(line_count);
	    }
	}
	line_count++;
    }

    std::vector<std::vector<std::string>> possible_chunks;
    for (int i = 0; i < ends.size(); i++) {
	std::vector<std::string> chunk(file_contents.begin() + real_starts[i],
				       file_contents.begin() + ends[i]);
	possible_chunks.push_back(chunk);
    }

    std::cerr << "Possible chunks size: " << possible_chunks.size() << std::endl;
    
    std::vector<std::vector<std::string>> chunks;
    for (auto possible_chunk : possible_chunks) {
	for (auto line : possible_chunk) {
	    if (line.find("FULL_ENTITY - Updating [entityName=") != std::string::npos) {
		auto card_name = pystr.get_str_between(line, "[entityName=", " id=");
		try {
		    card_factory.get_card(card_name);
		    chunks.push_back(possible_chunk);
		    break;
		}
		catch (...) {
		}
	    }
	}
    }

    // Filter out chunks w/ SUB_SPELL_START in them
    std::vector<std::vector<std::string>> wo_subspell_chunks;
    auto pred = [](std::vector<std::string> chunk) {
		    for (auto line : chunk) {
			if (line.find("SUB_SPELL_START") != std::string::npos) {
			    return false;
			}
			else if (line.find("SUB_SPELL_END") != std::string::npos) {
			    return false;
			}
		    }
		    return true;
		};
    std::copy_if(chunks.begin(), chunks.end(), std::back_inserter(wo_subspell_chunks), pred);

    // Filter out blockType that aren't TRIGGER
    std::vector<std::vector<std::string>> trigger_blocks;
    auto pred_trig = [](std::vector<std::string> chunk) {
			 auto line = chunk[0];
			 return line.find("BLOCK_START BlockType=TRIGGER") != std::string::npos;
		};
    std::copy_if(wo_subspell_chunks.begin(), wo_subspell_chunks.end(), std::back_inserter(trigger_blocks), pred_trig);
    
    std::cerr << "Returning chunks..." << std::endl;
    std::cerr << "Chunks size: " << trigger_blocks.size() << std::endl;
    return trigger_blocks;
}

std::pair<std::shared_ptr<Board>, std::shared_ptr<Board>> BobsBuddy::parse_chunk(std::vector<std::string> chunk) {
    std::cerr << "Chunk: " << std::endl;
    std::map<int, std::shared_ptr<BgBaseCard>> our_id_to_card;
    std::map<int, std::shared_ptr<BgBaseCard>> their_id_to_card;

    auto our_zone_pos = 0;
    auto their_zone_pos = 0;
    for (auto line : chunk) {
	std::cerr << "ChunkLine: " << line << std::endl;
	if (line.find("FULL_ENTITY - Updating") != std::string::npos) {
	    auto card_name = pystr.get_str_between(line, "entityName=", " id=");
	    std::shared_ptr<BgBaseCard> card;
	    try {
		card = card_factory.get_card(card_name);
	    }
	    catch (...) {
		continue;
	    }
	    // auto zone_pos = atoi(pystr.get_str_between(line, "zonePos=", " cardId").c_str());
	    auto player = pystr.get_str_between(line, "player=", "]");
	    if (player == "8") {
		our_id_to_card[++our_zone_pos] = card;
		std::cerr << "Added " << card->get_name() << " to ours with ID " << our_zone_pos << std::endl;
	    }
	    else {
		their_id_to_card[++their_zone_pos] = card;
		std::cerr << "Added " << card->get_name() << " to theirs with ID " << their_zone_pos << std::endl;
	    }
	}
    }

    std::cerr << "Original ids assigned" << std::endl;

    // IDs switch for some reason? For now just re-arrange till
    // we figure out what the hell is going on...
    // int last_copied_ent_line = -1;
    // for (size_t line_num = 0; line_num < chunk.size(); line_num++) {
    // 	auto line = chunk[line_num];
    // 	if (line.find("TAG_CHANGE") != std::string::npos) {
    // 	    auto tag = pystr.get_str_between(line, "tag=", " value");
    // 	    if (tag == "COPIED_FROM_ENTITY_ID") {
    // 		last_copied_ent_line = line_num;
    // 	    }
    // 	}
    // }

    // std::cerr << "Last copied line" << std::endl;

    // auto change_line = chunk[last_copied_ent_line];
    // auto new_id = atoi(pystr.get_str_between(change_line, "value=", " ").c_str());
    // auto old_id = atoi(pystr.get_str_between(change_line, "id=", " zone").c_str());
    // if (our_id_to_card.count(old_id) > 0) {
    // 	auto nodeHandler = our_id_to_card.extract(old_id);
    // 	nodeHandler.key() = new_id;
    // 	our_id_to_card.insert(std::move(nodeHandler));
    // }
    // else {
    // 	auto nodeHandler = their_id_to_card.extract(old_id);
    // 	nodeHandler.key() = new_id;
    // 	their_id_to_card.insert(std::move(nodeHandler));
    // }

    std::cerr << "Reassigned..." << std::endl;
    std::cerr << "OUR SIZE: " << our_id_to_card.size() << std::endl;
    std::cerr << "THEIR SIZE: " << our_id_to_card.size() << std::endl;

    // Update based on stat buffs
    for (auto line : chunk) {
	if (line.find("TAG_CHANGE") != std::string::npos) {
	    auto tag = pystr.get_str_between(line, "tag=", " value");
	    if (tag == "HEALTH") {
		auto zone_pos = atoi(pystr.get_str_between(line, "zonePos=", " cardId").c_str());
		auto health = atoi(pystr.get_str_between(line, "value=", " ").c_str());
		auto player = pystr.get_str_between(line, "player=", "]");
		if (player == "8") {
		    our_id_to_card[zone_pos]->set_health(health);
		}
		else {
		    their_id_to_card[zone_pos]->set_health(health);
		}
	    }
	    else if (tag == "ATK") {
		auto zone_pos = atoi(pystr.get_str_between(line, "zonePos=", " cardId").c_str());
		auto player = pystr.get_str_between(line, "player=", "]");
		auto attack = atoi(pystr.get_str_between(line, "value=", " ").c_str());
		// std::cerr << "OurIdToCard size: " << our_id_to_card.size() << std::endl;
		// for (auto item : our_id_to_card) {
		//     std::cerr << item.first << std::endl;
		// }
		if (player == "8") {
		    std::cerr << "OUR ID: " << zone_pos << std::endl;
		    our_id_to_card[zone_pos]->set_attack(attack);
		    std::cerr << "After our attack set..."  << std::endl;
		}
		else {
		    std::cerr << "THEIR ID: " << zone_pos << std::endl;
		    // std::cerr << "TheirIdToCard size: " << their_id_to_card.size() << std::endl;
		    // for (auto item : their_id_to_card) {
		    // 	std::cerr << item.first << std::endl;
		    // }
		    their_id_to_card[zone_pos]->set_attack(attack);
		    std::cerr << "After their attack set..."  << std::endl;
		}
	    }
	}
    }

    std::vector<std::shared_ptr<BgBaseCard>> our_cards;
    for (auto kv : our_id_to_card) {
	std::cerr << "FINAL ADD OURS " << kv.second->get_name() << std::endl;
	our_cards.push_back(kv.second);
    }

    std::vector<std::shared_ptr<BgBaseCard>> their_cards;
    for (auto kv : their_id_to_card) {
	std::cerr << "FINAL ADD THEIRS " << kv.second->get_name() << std::endl;
	their_cards.push_back(kv.second);
    }

    auto our_board = std::make_shared<Board>(our_cards);
    auto their_board = std::make_shared<Board>(their_cards);

    auto pair = std::make_pair(our_board, their_board);
    return pair;
    // std::vector<std::pair<std::shared_ptr<Board>, std::shared_ptr<Board>>> res;
    // res.push_back(pair);
    // return res;
}
