import time, sys
sys.path.append("/Users/will/projects/cpprtx/build")

from GameContext import *

init_window()
init_game("/Users/will/projects/cpprtx/")

timelast = time.perf_counter()
timestart = timelast

while True:
	
	timecur = time.perf_counter()
	timelapsed = timecur - timelast
	timelast = timecur

	inputbreak = step_game(timelapsed)

	if timecur - timestart > 10:
		break

destroy_game()