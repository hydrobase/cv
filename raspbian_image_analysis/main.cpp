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
	std::string init_image_file, current_image_file;

	if (DEBUG) {
		init_image_file = "../../../pics/plant1.jpg";
		current_image_file = "../../../pics/plant2.jpg";
	} else {
		if ((argc != 2)&&(argc!=3)) {
			std::cout << "Usage: \n\t./ImageProcessor <init_image_file> \n\t./ImageProcessor <init_init_image_file> <current_img_file>" <<  std::endl;
			return ARGUMENT_ERROR;
		}

		init_image_file = std::string(argv[1]);
		if (argc==3)	current_image_file = std::string(argv[2]);
	}


	ImageProcessor image_processor;

	try {
		cv::Mat init_img = cv::imread(init_image_file);
		if (argc == 2) {
			image_processor.DetectDominantColor(init_img);
		} else if (argc == 3) {
			cv::Mat current_img = cv::imread(current_image_file);	
			image_processor.ComputeImagesDifference(init_img, current_img);	
		}
	}
	catch (const std::exception& ex) {
		std::string err(ex.what());
		std::string error_msg = "\nCaught Error: " + err;
		std::cerr << error_msg << std::endl;
	}
	
	return 0;
}
