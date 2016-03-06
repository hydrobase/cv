#!/usr/bin/env

# Robin Kalia
# robinkalia@berkeley.edu
# rgb_image_capture.py: Contains the code for capturing image stream in rgb format


import time
import picamera
import picamera.array

with picamera.PiCamera() as camera:
    with picamera.array.PiRGBArray(camera) as stream:
        camera.resolution = (1024, 768)
        camera.start_preview()
        time.sleep(2)
        camera.capture(stream, 'rgb')
        print(stream.array.shape)