import json
import numpy as np
import ray
from ray.rllib.agents import dqn
from ray.rllib.models import ModelCatalog
from ray.rllib.policy import Policy
from ray.tune.registry import get_trainable_cls

from env.bgs_environment import BGSEnvironment
from env.enumerations import ACTION_ENUM
from models.parametric_model import TorchParametricActionsModel


class RandomPolicy(Policy):
    """Hand-coded policy that returns random actions.
    Stollen From: https://github.com/ray-project/ray/issues/7991
    """

    def compute_actions(self,
                        obs_batch,
                        state_batches=None,
                        prev_action_batch=None,
                        prev_reward_batch=None,
                        info_batch=None,
                        episodes=None,
                        **kwargs):
        """Compute actions on a batch of observations."""
        return [self.action_space.sample() for _ in obs_batch], [], {}

    def get_weights(self):
        return None

    def set_weights(self, weights):
        pass

    def learn_on_batch(self, samples):
        """No learning."""
        return {}


def get_agent(algo, checkpoint, obs_space, act_space):
    cls = get_trainable_cls(algo)
    config = dqn.DEFAULT_CONFIG
    config.update({
            'env': BGSEnvironment,
            'framework': 'torch',
            'model': {
                'custom_model': 'bgs_model'
            },
            'multiagent': {
                'policies': {
                    'p1_policy': (None, obs_space, act_space, {}),
                    'p2_random': (RandomPolicy, obs_space, act_space, {}),
                },  
                'policy_mapping_fn': (
                    lambda agent_id: {'p1':'p1_policy', 'p2':'p1_policy'}[agent_id]),
                'policies_to_train': ["p1_policy"]
            }, 
            'hiddens': [],
            'dueling': False,
            'num_workers': 0,
    })
    agent = cls(config, BGSEnvironment)
    agent.restore(checkpoint)
    return agent


if __name__ == '__main__':
    ray.init()

    ModelCatalog.register_custom_model(
        "bgs_model", TorchParametricActionsModel)

    env = BGSEnvironment({})
    obs_space = env.observation_space
    act_space = env.action_space
    print(obs_space)

    agent = get_agent('DQN',
        #'/home/fjones/ray_results/DQN/DQN_BGSEnvironment_492f6_00000_0_2021-05-01_12-07-11/checkpoint_22/checkpoint-22',
        '/home/fjones/ray_results/DQN/DQN_BGSEnvironment_492f6_00000_0_2021-05-01_12-07-11/checkpoint_1/checkpoint-1',
        obs_space,
        act_space)

    # Play 100 Games and Track the actions
    num_games = 1000
    action_list = []

    for _ in range(num_games):
        p1_actions = []
        p2_actions = []

        obs = env.reset()
        done = {'__all__': False}
        action_dict = {'p1': None, 'p2': None}
        while not done['__all__']:
            p1_selection = agent.compute_action(
                obs['p1'], prev_action=action_dict['p1'], prev_reward=None, policy_id='p1_policy'
            )
            p2_selection = agent.compute_action(
                obs['p2'], prev_action=action_dict['p2'], prev_reward=None, policy_id='p1_policy'
            )

            p1_actions.append(ACTION_ENUM[p1_selection])
            p2_actions.append(ACTION_ENUM[p2_selection])

            action_dict = {'p1': p1_selection, 'p2': p2_selection}
            obs, reward, done, info = env.step(action_dict)

        action_list.append(p1_actions)
        action_list.append(p2_actions)

    with open('/home/fjones/Projects/heartspeed/src/python/action_list_not_trained.json', 'w') as f:
        json.dump(action_list, f)


    #confusion_matrix = np.zeros(289)

    #for i in range(289):
    #    if i%10 == 0:
    #        print(i)
    #    obs = np.random.randint(0, 289, size=(1000,3))
    #    for o in obs:
    #        if i not in o:
    #            o[0] = i
    #        selection = agent.compute_action(
    #            o, prev_action=None, prev_reward=None, policy_id='p1_policy'
    #        )
    #        selection = o[selection]
    #        confusion_matrix[selection] += 1

    
    import pdb; pdb.set_trace()