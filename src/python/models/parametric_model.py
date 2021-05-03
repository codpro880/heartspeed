from gym.spaces import Box

from ray.rllib.agents.dqn.dqn_torch_model import DQNTorchModel
from ray.rllib.models.torch.fcnet import FullyConnectedNetwork
from ray.rllib.utils.torch_ops import FLOAT_MIN, FLOAT_MAX

import torch


class TorchParametricActionsModel(DQNTorchModel):

    def __init__(self,
                 obs_space,
                 action_space,
                 num_outputs,
                 model_config,
                 name,
                 true_obs_shape=(2*867 + 2, ),
                 action_embed_size=9,
                 **kw):
        DQNTorchModel.__init__(self, obs_space, action_space, num_outputs,
                               model_config, name, **kw)

        self.action_embed_model = FullyConnectedNetwork(
            Box(-1, 1, shape=true_obs_shape),
            action_space,
            action_embed_size,
            model_config,
            name + "_action_embed",
        )

    def forward(self, input_dict, state, seq_lens):
        # Extract the available actions tensor from the observation.
        action_mask = input_dict["obs"]["action_mask"]
        #import pdb; pdb.set_trace()

        obs_state = input_dict['obs']#['state']
        batch_size = obs_state['tavern_minions'].values.shape[0]
        minions = obs_state['tavern_minions'].values.reshape(batch_size, -1)
        hand = obs_state['hand'].values.reshape(batch_size, -1)
        obs = torch.cat([minions,
                        hand,
                        obs_state['gold'],
                        obs_state['health']], -1)
        #import pdb; pdb.set_trace()
        # Compute the predicted action embedding
        action_logits, _ = self.action_embed_model({
            "obs": obs,
        })

        # Mask out invalid actions (use -inf to tag invalid).
        # These are then recognized by the EpsilonGreedy exploration component
        # as invalid actions that are not to be chosen.
        inf_mask = torch.clamp(torch.log(action_mask), FLOAT_MIN, FLOAT_MAX)

        #print(f'Action Logits: {action_logits + inf_mask}')
        return action_logits + inf_mask, state

    def value_function(self):
        return self.action_embed_model.value_function()
