import json
import os

from heartspeed.cards.generate_card_classes import JsonCardGenerator, CardClassGenerator

def test_generate_card_class():
    json_str = """{"artist":"Nutthapon Petchthai","cardClass":"MAGE","collectible":true,"cost":5,"dbfId":2539,"flavor":"It's on the rack next to ice lance, acid lance, and English muffin lance.","id":"AT_001","name":"Flame Lance","playRequirements":{"REQ_MINION_TARGET":0,"REQ_TARGET_TO_PLAY":0},"rarity":"COMMON","set":"TGT","text":"Deal $8 damage to a minion.","type":"SPELL"}"""
    card_json = json.loads(json_str)
    gen = CardClassGenerator(card_json)

    this_dir = os.path.dirname(os.path.realpath(__file__))
    gen.generate_class_file(cards_cpp_dir=this_dir)

    expected_cpp_fname = os.path.join(this_dir, "FlameLance.cpp")
    expected_hpp_fname = os.path.join(this_dir, "FlameLance.hpp")

    assert os.path.exists(expected_cpp_fname)
    assert os.path.exists(expected_hpp_fname)

    # Do some spot checking...
    with open(expected_cpp_fname, 'r') as f:
        text = f.read()
        assert "FlameLance::FlameLance()" in text
        assert 'name("FlameLance")' in text
        assert 'id("AT_001")' in text
        assert 'text("Deal $8 damage to a minion.")' in text
    
    with open(expected_hpp_fname, 'r') as f:
        text = f.read()
        assert "class FlameLance : public BaseCard" in text
        assert "get_name" in text

    os.remove(expected_cpp_fname)
    os.remove(expected_hpp_fname)
