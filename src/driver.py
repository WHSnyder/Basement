import time, sys
sys.path.append("/Users/will/projects/cpprtx/build")

import termios, tty, os


def getch():

    fd = sys.stdin.fileno()
    old_settings = termios.tcgetattr(fd)

    try:
        tty.setraw(sys.stdin.fileno())
        ch = sys.stdin.read(1)

    finally:
        termios.tcsetattr(fd, termios.TCSADRAIN, old_settings)

    return ch
 
button_delay = 0.2



from GameContext import *

init_window()
init_game("/Users/will/projects/cpprtx/")

timelast = time.perf_counter()
timestart = timelast

while True:
	
	timecur = time.perf_counter()
	timelapsed = timecur - timelast
	timelast = timecur

	step_game(timelapsed)

	if timecur - timestart > 10: #getch() == 'q':
		print(globals())
		break

destroy_game()