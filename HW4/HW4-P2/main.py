from threading import Thread
from time import sleep, time

import requests


def task():
    # block for a moment
    r = requests.get('http://example.org')
    # display a message
    # print(r)


if __name__ == '__main__':
    num_of_threads = 20
    num_of_iterations = 1
    times = []
    for i in range(num_of_iterations):
        t = time()
        threads = []
        for i in range(num_of_threads):
            thread = Thread(target=task)
            threads.append(thread)
            thread.start()
        for thread in threads:
            thread.join()
        t = time() - t
        times.append(t)
    avg_time = sum(times) / len(times)
    print(avg_time)
