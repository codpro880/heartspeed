import json
import os
import shutil

import requests

THIS_DIR = os.path.dirname(__file__)

def dump_bg_cards_to_assets(directory):
    print("Populating assets directory with most recent bgs jpg assets...this may take a few minutes.")
    if not os.path.exists(directory):
        os.mkdir(directory);
    card_json_str = get_latest_card_json()
    dump_cards(directory, card_json_str)

def get_latest_card_json():
    url = 'https://api.hearthstonejson.com/v1/latest/enUS/cards.json'
    res = requests.get(url)    
    return res.json()

def dump_cards(directory, card_json):
    bgs_cards = [card for card in card_json if card['set'] == "BATTLEGROUNDS"]
    for card in bgs_cards:
        id_ = card['id']
        name = card['name'].replace("'", "").replace("!", "")        
        url = f'https://art.hearthstonejson.com/v1/256x/{id_}.jpg'
        res = requests.get(url, stream=True)
        with open(os.path.join(directory, name + '.jpg'), 'wb') as of:
            res.raw.decode_content = True
            shutil.copyfileobj(res.raw, of)

def main():
    dump_bg_cards_to_assets(directory=os.path.join(THIS_DIR, 'assets'))

if __name__ == '__main__':
    main()
