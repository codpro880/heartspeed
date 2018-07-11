import json
import os

from heartspeed.cards.generate_card_classes import CardFactoryGenerator

def test_generate_card_class():
    json_str = """[{"artist":"Nutthapon Petchthai","cardClass":"MAGE","collectible":true,"cost":5,"dbfId":2539,"flavor":"It's on the rack next to ice lance, acid lance, and English muffin lance.","id":"AT_001","name":"Flame Lance","playRequirements":{"REQ_MINION_TARGET":0,"REQ_TARGET_TO_PLAY":0},"rarity":"COMMON","set":"TGT","text":"Deal $8 damage to a minion.","type":"SPELL"},{"artist":"Tooth","cardClass":"MAGE","collectible":true,"cost":3,"dbfId":2541,"flavor":"Burning man, brah.","id":"AT_002","mechanics":["SECRET"],"name":"Effigy","rarity":"RARE","set":"TGT","text":"<b>Secret:</b> When a friendly minion dies, summon a random minion with the same Cost.","type":"SPELL"}]"""
    card_json = json.loads(json_str)
    gen = CardFactoryGenerator(card_json)

    this_dir = os.path.dirname(os.path.realpath(__file__))
    gen.generate_class_file(cards_cpp_dir=this_dir)

    expected_cpp_fname = os.path.join(this_dir, "CardFactory.cpp")

    assert os.path.exists(expected_cpp_fname)

    # Do some spot checking...
    with open(expected_cpp_fname, 'r') as f:
        text = f.read()
        assert 'if (name == "FlameLance")' in text
        assert """BaseCard("FlameLance", 5, "AT_001", "Deal $8 damage to a minion.")""" in text
        assert 'else if (name == "Effigy")' in text
        assert """BaseCard("Effigy", 3, "AT_002", "<b>Secret:</b> When a friendly minion dies, summon a random minion with the same Cost.")""" in text



    os.remove(expected_cpp_fname)
