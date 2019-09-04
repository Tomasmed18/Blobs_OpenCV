#include "Blobs_OpenCV.h"

using cv::Mat;
using cv::imread;
using cv::imwrite;
using std::cout;

void blobsDetectionCV(std::string imageFileName, ROI roi, std::string outPrefix){
	cout << "**********************STARTING " + imageFileName + " BLOBS DETECTION**********************\n\n";

	//read the grayscale image
	Mat in_img_grayscale;
	in_img_grayscale = imread(imageFileName, CV_LOAD_IMAGE_GRAYSCALE);
	//read the color image and crop it
	Mat in_img_color;
	in_img_color = imread(imageFileName, CV_LOAD_IMAGE_COLOR);
	Mat in_img_color_cropped = in_img_color(cv::Rect(roi.x1, roi.y1, roi.x4 - roi.x1, roi.y4 - roi.y1));

	//crop the image with the ROI info
	Mat step_1 = in_img_grayscale(cv::Rect(roi.x1, roi.y1, roi.x4 - roi.x1, roi.y4 - roi.y1));
	imwrite(outPrefix + "step_1.jpg", step_1);

	//apply a threshold to the image
	Mat step_2;
	cv::threshold(step_1, step_2, 135, 255, step_1.type());
	imwrite(outPrefix + "step_2.jpg", step_2);

	//invert colors
	Mat step_3;
	cv::bitwise_not(step_2, step_3);
	imwrite(outPrefix + "step_3.jpg", step_3);


	//erode
	Mat step_4;
	Mat element1 = cv::getStructuringElement(ERODE_KERNEL_SHAPE,cv::Size(ERODE_FILTER_SIZE, ERODE_FILTER_SIZE), cv::Point(-1, -1));
	cv::erode(step_3, step_4, element1, cv::Point(-1, -1), ERODE_ITERATIONS, cv::BORDER_CONSTANT);
	imwrite(outPrefix + "step_4.jpg", step_4);

	//dilate
	Mat step_5;
	Mat element2 = cv::getStructuringElement(DILATE_KERNEL_SHAPE,cv::Size(DILATE_FILTER_SIZE, DILATE_FILTER_SIZE), cv::Point(-1, -1));
	cv::dilate(step_4, step_5, element2, cv::Point(-1, -1), DILATE_ITERATIONS, cv::BORDER_CONSTANT);
	imwrite(outPrefix + "step_5.jpg", step_5);


	//for testing different parameter values...
	/*
	for(int erosion_fs = 3; erosion_fs < 9; erosion_fs=erosion_fs+2){
		for(int erosion_it = 2; erosion_it < 10; erosion_it++){

			Mat element4 = cv::getStructuringElement(ERODE_KERNEL_SHAPE,cv::Size(erosion_fs, erosion_fs), cv::Point(-1, -1));
			cv::erode(step_3, step_4, element4, cv::Point(-1, -1), erosion_it, cv::BORDER_CONSTANT);

			for(int dilate_fs = 3; dilate_fs < 9; dilate_fs=dilate_fs+2){
				for(int dilate_it = 2; dilate_it < 10; dilate_it++){
					Mat element3 = cv::getStructuringElement(DILATE_KERNEL_SHAPE,cv::Size(dilate_fs, dilate_fs), cv::Point(-1, -1));
					cv::dilate(step_4, step_5, element3, cv::Point(-1, -1), dilate_it, cv::BORDER_CONSTANT);
					std::string outFileName = outPrefix + "step_5_dfs_" + std::to_string(dilate_fs)+ "_dit_" + std::to_string(dilate_it) +
							"_efs_" + std::to_string(erosion_fs) + "_eit_" + std::to_string(erosion_it) + ".jpg";
					imwrite(outFileName, step_5);
				}
			}
		}
	}
	*/


	std::vector<std::vector<cv::Point> > contours;
	std::vector<cv::Vec4i> hierarchy;
	cv::findContours( step_5, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0) );

	cout << "Contours: " << contours.size() << "\n\n";

	  /// Draw contours
	  Mat step_6;
	  cv::cvtColor(step_1, step_6, cv::COLOR_GRAY2RGB);
	  for( int i = 0; i< contours.size(); i++ )
	     {
		  int r = rand() % 171 + 50;
		  int g = rand() % 171 + 50;
		  int b = rand() % 171 + 50;
	       cv::Scalar color = cv::Scalar(b, g, r);
	       cv::fillConvexPoly(step_6, contours.at(i), color);
	       cv::drawContours( step_6, contours, i, cv::Scalar(0), 5, 8, hierarchy, 0, cv::Point() );
	     }
	  imwrite(outPrefix + "step_6.jpg", step_6);


	  Mat step_7 = in_img_color_cropped.clone();
	  for (short unsigned int i = 0; i < contours.size(); i++){
		  std::vector<cv::Point> contour = contours.at(i);

		  //calculate the contour features
		  double area = cv::contourArea(contour);
		  double perimeter = cv::arcLength(contour, true);
		  double roundness = sqrt(area / PI) / (perimeter * 2 * PI);

		  cout << "Contour " << i << " area: " << area << " perimeter: " << perimeter << "\n     ---> roundness: " << roundness;



		  if (area > MIN_BLOB_AREA && roundness > MIN_BLOB_ROUNDNESS){
			  cout << "                      <-----------------------  **********BLOB DEFECT DETECTED*************\n";
			  cv::Scalar color = cv::Scalar(0, 0, 255);
			  cv::drawContours( step_7, contours, i, color, 5, 8, hierarchy, 0, cv::Point() );
		  }
		  else
			  cout <<"\n";

	  }
	  imwrite(outPrefix + "step_7.jpg", step_7);
	  cout << "\n\n";

}
