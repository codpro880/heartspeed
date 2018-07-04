from heartspeed.cards.generate_card_classes import JsonCardGenerator

def test_generate_card_classes():
    gen = JsonCardGenerator()
    cards = gen.get_cards_from_web()
    first_card = next(cards)
    assert first_card["name"] == "Flame Lance"
