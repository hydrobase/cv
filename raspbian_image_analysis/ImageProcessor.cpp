// Robin Kalia
// robinkalia@berkeley.edu
// UC Berkeley
// Project: Image Processor - Analyzes the images obtained from the hydroponics plant
//
// ImageProcessor.cpp: Definition of the various methods and data memebers of the ImageProcessor class


#include "ImageProcessor.h"

#include <opencv/highgui.h>

#include <stdint.h>
#include <time.h>
#include <fstream>
#include <sstream>

const int32_t DISPLAY = 0;

ImageProcessor::ImageProcessor() : MAX_VAL((1 << 8) - 1), HEIGHT_THRESHOLD_NUM_PIXELS(5), PUBLISH_COND(1)
{


}



ImageProcessor::~ImageProcessor()
{


}

namespace std { 

	template<typename T>
	std::string to_string(T value)
	{
		std::ostringstream os;
		os << value;
		return os.str(); 
	}

}

void ImageProcessor::GetUTCTimeStamp(std::string &ts_str)
{
	time_t rawtime;
	time(&rawtime);
	
	struct tm *timestamp = gmtime(&rawtime);
	
	ts_str = std::to_string(timestamp->tm_mon) + "/" + std::to_string(timestamp->tm_mday) + "/" + std::to_string(timestamp->tm_year) + ":- " + std::to_string(timestamp->tm_hour) + ":" + std::to_string(timestamp->tm_min) + ":" + std::to_string(timestamp->tm_sec);
}	


double ImageProcessor::ComputeFoliageDensity(const cv::Mat& img)
{
	int64_t img_area = img.rows * img.cols;
	
	int64_t foliage_pixels_count = 0;

	for (int32_t i = 0; i < img.rows; i++)
		for (int32_t j = 0; j < img.cols; j++) 
			if (img.at<uchar>(cv::Point(j,i)) == MAX_VAL)
				++foliage_pixels_count;	

	double foliage_density = ((double)foliage_pixels_count)/((double)img_area);

	return foliage_density;
}


double ImageProcessor::ComputeFoliageDensityDifference(const cv::Mat &init_img, const cv::Mat &current_img)
{
	double init_img_foliage_density = ComputeFoliageDensity(init_img);
	double current_img_foliage_density = ComputeFoliageDensity(current_img);

       	double foliage_density_diff = current_img_foliage_density - init_img_foliage_density;		
	return foliage_density_diff;	
}


int32_t ImageProcessor::ComputeUpperHeight(const cv::Mat& img)
{
	int32_t upper_height_pixel = -1;
	int32_t white_pixels_count = 0;		

	for (int32_t i = 0; i < img.rows; i++) {
		white_pixels_count = 0;
		for (int32_t j = 0; j < img.cols; j++) 
			if (img.at<uchar>(cv::Point(j,i)) == MAX_VAL)
				++white_pixels_count;	
		if (white_pixels_count >= HEIGHT_THRESHOLD_NUM_PIXELS)	{
			upper_height_pixel = i+1;
			break;
		}
	}
	
	return upper_height_pixel;	
}


int32_t ImageProcessor::ComputeHeightDifference(const cv::Mat &init_img, const cv::Mat &current_img)
{
	int32_t init_img_height_pixel = ComputeUpperHeight(init_img);
	int32_t current_img_height_pixel = ComputeUpperHeight(current_img);

       	int32_t height_diff = init_img_height_pixel - current_img_height_pixel;		

	return height_diff;	
}


