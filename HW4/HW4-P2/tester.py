import re
import subprocess
import sys
from subprocess import run, PIPE
from time import sleep


def get_test(num_of_test):
    t1 = []
    t2 = []
    for i in range(num_of_test):
        executable_path = '/YOUR_PYTHON_PATH/python.exe'
        # sleep(0.5)
        out2 = subprocess.check_output([executable_path, './main2.py'])
        out2 = float(str(re.sub("[^0-9]", "", out2.decode("utf-8") )))
        # sleep(0.5)
        out1 = subprocess.check_output([executable_path, './main.py'])
        out1 = float(str(re.sub("[^0-9]", "", out1.decode("utf-8"))))
        # list(p.stdout.split(" "))

        t1.append(out1)
        t2.append(out2)

    print(f"Average runtime with Threads : {sum(t1)/num_of_test} --- Run with async : {sum(t2)/num_of_test}")


if __name__ == '__main__':
    get_test(20)
