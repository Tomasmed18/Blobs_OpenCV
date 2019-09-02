#include "Blobs_OpenCV.h"

void blobsDetectionCV(std::string imageFileName, ROI roi, std::string outPrefix){
	std::cout << "**********************STARTING " + imageFileName + " BLOBS DETECTION**********************\n\n";

	//read the image
	cv::Mat in_img;
	in_img = cv::imread(imageFileName, CV_LOAD_IMAGE_GRAYSCALE);

	//crop the image with the ROI info
	cv::Mat step_1 = in_img(cv::Rect(roi.x1, roi.y1, roi.x4 - roi.x1, roi.y4 - roi.y1));
	cv::imwrite(outPrefix + "step_1.jpg", step_1);

	//apply a threshold to the image
	cv::Mat step_2;
	cv::threshold(step_1, step_2, 135, 255, step_1.type());
	cv::imwrite(outPrefix + "step_2.jpg", step_2);

	//invert colors
	cv::Mat step_3;
	cv::bitwise_not(step_2, step_3);
	cv::imwrite(outPrefix + "step_3.jpg", step_3);


	//erode
	cv::Mat step_4;
	cv::Mat element1 = cv::getStructuringElement(ERODE_KERNEL_SHAPE,cv::Size(ERODE_FILTER_SIZE, ERODE_FILTER_SIZE), cv::Point(-1, -1));
	cv::erode(step_3, step_4, element1, cv::Point(-1, -1), ERODE_ITERATIONS, cv::BORDER_CONSTANT);
	cv::imwrite(outPrefix + "step_4.jpg", step_4);

	//dilate
	cv::Mat step_5;
	cv::Mat element2 = cv::getStructuringElement(DILATE_KERNEL_SHAPE,cv::Size(DILATE_FILTER_SIZE, DILATE_FILTER_SIZE), cv::Point(-1, -1));
	cv::dilate(step_4, step_5, element2, cv::Point(-1, -1), DILATE_ITERATIONS, cv::BORDER_CONSTANT);
	cv::imwrite(outPrefix + "step_5.jpg", step_5);

	//for testing different parameter values...
	for(int fs = 3; fs < 9; fs=fs+2){
		for(int it = 2; it < 10; it++){
			cv::Mat element3 = cv::getStructuringElement(DILATE_KERNEL_SHAPE,cv::Size(fs, fs), cv::Point(-1, -1));
			cv::dilate(step_4, step_5, element3, cv::Point(-1, -1), it, cv::BORDER_CONSTANT);
			std::string outFileName = outPrefix + "step_5_fs_" + std::to_string(fs)+ "_it_" + std::to_string(it) + ".jpg";
			cv::imwrite(outFileName, step_5);
		}
	}
}