void ImageProcessor::ComputeImagesDifference(const cv::Mat &init_img, const cv::Mat &current_img)
{
	cv::Mat HSV;
	cv::cvtColor(init_img, HSV, CV_BGR2HSV);

	cv::Mat threshold_init, threshold_current;

	cv::Scalar lower_green = cv::Scalar(0, 10, 0);
	cv::Scalar upper_green = cv::Scalar(100, 255, 100);

	cv::Mat orig_img = init_img.clone();
	cv::inRange(orig_img, lower_green, upper_green, threshold_init);

	double foliage_density_init = ComputeFoliageDensity(threshold_init);	
	int32_t upper_height_pixel_init = ComputeUpperHeight(threshold_init);

	orig_img = current_img.clone();
	cv::inRange(orig_img, lower_green, upper_green, threshold_current);

	double foliage_density_current = ComputeFoliageDensity(threshold_current);
	int32_t upper_height_pixel_current = ComputeUpperHeight(threshold_current);

	double foliage_density_difference = ComputeFoliageDensityDifference(threshold_init, threshold_current);
	double upper_height_pixel_difference = ComputeHeightDifference(threshold_init, threshold_current);		
	
	std::string ts_str;
        GetUTCTimeStamp(ts_str);
	
	if (PUBLISH_COND) {
		std::string file = "results.txt";

		std::ofstream file_stream(file.c_str());
		if (!file_stream.is_open()) {
			std::cout << "Error encountered: Unable to open the file " << file << std::endl;
			return;
		}

		//file_stream << ts_str << "\n";
		file_stream << std::to_string(upper_height_pixel_current) << "\n";
		file_stream << std::to_string(upper_height_pixel_difference) << "\n";
		file_stream << std::to_string(foliage_density_current * 100.0) << "\n";
		file_stream << std::to_string(foliage_density_difference * 100.0) << "\n";
		
		file_stream.close();	
		//std::string msg =  "'time_stamp': '" + ts_str  + "', 'height' : { 'height_raw': " + std::to_string((int)upper_height_pixel) + ", 'height_increase': 0}, 'foliage_density': { 'fd_raw': " + std::to_string(foliage_density*100.0) + ", 'fd_increase': 0}";	
		//std::cout << msg;
	} else {
		std::cout << "\nImage 1:\n\tFoliage Density = " << foliage_density_init * 100.0 << "%" << std::endl;  	
		std::cout << "\tUppper Height Pixel = " << upper_height_pixel_init << std::endl;  	

		std::cout << "\nImage 2:\n\tFoliage Density = " << foliage_density_current * 100.0 << "%" << std::endl;  	
		std::cout << "\tUppper Height Pixel = " << upper_height_pixel_current  << std::endl;  	

		std::cout << "\nFoliage Density Increment = " << foliage_density_difference * 100.0 << "%";
		std::cout << "\nUpper Height Increment = " << upper_height_pixel_difference << " [" << ((double)upper_height_pixel_difference * 100.0)/((double)current_img.rows) << "%]\n\n";	
	}
}


void ImageProcessor::DetectDominantColor(const cv::Mat& img)
{
	// Detect the Plant regions
	cv::Mat HSV;
	cv::Mat threshold;

	cv::Mat orig_img = img.clone();

	cv::cvtColor(img, HSV, CV_BGR2HSV);
	cv::Scalar lower_green = cv::Scalar(0, 10, 0);
	cv::Scalar upper_green = cv::Scalar(100, 255, 100);
	cv::inRange(orig_img, lower_green, upper_green, threshold);

	double foliage_density = ComputeFoliageDensity(threshold);
	int32_t upper_height_pixel = ComputeUpperHeight(threshold);
	
	std::string ts_str;
 	GetUTCTimeStamp(ts_str);	
	
	if (PUBLISH_COND) {
		std::string file = "results.txt";

		std::ofstream file_stream(file.c_str());
		//std::ofstream file_stream;
		//file_stream.open(file.c_str(), std::ofstream::app);
		if (!file_stream.is_open()) {
			std::cout << "Error encountered: Unable to open the file " << file << std::endl;
			return;
		}

		// file_stream << ts_str << "\n";
		file_stream << std::to_string(upper_height_pixel) << "\n";
		file_stream << "0\n";
		file_stream << std::to_string(foliage_density * 100.0) << "\n";
		file_stream << "0\n";
		
		file_stream.close();	
		//std::string msg =  "'time_stamp': '" + ts_str  + "', 'height' : { 'height_raw': " + std::to_string((int)upper_height_pixel) + ", 'height_increase': 0}, 'foliage_density': { 'fd_raw': " + std::to_string(foliage_density*100.0) + ", 'fd_increase': 0}";	
		//std::cout << msg;
	} else {
		std::cout << "\n\tFoliage Density = " << foliage_density * 100.0 << "%" << std::endl;  	
		std::cout << "\tUppper Height Pixel = " << upper_height_pixel  << std::endl;  	
	}

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
		// cv::namedWindow(display_img_str, CV_WINDOW_NORMAL);
		// cv::imshow(display_img_str, disp_image);
		// cv::waitKey(0);

		std::string result_img_str = "Segmented_Plant_Regions_4.jpg";
		//cv::imwrite(result_img_str, disp_image);
		//std::cout << "\tWritten the resulting image to the file: " << result_img_str << "\n\n";
	}
}

