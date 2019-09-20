#include "CBD_OpenCV_ref.h"
#define PAIR_VECTOR_TYPE std::vector<std::pair<int, int>>



int distance(int x, int y){
	return abs(x - y);
}

int computeDimension(ROI & roi, PAIR_VECTOR_TYPE v1, PAIR_VECTOR_TYPE v2, PAIR_VECTOR_TYPE v3, PAIR_VECTOR_TYPE v4, cv::Mat & image){

	for (PAIR_VECTOR_TYPE::iterator p = v1.begin(); p != v1.end(); p++){
		if (((*p).first > roi.x1) || roi.x1 == -1)
			roi.x1 = (*p).first;
		if (((*p).second > roi.y1) || roi.y1 == -1)
			roi.y1 = (*p).second;
	}
	for (PAIR_VECTOR_TYPE::iterator p = v2.begin(); p != v2.end(); p++){
		if (((*p).first < roi.x2) || roi.x2 == -1)
			roi.x2 = (*p).first;
		if (((*p).second > roi.y2) || roi.y2 == -1)
			roi.y2 = (*p).second;
	}
	for (PAIR_VECTOR_TYPE::iterator p = v3.begin(); p != v3.end(); p++){
		if (((*p).first > roi.x3) || roi.x3 == -1)
			roi.x3 = (*p).first;
		if (((*p).second < roi.y3) || roi.y3 == -1)
			roi.y3 = (*p).second;
	}
	for (PAIR_VECTOR_TYPE::iterator p = v4.begin(); p != v4.end(); p++){
		if (((*p).first < roi.x4) || roi.x4 == -1)
			roi.x4 = (*p).first;
		if (((*p).second < roi.y4) || roi.y4 == -1)
			roi.y4 = (*p).second;
	}

	std::cout << "Corner points:" << std::endl;
	std::cout << "p1 = (" << roi.x1 << ", " << roi.y1 << ")" << std::endl;
	std::cout << "p2 = (" << roi.x2 << ", " << roi.y2 << ")" << std::endl;
	std::cout << "p3 = (" << roi.x3 << ", " << roi.y3 << ")" << std::endl;
	std::cout << "p4 = (" << roi.x4 << ", " << roi.y4 << ")" << std::endl;
	/*
	cv::circle(image, cv::Point(roi.x1, roi.y1), 20, cv::Scalar(0, 255, 0), 6, 8, 0);
	cv::circle(image, cv::Point(roi.x2, roi.y2), 20, cv::Scalar(0, 255, 0), 6, 8, 0);
	cv::circle(image, cv::Point(roi.x3, roi.y3), 20, cv::Scalar(0, 255, 0), 6, 8, 0);
	cv::circle(image, cv::Point(roi.x4, roi.y4), 20, cv::Scalar(0, 255, 0), 6, 8, 0);
	 */
	cv::line(image, cv::Point(roi.x1, roi.y1), cv::Point(roi.x2, roi.y2), cv::Scalar(0, 255, 0), 6, 8, 0);
	cv::line(image, cv::Point(roi.x2, roi.y2), cv::Point(roi.x4, roi.y4), cv::Scalar(0, 255, 0), 6, 8, 0);
	cv::line(image, cv::Point(roi.x4, roi.y4), cv::Point(roi.x3, roi.y3), cv::Scalar(0, 255, 0), 6, 8, 0);
	cv::line(image, cv::Point(roi.x3, roi.y3), cv::Point(roi.x1, roi.y1), cv::Scalar(0, 255, 0), 6, 8, 0);


	// Computing area of the polygon
	// it has to be calculated following the order of the points
	// clockwise or the opposite
	int s = ((roi.x1 * roi.y2) - (roi.x2 * roi.y1));
	s += (roi.x2 * roi.y4) - (roi.x4 * roi.y2);
	s += (roi.x4 * roi.y3) - (roi.x3 * roi.y4);
	s += (roi.x3 * roi.y1) - (roi.x1 * roi.y3);
	return abs(s / 2);

}


