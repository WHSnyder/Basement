import time, sys, os

sys.path.insert(0,"/home/will/projects/cpprtx/build/")

#os.chdir("/home/will/projects/cpprtx/libs/tf_gl/")
from GameContext import *
#os.chdir("/home/will/projects/cpprtx/")

init_window()
#run_model()

init_game("/home/will/projects/cpprtx/")

timelast = time.perf_counter()
timestart = timelast

while True:
	
	timecur = time.perf_counter()
	timelapsed = timecur - timelast
	timelast = timecur

	step_game(timelapsed)

	if timecur - timestart > 20:
		while timecur - timestart < 1:
			timecur = time.perf_counter()
		break	

destroy_game()