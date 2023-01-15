import matplotlib.pyplot as plt
import os
import re
directory = 'outputs'

for filename in os.listdir(directory):
    f = os.path.join(directory, filename)
    if os.path.isfile(f):
        line_list = []
        length_list = []
        file = open(f)
        for i, line in enumerate(file):
            if "length" in line:
                line_list.append(i)
                m = re.search('length is ([\d]+)', line)
                m.group(0)
                length_list.append(int(m.group(1)))
        plt.plot(line_list, length_list)
        plt.ylabel('length_list')
        plt.xlabel('line_list')
        # plt.show()
        plt.savefig(f'{directory}/{filename}.png')
        plt.clf()
        line_list = []
        length_list = []