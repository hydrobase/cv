// Robin Kalia
// robinkalia@berkeley.edu
// UC Berkeley
// Project: Image Processor - Analyzes the images obtained from the hydroponics plant
//
// ImageProcessor.h: Contains class that carries out image processing operations on the hydroponics plant


#ifndef _HYDROBASE_ANALYTICS_IMAGEANALYSIS_IMAGEPROCESSOR_H
#define _HYDROBASE_ANALYTICS_IMAGEANALYSIS_IMAGEPROCESSOR_H

#include <iostream>

#include <opencv/cv.h>

#include <stdint.h>
#include <time.h>
#include <string>

class ImageProcessor {
private:

	double ComputeFoliageDensity(const cv::Mat& img);
	double ComputeFoliageDensityDifference(const cv::Mat &init_img, const cv::Mat &current_img);	
	int32_t ComputeUpperHeight(const cv::Mat& img);
	int32_t ComputeHeightDifference(const cv::Mat &init_img, const cv::Mat &current_img);
	
	void GetUTCTimeStamp(std::string& ts_str);

	int32_t MAX_VAL;
	int32_t HEIGHT_THRESHOLD_NUM_PIXELS;
	int32_t PUBLISH_COND;

public:
	ImageProcessor();
	~ImageProcessor();

	void DetectDominantColor(const cv::Mat& img);
	void ComputeImagesDifference(const cv::Mat &init_img, const cv::Mat &current_img);
};


#endif		// _HYDROBASE_ANALYTICS_IMAGEANALYSIS_IMAGEPROCESSOR_H
