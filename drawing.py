import bluetooth
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import subprocess
import re
import threading
from time import sleep

addr = "00:18:E4:40:00:06"      # Device Address
port = 1                        # RFCOMM port
connected = False

while (connected == False):
    try:
        s = bluetooth.BluetoothSocket(bluetooth.RFCOMM)
        s.connect((addr,port))
        connected = True
    except Exception as err:
        print("Connection error...")
        print("Retrying...")

print ("Connected... ")

s.send("Hello there")

fig, ax = plt.subplots()

running = True

Rxs = []
Lxs = []
Rys = []
Lys = []

line1, = ax.plot(Rxs, Rys, color = "r")
line2, = ax.plot(Lxs, Lys, color = "g")

lock = threading.Lock()

messageBuffer = ""

def threadFunction():
    global messageBuffer
    while(running):
        lock.acquire()
        try:
            data = s.recv(12800)
            dataDec = data.decode("utf-8", "ignore")
            messageBuffer += dataDec
        finally:
            lock.release()
        sleep(0.1)

thread = threading.Thread(target=threadFunction)
thread.start()

def update(i):
    global messageBuffer, Rxs, Lxs, Lys, Rys, line1, line2
    lock.acquire()
    try:
        flag = True
        while (flag):
            match = re.search('<[^>]+>|$', messageBuffer).group()
            print(match)
            if (len(match) == 0):
                break
            messageBuffer = messageBuffer[len(match):]
            match = match[1: len(match)-1]
            match = match.split(';')
            if (match[0] == 'L'):
                Lys.append(int(match[2]))
                Lxs.append(-int(match[1]))
            else :
                Rys.append(int(match[2]))
                Rxs.append(int(match[1]))
            flag = False
    finally:
        lock.release()

    Rxs = Rxs[-40:]
    Lxs = Lxs[-40:]
    Rys = Rys[-40:]
    Lys = Lys[-40:]

    line2.set_data(Lxs, Lys)
    line1.set_data(Rxs, Rys)
    ax.relim()
    ax.autoscale_view(True,True,True)

    lim = 100
    for item in Rxs:
        if item < 4000 and item > lim:
            lim = item
    for item in Lxs:
        if -item < 4000 and -item > lim:
            lim = -item
    lim += 20

    ax.set_xlim([-lim, lim])

    return [line1, line2]

ani = animation.FuncAnimation(fig, update, interval=100)
ax.set_ylabel('Distance [m]')
ax.axvline(x=0, color='k')
plt.show()

running = False