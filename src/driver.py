import time, sys

sys.path.append("/Users/will/projects/cpprtx/build")
#sys.path.append("/home/will/projects/cpprtx/build")

import termios, tty, os

os.chdir("/Users/will/projects/cpprtx/libs/master_tf")
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

	if timecur - timestart > 2:
		while timecur - timestart < 1:
			timecur = time.perf_counter()
		break	

destroy_game()