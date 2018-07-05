from heartspeed.cards.generate_card_classes import JsonCardGenerator

def test_get_cards_from_web():
    gen = JsonCardGenerator()
    cards = gen.get_cards_from_web(write_to_disk=True)
    first_card = next(cards)
    #import pdb; pdb.set_trace()
    assert first_card["name"] == "Flame Lance"

# def test_get_cards_from_disk():
#     gen = JsonCardGenerator()
#     cards = gen.get_cards_from_disk()
