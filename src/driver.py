import time
import sys

sys.path.append("/Users/will/projects/cpprtx/")
sys.path.append("/Users/will/projects/cpprtx/build")

from GameContext import *

init_window()
init_game()

timelast = time.perf_counter()
timestart = timelast

while True:
	
	timecur = time.perf_counter()
	timelapsed = timecur - timelast
	timelast = timecur

	step_game(timelapsed)

	if timecur - timestart > 5:
		break

destroy_game()