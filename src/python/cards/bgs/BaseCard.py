"""
"""
from abc import ABC, abstractmethod

class BaseCard(ABC):
    """
    All cards must have these properties and methods.

    Properties:
    * Attack (int)
    * Cost (int)
    * Health (int)
    * Name (str)
    * Card Class (str)
    * Mechanics (str)
    * Race (str)
    * Rarity (str)
    * Tavern Tier (int)

    """
    def __init__(self,
                 attack: int,
                 cost: int,
                 health: int,
                 name: str,
                 card_class: str,
                 mechanics: str,
                 race: str,
                 rarity: str,
                 tavern_tier: str):
        self.attack = attack
        self.cost = cost
        self.health = health
        self.name = name
        self.card_class = card_class
        self.mechanics = mechanics
        self.race = race
        self.rarity = rarity
        self.tavern_tier = tavern_tier
        super().__init__()



    # @abstractmethod
    # @abstractmethod
    # @abstractmethod
    # @abstractmethod




