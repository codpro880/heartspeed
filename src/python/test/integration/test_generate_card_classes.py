from heartspeed.cards.generate_card_classes import JsonCardGenerator

def test_get_cards_from_web():
    gen = JsonCardGenerator()
    cards = gen.get_cards_from_web()
    first_card = next(cards)
    assert first_card["id"] # Dunno what it is a priori, but should be something there.

def test_get_cards_from_disk():
    gen = JsonCardGenerator()
    cards = gen.get_cards_from_disk()
    first_card = next(cards)
    assert first_card["name"] == "Flame Lance"