PAIR_VECTOR_TYPE computeEdgeError(PAIR_VECTOR_TYPE vError, int minX, int maxX, int minY, int maxY){
	PAIR_VECTOR_TYPE vNoError;
	PAIR_VECTOR_TYPE vErrorEdge;
	for(PAIR_VECTOR_TYPE::iterator error = vError.begin(); error != vError.end(); error++){

		if ((distance(minX, (*error).first) < HANDICAP_ERROR)
			|| (distance(maxX, (*error).first) < HANDICAP_ERROR)
			|| (distance(minY, (*error).second) < HANDICAP_ERROR)
			|| (distance(maxY, (*error).second) < HANDICAP_ERROR))
				vNoError.push_back(*error);
			else
				vErrorEdge.push_back(*error);
	}
	return vErrorEdge;
}

ROI cornerBorderDimensionDetectionCV(std::string imageFileName, std::string outPrefix){
	std::cout << "**********************STARTING " + imageFileName + " CORNER DETECTION**********************\n";

	ROI roi;
	roi.x1 = -1; roi.y1 = -1; roi.x2 = -1; roi.y2 = -1;
	roi.x3 = -1; roi.y3 = -1; roi.x4 = -1; roi.y4 = -1;
	//****************************************************************************STEP 1.1 >> READ THE IMAGE
		std::cout << "\nSTEP 1: READING IMAGE "+ imageFileName +"\n";

		cv::Mat im;
		im = cv::imread(imageFileName, CV_LOAD_IMAGE_GRAYSCALE);
		cv::imwrite(outPrefix + "step_1_1.jpg", im);

	//****************************************************************************STEP 1.2 >> CROP THE IMAGE

		cv::Mat cropped = im(cv::Rect(PIXELS_CROP, PIXELS_CROP, im.cols - PIXELS_CROP * 2, im.rows - PIXELS_CROP * 2));
		imwrite(outPrefix + "step_1_2.jpg", cropped);

		im = cropped;

	//****************************************************************************STEP 1.3 >> REMAP THE IMAGE

		cv::Mat im_remapped;
		cv::Mat map1, map2;
		map1 = cv::Mat::zeros(im.rows, im.cols, CV_16SC2);
		map2 = cv::Mat::zeros(im.rows, im.cols, CV_16SC2);
		cv::Mat camera_matrix, distortion_coefficients;

		cv::Size imageSize(map1.cols, map1.rows);
		cv::FileStorage fs;
		fs.open("camera_calibration.xml", cv::FileStorage::READ);


		fs["Camera_Matrix"] >> camera_matrix;
		fs["Distortion_Coefficients"] >> distortion_coefficients;

		fs.release();

		cv::initUndistortRectifyMap(camera_matrix, distortion_coefficients, cv::Mat(),
			           cv::getOptimalNewCameraMatrix(camera_matrix, distortion_coefficients, imageSize, 1, imageSize, 0),
			           imageSize, CV_16SC2, map1, map2);

		cv::remap(im, im_remapped, map1, map2, cv::INTER_LINEAR , cv::BORDER_CONSTANT, 0);
		imwrite(outPrefix + "step_1_3.jpg", im_remapped);



		//****************************************************************************STEP 2 >> THRESHOLD
		std::cout << "STEP 2: THRESHOLD\n";

		cv::Mat threshold;
		cv::threshold(im_remapped, threshold, THRESH, 255, im_remapped.type());
		//cv::threshold(im, threshold, 110, 255, im.type());
		//cv::threshold(im, threshold, 120, 255, im.type());
		//cv::threshold(im, threshold, 130, 255, im.type());
		cv::imwrite(outPrefix + "step_2.jpg", threshold);


		//****************************************************************************STEP 3 >> FILTERING SMALL ELEMENTS
		std::cout << "STEP 3: FILTERING SMALL ELEMENTS\n";

		cv::Mat erode;
		cv::Mat element1 = cv::getStructuringElement(ERODE_KERNEL_SHAPE,cv::Size(ERODE_FILTER_SIZE, ERODE_FILTER_SIZE), cv::Point(-1, -1));
		cv::erode(threshold, erode, element1, cv::Point(-1, -1), ERODE_ITERATIONS, cv::BORDER_CONSTANT);

		cv::imwrite(outPrefix + "step_3.jpg", erode);


		//****************************************************************************STEP 4 >> FILLING HOLES

		std::cout << "STEP 4: FILLING HOLES\n";

		cv::Mat dilate;
		cv::Mat element2 = cv::getStructuringElement(DILATE_KERNEL_SHAPE,cv::Size(DILATE_FILTER_SIZE, DILATE_FILTER_SIZE), cv::Point(-1, -1));
		cv::dilate(erode, dilate, element2, cv::Point(-1, -1), DILATE_ITERATIONS, cv::BORDER_CONSTANT);

		cv::imwrite(outPrefix + "step_4.jpg", dilate);


		//****************************************************************************STEP 5 >> HARRIS-STEPHENS CORNER DETECTION
		std::cout << "STEP 5: HARRIS-STEPHENS CORNER DETECTION\n";

		cv::Mat harris = cv::Mat::zeros(dilate.rows, dilate.cols, dilate.type());

		cv::cornerHarris(dilate, harris, 5, 7, 0.04, cv::BORDER_DEFAULT);

		//To store the detected corners
		PAIR_VECTOR_TYPE detectedCorners;
		//To show the results of the Harris-Stephens Corner Detection,
		//the resulting points will be drawn in the previous image
		cv::Mat harrisColor;
		cv::cvtColor(dilate, harrisColor, cv::COLOR_GRAY2RGB); //to show the points in color

		int thresh = 20;
		for (int i = 0; i < harris.rows; i++){
			for (int j = 0; j < harris.cols; j++){
				if ((int)harris.at<float>(i, j) > thresh){
					//circles the image at the point (j, i)
					cv::circle(harrisColor, cv::Point(j, i), 20, cv::Scalar(0, 0, 255), 6, 8, 0);

					//creates a new corner and stores it in detectedCorners vector
					std::pair<int, int> corner(j, i);
					detectedCorners.push_back(corner);

					//print the location of the corner
					//std::cout << "(" << j << ", " << i << ")" << std::endl;
				}
			}
		}
		//std::cout << "Detected corners: " << detectedCorners.size() << std::endl;
		cv::imwrite(outPrefix + "step_5.jpg", harrisColor);




		//****************************************************************************STEP 6 >> CORNER CLASSIFICATION
		std::cout << "STEP 6: CORNER CLASSIFICATION\n\n";

		// finding min and max Xs and Ys
		int minX = -1;
		int maxX = -1;
		int minY = -1;
		int maxY = -1;
		for(PAIR_VECTOR_TYPE::iterator corner = detectedCorners.begin(); corner != detectedCorners.end(); corner++){
			if ((minX == -1) || (minX > (*corner).first))
						minX = (*corner).first;
			if ((maxX == -1) || (maxX < (*corner).first))
						maxX = (*corner).first;
			if ((minY == -1) || (minY > (*corner).second))
						minY = (*corner).second;
			if ((maxY == -1) || (maxY < (*corner).second))
						maxY = (*corner).second;
		}

		/*
		std:: cout << "minX: " << minX << std::endl;
		std:: cout << "maxX: " << maxX << std::endl;
		std:: cout << "minY: " << minY << std::endl;
		std:: cout << "maxY: " << maxY << std::endl;
		 */

		//Dividing corners into 5 vectors, 4 for "Real corners" and 1 for pretended errors
		PAIR_VECTOR_TYPE v1;
		PAIR_VECTOR_TYPE v2;
		PAIR_VECTOR_TYPE v3;
		PAIR_VECTOR_TYPE v4;
		PAIR_VECTOR_TYPE vError;
		for(PAIR_VECTOR_TYPE::iterator corner = detectedCorners.begin(); corner != detectedCorners.end(); corner++){

			if ((distance(minX, (*corner).first) < HANDICAP_CORNER) && (distance(minY, (*corner).second) < HANDICAP_CORNER))
				v1.push_back(*corner);

			else if ((distance(maxX, (*corner).first) < HANDICAP_CORNER) && (distance(minY, (*corner).second) < HANDICAP_CORNER))
				v2.push_back(*corner);

			else if ((distance(minX, (*corner).first) < HANDICAP_CORNER) && (distance(maxY, (*corner).second) < HANDICAP_CORNER))
				v3.push_back(*corner);

			else if ((distance(maxX, (*corner).first) < HANDICAP_CORNER) && (distance(maxY, (*corner).second) < HANDICAP_CORNER))
				v4.push_back(*corner);
			else
				vError.push_back(*corner);
		}
		/*
		std::cout << std::endl << "v1: " << v1.size() << " corners" << std::endl;
		std::cout << "v2: " << v2.size() << " corners" << std::endl;
		std::cout << "v3: " << v3.size() << " corners" << std::endl;
		std::cout << "v4: " << v4.size() << " corners" << std::endl;
		std::cout << "vError: " << vError.size() << " corners" << std::endl;
		*/

		// verifiying if the 4 real corner vectors contain any corner
		int cornersFound = 0;
		if (v1.size())
			cornersFound++;
		if (v2.size())
			cornersFound++;
		if (v3.size())
			cornersFound++;
		if (v4.size())
			cornersFound++;

		cv::Mat errorImage;
		cv::cvtColor(dilate, errorImage, cv::COLOR_GRAY2RGB); //to show the points in color

		//CORNER VERIFICATION
		//if there are less than 4 corners found, it means that the tile has one or more damaged corners
		if (cornersFound != 4)
			std::cout << std::endl << "Corner error: less than 4 corners found. Number of corners: " << cornersFound << std::endl;
		else{
			std::cout << "No corner errors found in " + imageFileName << std::endl;


			//BORDER VERIFICATION
			//computeEdgeError is invoked to obtain the points out of edge bounds
			PAIR_VECTOR_TYPE vErrorEdge = computeEdgeError(vError, minX, maxX, minY, maxY);

			if (vErrorEdge.size() != 0){ //border error
				std::cout << "\nBorder error: " << vErrorEdge.size() << " points out of edge bounds" << std::endl;

				//****************************************************************************STEP 7 >> DRAW ERROR IMAGE
				std::cout << "\nSTEP 7: DRAWING ERROR IMAGE\n";

				for(PAIR_VECTOR_TYPE::iterator errorEdge = vErrorEdge.begin(); errorEdge != vErrorEdge.end(); errorEdge++){
					//std::cout << "(" << (*errorEdge).first << ", " << (*errorEdge).second << ")" << std::endl;
					cv::circle(errorImage, cv::Point((*errorEdge).first, (*errorEdge).second), 20, cv::Scalar(255, 0, 255), 6, 8, 0);
				}

			}
			else{//no border error

				std::cout << "No border errors found in " + imageFileName << std::endl;

				//****************************************************************************STEP 8 >> DIMENSION VERIFICATION
				std::cout << "\nSTEP 8: DIMENSION VERIFICATION\n\n";
				cv::Mat dimensionImage = cv::imread(imageFileName, CV_LOAD_IMAGE_COLOR);
				cv::Mat dimensionImage_remapped;
				cv::remap(dimensionImage, dimensionImage_remapped, map1, map2, cv::INTER_LINEAR , cv::BORDER_CONSTANT, 0);
				//cv::cvtColor(im, dimensionImage, cv::COLOR_GRAY2RGB); //to show the points in color

				//DIMENSION VERIFICATION
				//compute dimension to find the area and decide whether it passed or not all the tests
				int area = computeDimension(roi, v1, v2, v3, v4, dimensionImage_remapped);

				//save the image with the 4 real corners drawn
				cv::imwrite(outPrefix + "step_8.jpg", dimensionImage_remapped);

				std::cout << "Area: " << area << std::endl;

				//verify if the area satisfies the criteria
				if (area >= MIN_AREA)
					if (area <= MAX_AREA){
						roi.valid = true;
						std::cout << "\n>>>>>>>>>>>>>>>>>>>>This tile passed the tests!<<<<<<<<<<<<<<<<<<<<<<<<<" << std::endl;

					}
					else{
						int areaError = (area - MAX_AREA);
						std::cout << "Dimension error: the tile is too big. It exceeds the maximum area by " << areaError << " pixels."<< std::endl;
					}

				else{
					int areaError = (MIN_AREA - area);
					std::cout << "Dimension error: the tile is too small. It is below the minimum area by " << areaError << " pixels."<< std::endl;
				}
			}

			cv::imwrite(outPrefix + "step_7.jpg", errorImage);
		}

		std::cout << "\n.............FINISHED " + imageFileName + " PROCESSING\n\n";
		return roi;
}
