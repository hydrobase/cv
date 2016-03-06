#!/usr/bin/env

# Robin Kalia
# robinkalia@berkeley.edu
# sequence_image_capture.py: Contains the code for capturing low resolution image sequence


import time
import picamera

with picamera.PiCamera() as camera:
    camera.resolution = (640, 480)
    camera.start_preview()
    start = time.time()
    camera.capture_sequence((
        'image%03d.jpg' % i
        for i in range(120)
        ), use_video_port=True)
    print('Captured 120 images at %.2ffps' % (120 / (time.time() - start)))
    camera.stop_preview()