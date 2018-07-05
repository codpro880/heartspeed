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

# class CardClassGenerator:
#     """ Responsible for generate a single C++ class based on the card json """
#     def __init__(self, card_json):
#         self.card_json = card_json
#         self.class_str = self._generate_class_str()

#     def generate_class_file(self, filename=None):
#         if filename is None:
#             filename = os.path.join(
#         with open('', 'w') as f:
#             f.write(self.class_str)
