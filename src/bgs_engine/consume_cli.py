""" 
Quick script to show some python usage.
Note: You must build the cli tool first.
Can do this either with `make cli` (no docker)
or with `make docker_cli` (docker)
"""
import json
import os
import random
import subprocess

EXE_NAME = 'bgscli'

def execute_cli_cmd(flag):
    if EXE_NAME in os.listdir('.'):
        stream = os.popen('./' + EXE_NAME + ' ' + flag)
    else:
        stream = os.popen('docker run bgs_cli ' + flag)
    cout = stream.read()
    no_newlines = cout.replace('\n', '')
    try:
        res = json.loads(no_newlines)
    except json.JSONDecodeError:
        # Some commands do not return json, maybe we should change this to return empty json
        res = None
    return res

def main():
    flag = '--all-possible-actions'
    res = execute_cli_cmd(flag)
    print("res.keys(): " + str(list(res.keys())))
    print("len(res['all_possible_actions']): " + str(len(res['all_possible_actions'])))

def random_loop():
    # reset()
    while True:
        if is_dead('p1'):
            print("P2 wins!")
            return
        elif is_dead('p2'):
            print("P1 wins!")
            return
        p1_action = select_random_action('p1')
        p2_action = select_random_action('p2')
        take_action('p1', p1_action)
        take_action('p2', p2_action)
        if turn_ended('p1') and turn_ended('p2'):
            battle()

def reset():
    execute_cli_cmd('--reset')

def is_dead(player):
    res = execute_cli_cmd(f'--{player}-state')
    return res['health'] <= 0

def turn_ended(player):
    res = execute_cli_cmd(f'--{player}-state')
    return res['turn_ended']

def select_random_action(player):
    res = execute_cli_cmd(f'--{player}-available-actions')
    if not res:
        return None
    available_actions = res['available_actions']
    ind = random.randint(0, len(available_actions)-1)
    return available_actions[ind]

def take_action(player, action):
    if not action:
        return
    execute_cli_cmd(f'--{player}-take-action {action}')

def battle():
    print("Battlin...")
    print_board('p1')
    print_board('p2')
    execute_cli_cmd('--battle')

def print_board(player):
    res = execute_cli_cmd(f'--{player}-state')
    print(f"Player {player} board: \n" + json.dumps(res['board'], indent=4))
    
if __name__ == '__main__':
    random_loop()
