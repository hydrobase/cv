// Robin Kalia
// robinkalia@berkeley.edu
// UC Berkeley
// Project: Image Processor - Analyzes the images obtained from the hydroponics plant
//
// ImageProcessor.cpp: Definition of the various methods and data memebers of the ImageProcessor class


#include "ImageProcessor.h"

#include <opencv/highgui.h>

#include <stdint.h>

const int32_t DISPLAY = 1;

ImageProcessor::ImageProcessor() : MAX_VAL((1 << 8) - 1)
{


}



ImageProcessor::~ImageProcessor()
{


}



void ImageProcessor::DetectDominantColor(const cv::Mat& img)
{
	// Detect the tin range regions
	cv::Mat HSV;
	cv::Mat threshold;

	cv::Mat orig_img = img.clone();

	cv::cvtColor(img, HSV, CV_BGR2HSV);
	cv::Scalar lower_green = cv::Scalar(0, 10, 0);
	cv::Scalar upper_green = cv::Scalar(100, 255, 100);
	cv::inRange(orig_img, lower_green, upper_green, threshold);

	if (DISPLAY) {
		cv::Mat disp_image(img.rows, 3*img.cols, CV_8UC3, cv::Scalar(0));
		cv::Rect roi_boundary(0, 0, img.cols, img.rows);
		cv::Mat roi = disp_image(roi_boundary);
		orig_img.copyTo(roi);

		cv::Mat thresh_image;
		cvtColor(threshold, thresh_image, CV_GRAY2BGR);
		roi_boundary = cv::Rect(img.cols, 0, img.cols, img.rows);
		roi = disp_image(roi_boundary);
		thresh_image.copyTo(roi);

		roi_boundary = cv::Rect(2*img.cols, 0, img.cols, img.rows);
		roi = disp_image(roi_boundary);
		for (int32_t i = 0; i < threshold.rows; i++)
			for (int32_t j = 0; j < threshold.cols; j++) 
				if (threshold.at<uchar>(cv::Point(j,i)) == MAX_VAL)
					for (int32_t k = 0; k < 3; k++)		
						roi.at<cv::Vec3b>(cv::Point(j, i))[k] = orig_img.at<cv::Vec3b>(cv::Point(j, i))[k];
		
		std::string display_img_str = "Segmented Plant Regions";
		cv::namedWindow(display_img_str, CV_WINDOW_NORMAL);
		cv::imshow(display_img_str, disp_image);
		cv::waitKey(0);

		cv::imwrite("Segmented_Plant_Regions_6.jpg", disp_image);
	}
}

