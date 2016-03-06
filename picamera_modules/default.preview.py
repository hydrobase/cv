#!/usr/bin/env

# Robin Kalia
# robinkalia@berkeley.edu
# default_preview.py: Contains the code for default preview for a certain period [10 seconds]


import time
import picamera

camera = picamera.PiCamera()
try:
    camera.start_preview()
    time.sleep(10)
    camera.stop_preview()
finally:
    camera.close()