#!/usr/bin/env

# Robin Kalia
# robinkalia@berkeley.edu
# capture_image_customize.py: Contains the code for customizing exif image tags before capture

import time
import picamera

with picamera.PiCamera() as camera:
    camera.resolution = (2592, 1944)
    camera.start_preview()
    time.sleep(2)
    camera.exif_tags['IFD0.Artist'] = 'Hydrobase!'
    camera.exif_tags['IFD0.Copyright'] = 'Copyright (c) 2016 Hydrobase!'
    camera.capture('foo.jpg')
    camera.stop_preview()