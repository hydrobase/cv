#!/usr/bin/env

# Robin Kalia
# robinkalia@berkeley.edu
# hydrobase_cv.py: Contains the code for capturing images using picamera module and analyzing them using the ImageProcessor executable

import os
import sys
import time
import picamera
from datetime import datetime
from pubnub import Pubnub


class ImageAnalyzer:

	def __init__(self):
		print "\nImageCapture Class: Capturing images using picamera module and analyzing them using the ImageProcessor executablei followed by publishing them\n"
		self.count = 0
        	self.resolution = (1280, 720)

	def AnalyzeAndPublishImages(self):
		camera = picamera.PiCamera()
		camera.resolution = self.resolution
		camera.start_preview()
		camera.exif_tags['IFD0.Artist'] = 'Hydrobase!'
    		camera.exif_tags['IFD0.Copyright'] = 'Copyright (c) 2016 Hydrobase!'
			
		while (self.count < 10):
		
			results_file = "results.txt"
			if (self.count == 0):
				self.count += 1
		   		image_file = "../pics/plant_" + str(self.count) + ".jpg"
				camera.capture(image_file)
				capture_time = datetime.utcnow()
				print image_file, ": ", capture_time
				cmd = "./ImageAnalyzer " + image_file
				os.system(cmd)
				
				self.PublishImageData(results_file)
				time.sleep(3) #3 seconds - Actually the sleeping time could vary from an interval of 15 minuutes to 24 hours

			else:
				init_image_file = "../pics/plant_" + str(self.count) + ".jpg"
				self.count += 1
				current_image_file = "../pics/plant_" + str(self.count) + ".jpg"
				camera.capture(current_image_file)
				capture_time = datetime.utcnow()
				print current_image_file, ": ", capture_time
				cmd = "./ImageAnalyzer " + init_image_file + " " + current_image_file  
				os.system(cmd) 
				
				self.PublishImageData(results_file)
				time.sleep(3) #3 seconds - Actually the sleeping time could vary from an interval of 15 minuutes to 24 hours

		camera.stop_preview()

	def GetPubnubObject(self, file):
		file_counter = 1
		lines = [line.rstrip() for line in open(file, "r")]
		num_samples = len(lines)/4
		samples_data_list = {}
		for ind in range(num_samples):
			capture_time = datetime.utcnow()
			time_stamp = str(capture_time)
			height_raw = 		int(lines[ind*4])
			height_increase = 	float(lines[ind*4 + 1])
			fd_raw = 		float(lines[ind*4 + 2])
			fd_increase = 		float(lines[ind*4 + 3])
		
			height = {'height_raw': height_raw, 'height_increase': height_increase}
			foliage_density = {'fd_raw': fd_raw, 'fd_increase': fd_increase}
			data = {'time_stamp': time_stamp, 'height': height, 'foliage_density': foliage_density}
			sample = 'sample_' + str(self.count)
			sample_data = {sample: data} 
 			samples_data_list.update(sample_data)

		return samples_data_list

	def PublishImageData(self, file):
		pubnub_data = self.GetPubnubObject(file)
		message = {'CV_Data': pubnub_data}
		pubnub = Pubnub(publish_key="pub-c-93c6c384-e1a0-412f-87cf-e626aaab6a00", subscribe_key="sub-c-8ec9d89e-e4aa-11e5-a4f2-0619f8945a4f", auth_key="40ed6434-1991-4f7a-8034-20a072abde43")
		pubnub.publish("anubhav", message) 	
	
	def callback(self, message):
		print message			

			

if __name__ == '__main__':
	try:
		image_analyzer = ImageAnalyzer()
		image_analyzer.AnalyzeAndPublishImages()
	except:
		print "Error: ", sys.exc_info()[0] 
