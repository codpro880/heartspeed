""" 
Quick script to show some python usage.
Note: You must build the cli tool first.
Can do this either with `make cli` (no docker)
or with `make docker_cli` (docker)
"""
import json
import os

def main():
    exe_name = 'bgscli'
    flag = '--all-possible-actions'
    try:
        if exe_name in os.listdir('.'):
            stream = os.popen('./' + exe_name + ' ' + flag)
        else:
            stream = os.popen('docker run bgs_cli ' + flag)
    except:
        print(("You must build the cli tool first.\n"
               "Can do this either with `make cli` (no docker)\n"
               "or with `make docker_cli` (docker)\n"))
        exit(1)
    cout = stream.read()
    no_newlines = cout.replace('\n', '')
    res = json.loads(no_newlines)
    print("res.keys(): " + str(list(res.keys())))
    print("len(res['all_possible_actions']): " + str(len(res['all_possible_actions'])))

if __name__ == '__main__':
    main()
