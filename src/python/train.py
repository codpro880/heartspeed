import ray
from ray import tune
from ray.rllib.policy import Policy
from env.bgs_environment import BGSEnvironment


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


if __name__ == '__main__':
    ray.init(
        dashboard_host='172.17.64.235',
    )

    env = BGSEnvironment({})
    obs_space = env.observation_space
    act_space = env.action_space

    tune.run(
        "DQN",
        config={
            'env': BGSEnvironment,
            'framework': 'torch',
            'multiagent': {
                'policies': {
                    'p1_policy': (None, obs_space, act_space, {}),
                    'p2_random': (RandomPolicy, obs_space, act_space, {}),
                },  
                'policy_mapping_fn': (
                    lambda agent_id: {'p1':'p1_policy', 'p2':'p2_random'}[agent_id]),
                'policies_to_train': ["p1_policy"]
            }, 
            'num_workers': 3,
        }
    )