import time, sys, os

projpath = os.getcwd() + "/"
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
init_game(projpath)

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

#os.system('nvidia-smi')
print("CPU FPS: " + str(frames / (timecur - timestart)))

destroy_game()