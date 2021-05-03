import numpy as np
import ray
from ray import tune
from ray.rllib.models import ModelCatalog
from ray.rllib.policy import Policy
from env.bgs_environment import BGSEnvironment
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
        def sample(obs):
            possible_actions = obs[:9] == 1
            if obs[2]:
                return 2
            if obs[7]:
                return 7
            if obs[1]:
                return 1
            return 0
            if np.sum(possible_actions) == 0:
                return 0
            possible_actions = np.arange(9)[possible_actions]
            return np.random.choice(possible_actions, 1)[0]

        action = [sample(obs) for obs in obs_batch]
        return action, [], {}

    def get_weights(self):
        return None

    def set_weights(self, weights):
        pass

    def learn_on_batch(self, samples):
        """No learning."""
        return {}


if __name__ == '__main__':
    ray.init(
        #dashboard_host='172.17.64.235',
    )

    ModelCatalog.register_custom_model(
        "bgs_model", TorchParametricActionsModel)

    env = BGSEnvironment({})
    obs_space = env.observation_space
    act_space = env.action_space

    tune.run(
        "DQN",
        config={
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
                    lambda agent_id: {'p1':'p1_policy', 'p2':'p2_random'}[agent_id]),
                'policies_to_train': ["p1_policy"]
            }, 
            'hiddens': [],
            'dueling': False,
            'num_workers': 0,
        },
        checkpoint_freq=1,
        checkpoint_at_end=True,
    )