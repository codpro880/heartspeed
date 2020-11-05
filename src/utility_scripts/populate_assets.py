import json
import os
import shutil

import requests

THIS_DIR = os.path.dirname(__file__)

def dump_bg_cards_to_assets(directory):
    if not os.path.exists(directory):
        os.mkdir(directory);
    with open(os.path.join(THIS_DIR, 'cards.json'), 'r') as f:
        card_json_str = f.read()
    for card in card_json_str.split('{')[1:]:
        if "battlegroundsPremiumDbfId" in card:
            id_ = card.split('"id":')[-1].split(',')[0]
            id_ = id_[2:-1]
            name = card.split('"name":')[-1].split(',')[0]
            name = name[2:-1]
            url = f'https://art.hearthstonejson.com/v1/256x/{id_}.jpg'
            res = requests.get(url, stream=True)
            with open(os.path.join(directory, name + '.jpg'), 'wb') as of:
                res.raw.decode_content = True
                shutil.copyfileobj(res.raw, of)

            

def main():
    dump_bg_cards_to_assets(directory=os.path.join(THIS_DIR, 'assets'))

if __name__ == '__main__':
    main()
