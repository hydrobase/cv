#!/usr/bin/env

# Robin Kalia
# robinkalia@berkeley.edu
# continuous_image_capture.py: Contains the code for capturing image continuously at regular intervals


import time
import picamera

with picamera.PiCamera() as camera:
    camera.resolution = (1280, 720)
    camera.start_preview()
    time.sleep(1)
    for i, filename in enumerate(camera.capture_continuous('image{counter:02d}.jpg')):
        print('Captured image %s' % filename)
        if i == 100:
            break
        time.sleep(60)
    camera.stop_preview()