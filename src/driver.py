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

frames = 0

while True:
	
	timecur = time.perf_counter()
	timelapsed = timecur - timelast
	timelast = timecur

	step_game(timelapsed)

	if timecur - timestart > 3:

		os.system('nvidia-smi')

		while timecur - timestart < 1:
			timecur = time.perf_counter()
		break	

	frames += 1

print("FPS aggregate: " + str(frames / (timecur - timestart)))

destroy_game()