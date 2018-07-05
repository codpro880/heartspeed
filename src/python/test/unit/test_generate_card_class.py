# from heartspeed.cards.generate_card_classes import JsonCardGenerator, CardClassGenerator

# def test_generate_card_class():
#     json_str = """{'artist': 'Nutthapon Petchthai', 'cardClass': 'MAGE', 'collectible': True, 'cost': 5, 'dbfId': 2539, 'flavor': "It's on the rack next to ice lance, acid lance, and English muffin lance.", 'id': 'AT_001', 'name': 'Flame Lance', 'playRequirements': {'REQ_MINION_TARGET': 0, 'REQ_TARGET_TO_PLAY': 0}, 'rarity': 'COMMON', 'set': 'TGT', 'text': 'Deal $8 damage to a minion.', 'type': 'SPELL'}"""
#     card_json = json.loads(json_str)
#     gen = CardClassGenerator(card_json)
    
#     assert ": BaseCard" in gen.class_str
#     assert 'name("Flame Lance")' in gen.class_str
