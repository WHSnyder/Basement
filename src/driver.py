import time, sys, os


sys.path.insert(0,"/home/will/projects/cpprtx/build/")
os.chdir("/home/will/projects/cpprtx/libs/tf_gl/")
from GameContext import *
os.chdir("/home/will/projects/cpprtx/")

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

	if step_game(timelapsed) < 0:
		break

	if timecur - timestart > 60:
		while timecur - timestart < 1:
			timecur = time.perf_counter()
		break	

	frames += 1

os.system('nvidia-smi')
print("CPU FPS: " + str(frames / (timecur - timestart)))

destroy_game()