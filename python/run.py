import os
import threading

ful = "dasdsadsadvckldjwqiehinfdfdfdfdfdfdddqsadwwsdasdwaqaawwawawawwwwaw"
subs = ["th", "tr", "on", "ye", "wh", "o", "a", "qw", "ww", "aq"]
i = 0

def call_cpp():
	global i
	print " Starting a client with task=", subs[i]
	os.system('client/client ' + subs[i] + ' ' + ful)
	i += 1
	if i >= len(subs):
		i = 0
	threading.Timer(0.01, call_cpp).start()

call_cpp()
