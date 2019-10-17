import subprocess as subpro
import sys

turn = 1
speed = 90


control_dir = "./stepper " + str(turns) + " " + str(speed)
try:    
    retcode = subpro.Popen([control_dir], shell=True)
    if retcode < 0:
        print("Child was terminated by signal", -retcode, file=sys.stderr)
    else:
        print("Child returned", retcode, file=sys.stderr)
except OSError as e:
    print("Execution failed:", e, file=sys.stderr)