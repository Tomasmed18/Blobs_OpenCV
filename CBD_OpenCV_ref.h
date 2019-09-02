#ifndef _CBD_OPENCV_REF_H_
#define _CBD_OPENCV_REF_H_

#include "Blobs_OpenCV.h"

#define PAIR_VECTOR_TYPE std::vector<std::pair<int, int>>

// for threshold
#define THRESH 85
#define NPIX XF_NPPC8
#define THRESH_TYPE XF_THRESHOLD_TYPE_BINARY

// for ERODE and DILATE functions
#define KERNEL_SHAPE XF_SHAPE_CROSS
#define FILTER_SIZE 3
#define ERODE_ITERATIONS 3
#define DILATE_ITERATIONS 12

//handicaps
#define HANDICAP_CORNER 20
#define HANDICAP_ERROR 15

//dimension verification
#define MIN_AREA 5900000
#define MAX_AREA 6100000



ROI cornerBorderDimensionDetectionCV(std::string imageFileName, std::string outPrefix);

#endif//_CBD_OPENCV_REF_H_
