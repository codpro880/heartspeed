"""
"""
from abc import ABC, abstractmethod

class BaseCard(ABC):
    """
    All cards must have these properties and methods.

    Properties:
    * Attack (int)
    * Card Class (str)
    * Cost (int)
    * Health (int)
    * Name (str)
    * Mechanics (str)
    * Race (str)
    * Rarity (str)
    * Tavern Tier (int)
    * Card Type (str)

    """
    def __init__(self,
                 attack: int,
                 card_class: str,
                 cost: int,
                 health: int,
                 name: str,
                 mechanics: str,
                 race: str,
                 rarity: str,
                 tavern_tier: str,
                 card_type: str):
        self.attack = attack
        self.cost = cost
        self.health = health
        self.name = name
        self.card_class = card_class
        self.mechanics = mechanics
        self.race = race
        self.rarity = rarity
        self.tavern_tier = tavern_tier
        self.card_type = card_type
        super().__init__()



    # @abstractmethod
    # @abstractmethod
    # @abstractmethod
    # @abstractmethod




