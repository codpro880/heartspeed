from BaseCard import BaseCard


#####################
# Pirate Base Class #
#####################
class PirateCard(BaseCard):

    def __init__(self):
        # TODO
        super().__init__()

    def do_preattack(self, defender, board1, board2):
        # TODO
        return

################
# Pirate Cards #
################
class DreadAdmiralEliza(PirateCard):
    def __init__(self):
        super().__init__(6, "NEUTRAL", 6, 7, "Dread Admiral Eliza",
                        ["TRIGGER_VISUAL"], "PIRATE", "", 6, "MINION")

class DreadAdmiralElizaGolden(PirateCard):
    def __init__(self):
        super().__init__(12, "NEUTRAL", 6, 14, "Dread Admiral Eliza (Golden)",
                        ["TRIGGER_VISUAL"], "PIRATE", "", 6, "MINION")

class RipsnarlCaptain(PirateCard):
    def __init__(self):
        super().__init__(3, "NEUTRAL", 4, 4, "Ripsnarl Captain",
                        ["TRIGGER_VISUAL"], "PIRATE", "", 4, "MINION")

class RipsnarlCaptainGolden(PirateCard):
    def __init__(self):
        super().__init__(6, "NEUTRAL", 4, 8, "Ripsnarl Captain (Golden)",
                        ["TRIGGER_VISUAL"], "PIRATE", "", 4, "MINION")

class SkyPirate(PirateCard):
    def __init__(self):
        super().__init__(1, "ROGUE", 1, 1, "Sky Pirate",
                        [], "PIRATE", "", 1, "MINION")

class SkyPirateGolden(PirateCard):
    def __init__(self):
        super().__init__(2, "ROGUE", 1, 2, "Sky Pirate (Golden)",
                        [], "PIRATE", "", 1, "MINION")

class SouthseaCaptain(PirateCard):
    def __init__(self):
        super().__init__(3, "NEUTRAL", 3, 3, "Southsea Captain",
                        ["AURA"], "PIRATE", "EPIC", 2, "MINION")
    def do_precombat(self):
        #TODO
        pass
    def do_deathrattle(self):
        #TODO
        pass

class SouthseaCaptainGolden(PirateCard):
    def __init__(self):
        super().__init__(6, "NEUTRAL", 3, 6, "Southsea Captain (Golden)",
                        ["AURA"], "PIRATE", "EPIC", 2, "MINION")
    def do_precombat(self):
        #TODO
        pass
    def do_deathrattle(self):
        #TODO
        pass

class YoHoOgre(PirateCard):
    def __init__(self):
        super().__init__(2, "NEUTRAL", 6, 8, "Yo-Ho-Ogre",
                         ["TRIGGER_VISUAL", "TAUNT"], "PIRATE", "", 3, "MINION")
    def do_postdefense(self, attacker, board1, board2):
        #TODO
        pass

class YoHoOgreGolden(PirateCard):
    def __init__(self):
        super().__init__(4, "NEUTRAL", 6, 16, "Yo-Ho-Ogre (Golden)",
                        ["TRIGGER_VISUAL", "TAUNT"], "PIRATE", "", 3, "MINION")
    def do_postdefense(self, attacker, board1, board2):
        #TODO
        pass