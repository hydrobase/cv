// Robin Kalia
// robinkalia@berkeley.edu
// UC Berkeley
// Project: Image Processor - Analyzes the images obtained from the hydroponics plant
//
// ImageProcessor.h: Contains class that carries out image processing operations on the hydroponics plant


#ifndef _HYDROBASE_ANALYTICS_IMAGEANALYSIS_IMAGEPROCESSOR_H
#define _HYDROBASE_ANALYTICS_IMAGEANALYSIS_IMAGEPROCESSOR_H

#include <iostream>

#include <opencv/cv.h>>

#include <stdint.h>

class ImageProcessor {
private:



public:
	ImageProcessor();
	~ImageProcessor();

	void DetectDominantColor(const cv::Mat& img);


	int32_t MAX_VAL;
};


#endif		// _HYDROBASE_ANALYTICS_IMAGEANALYSIS_IMAGEPROCESSOR_H