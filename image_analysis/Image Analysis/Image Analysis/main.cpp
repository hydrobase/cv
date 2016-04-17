// Robin Kalia
// robinkalia@berkeley.edu
// UC Berkeley
// Project: Image Processor - Analyzes the images obtained from the hydroponics plant
//
// main.cpp: Illustration of the use of the "ImageProcessor" class


#include "ImageProcessor.h"

#include <stdint.h>

#include <opencv/highgui.h>


enum ERRORS { FUNCTION_ERROR = -2, ARGUMENT_ERROR = -1, SUCCESS = 0 };

const int32_t DEBUG = 0;

int32_t main(int32_t argc, char* argv[])
{
	std::string image_file;

	if (DEBUG) image_file = "plant1.jpg";
	else {
		if (argc != 2) {
			std::cout << "Usage: \n\tWindows:\tImageProcessor.exe <image_file>\n\tLinux/Unix:\t./ImageProcessor <image_file>" <<  std::endl;
			return ARGUMENT_ERROR;
		}

		image_file = std::string(argv[1]);
	}

	cv::Mat img = cv::imread(image_file);

	ImageProcessor image_processor;

	try {
		image_processor.DetectDominantColor(img);
	}
	catch (const std::exception& ex) {
		std::string err(ex.what());
		std::string error_msg = "\nCaught Error: " + err;
		std::cerr << error_msg << std::endl;
	}
	
	return 0;
}