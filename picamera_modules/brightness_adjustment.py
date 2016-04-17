#!/usr/bin/env

# Robin Kalia
# robinkalia@berkeley.edu
# brightness_adjustment.py: Contains the code for adjusting the brightness of camera with time

import time
import picamera

with picamera.PiCamera() as camera:
    camera.start_preview()
    try:
        for i in range(100):
            camera.brightness = i
            time.sleep(0.2)
    finally:
        camera.stop_preview()