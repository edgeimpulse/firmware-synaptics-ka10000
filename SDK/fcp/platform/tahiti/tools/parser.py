import getopt
import hashlib
import logging
import math
import os
import re
import sys
import time
import serial
import serial.tools.list_ports
import errno
import time
from PIL import Image
import matplotlib.pyplot as plt
from datetime import datetime

#Tags need to send out as [$TAGNAME$] [D3] [D1] [D1] [D0] xxxx
#                                     0x[D3..D0] will be number of bytes after D0
tag_name_1 = "[$MAGIC~IMAGE!$]"

def parser_input():
    list_dev = 0
    com_port = ''
    baudrate = 0
    file_name = 'uart_recv.raw'
    show_img = 0

    try:
        options, args = getopt.getopt(sys.argv[1:], "hlc:b:f:s")
    except:
        sys.exit(0)

    for name, value in options:
        if name in ("-h"):
            print("-l\tlist COM ports")
            print("-c\tSelect COM")
            print("-b\tSelect Baudrate")
            print("-f\tSave file name")
            print("-s\tSave file format: RAW/JPG")
            sys.exit(0)
        if name in ("-l"):
            list_dev = 1
        if name in ("-c"):
            if (value.strip() != ''):
                com_port = value.strip()
        if name in ("-b"):
            if (value.strip() != ''):
                baudrate = int(value.strip())
        if name in ("-f"):
            if (value.strip() != ''):
                file_name = value.strip()
        if name in ("-s"):
            show_img = 1

    if (list_dev == 0):
        if (com_port == ''):
            print("ERROR: Wrong COM port")
            sys.exit(-1)
        if (baudrate == 0):
            print("ERROR: Wrong baudrate")
            sys.exit(-1)

    return list_dev, com_port, baudrate, file_name, show_img


def tag_image(file_name, show_img, sp):
    size_list = [0,0,0,0]
    data_cnt = 0
    read_cnt = 0

    f = open(file_name, 'wb')

    while True:
        raw_data = sp.read(1)
        if(data_cnt < 4):
            val = int.from_bytes(raw_data, "big")
            size_list[data_cnt] = val
            data_cnt += 1
            if(data_cnt == 4):
                read_cnt = ((size_list[0] << 24) | (size_list[1] << 16) | (size_list[2] << 8) | (size_list[3]))
                print("Read Count=", read_cnt)
                print("Receiving image..")
                #print(size_list)
                time1 = datetime.now()
                raw_data = sp.read(read_cnt)
                time2 = datetime.now()
                timestamp1 = datetime.timestamp(time1)
                timestamp2 = datetime.timestamp(time2)
                time_int = (timestamp2 - timestamp1)
                print("Done, time = ", time_int)
                f.write(raw_data)
                print("Processing image..")
                raw_image = []
                for i in range(len(raw_data)):
                    b = raw_data[i]
                    #value = int.from_bytes(b, "big")
                    raw_image.append(b)
                new_img = Image.new("L", (324, 324), "white")
                new_img.putdata(raw_image)
                new_file_name = file_name + ".tif"
                new_img.save(new_file_name)
                if(show_img != 0):
                    #new_img.show()
                    plt.figure("Image")
                    plt.imshow(new_img, cmap='gray')
                    plt.axis('on')
                    plt.xticks([])
                    plt.yticks([])
                    plt.show()
                    plt.pause(0.1)
                    plt.clf()
                break
    f.close()


#-------------------------------------------------------------------------------------------

list_dev1, com_port, baudrate, file_name, show_img = parser_input()
#list_dev1 = 0
#com_port = "COM7"
#baudrate = 1536000
#file_name = "tests"
#show_img = 1

if (list_dev1 != 0):
    port_list = list(serial.tools.list_ports.comports())
    if (len(port_list) <= 0):
        print("No serial port connected")
        sys.exit(0)
    else:
        ls = len(port_list)
        for i in range(ls):
            print(list(port_list[i])[0], list(port_list[i])[1])
    sys.exit(0)

try:
    sp = serial.Serial(com_port, baudrate, timeout=10)
except:
    print("Error: Open port failure")
    sys.exit(-1)


plt.ion()

print("Seiral Opened", com_port, baudrate, "bps")

magic = ''
pattern_start = 0

try:
    while True:
        raw_data = sp.read(1)
        val = int.from_bytes(raw_data, "big")
        if (val >= 32) and (val <= 126):
            print(chr(val), end='', file=sys.stdout, flush=True)
        if (val==10):
            print("")

        if (val==ord('[')) and (pattern_start == 0):
            pattern_start = 1
            magic = chr(val)
            continue

        if (pattern_start == 1):
            if (val==ord('$')):
                pattern_start = 2
                magic += chr(val)
            else:
                magic = ''
                pattern_start = 0
            continue

        if (pattern_start >= 1):
            magic += chr(val)
            if(val == ord('$')) and (pattern_start <= 2):
                pattern_start = 3
                continue
            if(pattern_start == 3):
                if(val==ord(']')):
                    pattern_start = 999
                    print("\n\n\n===>find<===\n", magic)
                    if(magic == tag_name_1):
                        tag_image(file_name, show_img, sp)
                    else:
                        print("No service find")
                    print("Finished!\n\n\n")
                    pattern_start = 0
                    magic=''

                else:
                    pattern_start = 2

except KeyboardInterrupt:
    print("Key Break")
#raw_data = sp.read(read_bsize)
print("\nExit..")
sp.close()
