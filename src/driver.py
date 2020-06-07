import time, sys

sys.path.append("/Users/will/projects/cpprtx/build")
#sys.path.append("/home/will/projects/cpprtx/build")

import termios, tty, os

'''
def isData():
    return select.select([sys.stdin], [], [], 0) == ([sys.stdin], [], [])

def getch():
	c = None
	old_settings = termios.tcgetattr(sys.stdin)
	try:
	    tty.setcbreak(sys.stdin.fileno())

	    if isData():
	        c = sys.stdin.read(1)

	finally:
	    termios.tcsetattr(sys.stdin, termios.TCSADRAIN, old_settings)
	    return c
'''

def getch():

    fd = sys.stdin.fileno()
    old_settings = termios.tcgetattr(fd)

    try:
        tty.setraw(sys.stdin.fileno())
        #print("tryna read")
        ch = sys.stdin.read(1)
        #print("done read")

    finally:
        termios.tcsetattr(fd, termios.TCSANOW, old_settings)

    return ch


button_delay = 0.2


os.chdir("/Users/will/projects/cpprtx/libs/tf_gl")
from GameContext import *
os.chdir("/Users/will/projects/cpprtx/")


init_window()
run_model()

print("Model ran")

init_game("/Users/will/projects/cpprtx/")
#destroy_game()

timelast = time.perf_counter()
timestart = timelast

while True:
	
	timecur = time.perf_counter()
	timelapsed = timecur - timelast
	timelast = timecur

	step_game(timelapsed)

	if timecur - timestart > 12:
		while timecur - timestart < 8:
			timecur = time.perf_counter()
		break	

destroy_game()