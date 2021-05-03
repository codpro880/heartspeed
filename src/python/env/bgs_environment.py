import json
import os
from pathlib import Path
import tempfile
from typing import Tuple

import gym
import numpy as np
from ray.rllib.env.multi_agent_env import MultiAgentDict, MultiAgentEnv
from ray.rllib.utils.spaces.repeated import Repeated

from env.enumerations import ACTION_ENUM

class BGSEnvironment(MultiAgentEnv):

    def __init__(self, config=dict):
        NUM_ACTIONS = len(ACTION_ENUM)

        self.cards = _get_all_cards()
        num_cards = len(self.cards)

        self.observation_space = gym.spaces.Dict({
            #'state': gym.spaces.Dict({
            'tavern_minions': Repeated(gym.spaces.Discrete(num_cards), max_len=3),
            'hand': Repeated(gym.spaces.Discrete(num_cards), max_len=3),
            'health': gym.spaces.Box(0,41, shape=(1,), dtype=np.int),  # TODO: In the future these may need to be scaled.
            'gold': gym.spaces.Box(0,4, shape=(1,), dtype=np.int),
            #}),
            'action_mask': gym.spaces.Box(0, 2, shape=(NUM_ACTIONS, )),
        })
        self.action_space = gym.spaces.Discrete(NUM_ACTIONS)

        self.base_dir = os.getcwd()
        self.temp_dir = None

    def step(self, action_dict: MultiAgentDict) -> Tuple[MultiAgentDict, MultiAgentDict, MultiAgentDict, MultiAgentDict]:
        reward = {agent: 0 for agent in action_dict}
        done = {'__all__': False, 'p1': False, 'p2': False}

        #print(f'Action Dict: {action_dict}')
        #import pdb; pdb.set_trace()

        for agent_id, action in action_dict.items():
            _take_action(agent_id, action)

        # If P1 end turn and P2 end turn then battle
        if np.all([_player_turn_over(agent_id) for agent_id in action_dict]):
            reward = _battle()
            print(f'Battle: {reward}')
            done = {'__all__': True, 'p1': True, 'p2': True}

        # Get available actions
        observation = self.get_observations()
        # Always done after the battle for now.
        info = {}
        return observation, reward, done, info

    def reset(self):
        self._reset_working_dir()
        run_cli('--reset')

        return self.get_observations()

    def get_observations(self):
        # Get Observations
        state = {agent: self._get_player_observation(agent) for agent in ['p1', 'p2']}
        # Get available actions
        action_mask = {agent: self._get_player_actions(agent) for agent in ['p1', 'p2']}

        observations = {agent: {
            #'state': state[agent],
            'action_mask': action_mask[agent]} for agent in state}
        for agent in observations:
            observations[agent].update(state[agent])
        return observations

    def _get_player_observation(self, agent):
        player_state = _get_player_state(agent)
        tavern_minions = player_state['tavern_minions']
        player_hand = player_state['hand']
        player_health = player_state['health']
        player_gold = player_state['gold']

        if player_hand is None:
            player_hand = [0]
        else:
            player_hand = [self.cards[c['name']] for c in player_hand['cards']]
        tavern_minions = [self.cards[c] for c in tavern_minions]

        return {
            'tavern_minions': tavern_minions,
            'hand': player_hand,
            'health': np.asarray([player_health]),
            'gold': np.asarray([player_gold]),
        }

    def _get_player_actions(self, agent):
        all_actions = np.array(ACTION_ENUM)
        action_mask = np.zeros(len(all_actions))

        available_actions = _get_player_available_actions(agent)
        #print(f'Available Actions for {agent}: {available_actions}')
        for action in available_actions:
            action_mask[all_actions == action] = 1
        return action_mask

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
    #run_cli(f'--{agent_id}-available-actions')
    #run_cli(f'--{agent_id}-take-action BUY_{action_id}')
    #run_cli(f'--{agent_id}-available-actions')
    #run_cli(f'--{agent_id}-take-action PLAY_CARD_FROM_HAND_0_TO_BOARD_0')
    #run_cli(f'--{agent_id}-available-actions')
    #run_cli(f'--{agent_id}-take-action END_TURN')
    if action_id > 0:
        run_cli(f'--{agent_id}-take-action {ACTION_ENUM[action_id]}')


def _get_all_cards():
    card_list = run_cli('--list-cards')
    return {c: i for i, c in enumerate(card_list)}


def _get_player_state(agent):
    player_state = run_cli(f'--{agent}-state')
    return player_state


def _get_player_available_actions(agent):
    player_actions = run_cli(f'--{agent}-available-actions')
    if player_actions is None:
        return ["None"]
    return player_actions['available_actions']


def _player_turn_over(agent):
    available_actions = _get_player_available_actions(agent)
    return len(available_actions) == 1 and available_actions[0] == 'None'


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