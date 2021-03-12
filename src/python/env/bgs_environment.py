import json
import os
from pathlib import Path
import tempfile
from typing import Tuple

import gym
from ray.rllib.env.multi_agent_env import MultiAgentDict, MultiAgentEnv

class BGSEnvironment(MultiAgentEnv):

    def __init__(self, config=dict):
        NUM_ACTIONS = 3

        self.cards = _get_all_cards()
        num_cards = len(self.cards)

        self.observation_space = gym.spaces.MultiDiscrete([num_cards] * 3)
        self.action_space = gym.spaces.Discrete(NUM_ACTIONS)

        self.base_dir = os.getcwd()
        self.temp_dir = None

    def step(self, action_dict: MultiAgentDict) -> Tuple[MultiAgentDict, MultiAgentDict, MultiAgentDict, MultiAgentDict]:
        # reward = take_action
        for agent_id, action in action_dict.items():
            _take_action(agent_id, action)

        reward = _battle()

        observation = self.get_observations()
        # Always done after the battle for now.
        done = {'__all__': True, 'p1': True, 'p2': True}
        info = {}
        return observation, reward, done, info

    def reset(self):
        self._reset_working_dir()
        run_cli('--reset')
        return self.get_observations()

    def get_observations(self):
        observations = {agent: self._get_player_observation(agent) for agent in ['p1', 'p2']}
        return observations

    def _get_player_observation(self, agent):
        player_state = _get_player_state(agent)
        player_cards = player_state['tavern_minions']
        return [self.cards[c] for c in player_cards]

    def _reset_working_dir(self):
        os.chdir(self.base_dir)
        if self.temp_dir is not None:
            self.temp_dir.cleanup()
        self.temp_dir = tempfile.TemporaryDirectory()
        os.chdir(self.temp_dir.name)


def _battle():
    result = run_cli('--battle')

    p1_reward = 1 if result['who_won'] == 'p1' else 0
    p2_reward = 1 if result['who_won'] == 'p2' else 0
    return {'p1': p1_reward, 'p2': p2_reward}

def _take_action(agent_id, action_id):
    run_cli(f'--{agent_id}-available-actions')
    run_cli(f'--{agent_id}-take-action BUY_{action_id}')
    run_cli(f'--{agent_id}-available-actions')
    run_cli(f'--{agent_id}-take-action PLAY_CARD_FROM_HAND_0_TO_BOARD_0')
    run_cli(f'--{agent_id}-available-actions')
    run_cli(f'--{agent_id}-take-action END_TURN')


def _get_all_cards():
    card_list = run_cli('--list-cards')
    return {c: i for i, c in enumerate(card_list)}


def _get_player_state(agent):
    player_state = run_cli(f'--{agent}-state')
    return player_state


def run_cli(flag):
    exe_name = f'{Path(__file__).parent.parent.parent.absolute()}/bgs_engine/bgscli'
    try:
        stream = os.popen(f'{exe_name} {flag}')
    except:
        print(("You must build the cli tool first.\n"
               "Can do this either with `make cli` (no docker)\n"
               "or with `make docker_cli` (docker)\n"))
        exit(1)
    cout = stream.read()
    res = None
    if cout:
        no_newlines = cout.replace('\n', '')
        res = json.loads(no_newlines)
    return res