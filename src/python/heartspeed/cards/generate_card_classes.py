import json
import requests

class JsonCardGenerator:
    """ Responsible for generating C++ classes based on card json """
    def __init__(self):
        self.card_api = "https://api.hearthstonejson.com/v1/latest/enUS/cards.json"

    def get_cards_from_web(self):
        get_req = requests.get(self.card_api)
        json_dict = json.loads(get_req.text)
        yield from json_dict
        
#     def generate(self):
#         card_list = self.get_cards_from_web()
#         for card in card_list:
#             self._generate_class_file(card)

class CardClassGenerator:
    """ Responsible for generate a single C++ class based on the card json """
    def __init__(self, card_json):
        self.card_json = card_json

    def generate_class_file(self):
        class_str = self._generate_class_str()
        with open('', 'w') as f:
            f.write(class_str)
