#ifndef _CBD_OPENCV_REF_H_
#define _CBD_OPENCV_REF_H_

#include "Blobs_OpenCV.h"

#define PAIR_VECTOR_TYPE std::vector<std::pair<int, int>>

// to crop the image
#define PIXELS_CROP 1

// for threshold
#define THRESH 35//85
#define NPIX XF_NPPC8
#define THRESH_TYPE XF_THRESHOLD_TYPE_BINARY

// for ERODE function
#define ERODE_KERNEL_SHAPE XF_SHAPE_CROSS
#define ERODE_FILTER_SIZE 3
#define ERODE_ITERATIONS 1//3

// for DILATE function
#define DILATE_KERNEL_SHAPE XF_SHAPE_CROSS
#define DILATE_FILTER_SIZE 5
#define DILATE_ITERATIONS 5//12

//handicaps
#define HANDICAP_CORNER 20
#define HANDICAP_ERROR 20

//dimension verification
#define MIN_AREA 900000
#define MAX_AREA 1000000



ROI cornerBorderDimensionDetectionCV(std::string imageFileName, std::string outPrefix);

#endif//_CBD_OPENCV_REF_H_
