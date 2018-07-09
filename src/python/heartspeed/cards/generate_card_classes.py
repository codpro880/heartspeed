import json
import os
import requests

class JsonCardGenerator:
    """ Responsible for generating C++ classes based on card json """
    def __init__(self):
        self.card_api = "https://api.hearthstonejson.com/v1/latest/enUS/cards.json"
        this_dir = os.path.dirname(os.path.realpath(__file__))
        self.card_json_filename = os.path.join(this_dir, "cards.json")

    def get_cards_from_web(self, write_to_disk=False):
        """ Rope in the live data, it is updated occassionally """
        get_req = requests.get(self.card_api)
        card_json = json.loads(get_req.text)
        if write_to_disk:
            with open(self.card_json_filename, 'w') as f:
                f.write(get_req.text)
        yield from card_json

    def get_cards_from_disk(self):
        """ Make sure we have some stable set we can draw from """
        with open(self.card_json_filename, 'rb') as f:
            card_json = json.loads(f.read())
        yield from card_json
        
#     def generate(self):
#         card_list = self.get_cards_from_web()
#         for card in card_list:
#             self._generate_class_file(card)

class CardClassGenerator:
    """ Responsible for generate a single C++ class based on the card json """
    def __init__(self, card_json):
        self.card_json = card_json

    def generate_class_file(self, cards_cpp_dir=None):
        if cards_cpp_dir is None:
            this_dir = os.path.dirname(os.path.realpath(__file__))
            cards_cpp_dir = os.path.join(this_dir, "..", "..", "..", "cpp", "cards")
        name = self._get_name()
        cpp_file = os.path.join(cards_cpp_dir, name + ".cpp")
        hpp_file = os.path.join(cards_cpp_dir, name + ".hpp")
        with open(cpp_file, 'w') as f:
            cpp_str = self._gen_cpp_string()
            f.write(cpp_str)
        with open(hpp_file, 'w') as f:
            hpp_str = self._gen_hpp_string()
            f.write(hpp_str)

    def _get_name(self):
        return self.card_json["name"].replace(" ", "")

    def _gen_hpp_string(self):
        template = """#include "BaseCard.hpp"

class {0} : public BaseCard {{
public:
    {0}();
    std::string get_name();
    int get_cost();
    std::string get_id();
    std::string get_text();
private:
    std::string name;
    int cost;
    std::string id;
    std::string text;
}};"""

        return template.format(self._get_name())

    def _gen_cpp_string(self):
        template = """#include "{0}.hpp"

{0}::{0}() : name("{0}"),
			   cost({1}),
			   id("{2}"),
			   text("{3}") {{}}

std::string {0}::get_name() {{ return name; }}
int {0}::get_cost() {{ return cost; }}
std::string {0}::get_id() {{ return id; }}
std::string {0}::get_text() {{ return text; }}"""

        cost = self.card_json["cost"]
        id = self.card_json["id"]
        text = self.card_json["text"]
        return template.format(self._get_name(), cost, id, text)
