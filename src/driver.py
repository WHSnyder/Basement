import time, sys, os


projpath = os.getcwd()
buildpath = os.path.join(projpath, "build")
tf_path = os.path.join(projpath, "libs/tf_gl")

print(projpath)
print(tf_path)

sys.path.insert(0,buildpath)
os.chdir(tf_path) #must be done for shared lib dependency to load properly.  Install-name-tool
				  #and linker environment vars are ignored.
from GameContext import *
os.chdir(projpath)

init_window()
run_model()

print("Model ran")

init_game(projpath + "/")
#destroy_game()

timelast = time.perf_counter()
timestart = timelast

while True:
	
	timecur = time.perf_counter()
	timelapsed = timecur - timelast
	timelast = timecur

	step_game(timelapsed)

	if timecur - timestart > 10:
		while timecur - timestart < 1:
			timecur = time.perf_counter()
		break	

destroy_game()