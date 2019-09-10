#ifndef _BLOBS_OPENCV_H_
#define _BLOBS_OPENCV_H_


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "ap_int.h"
#include "hls_stream.h"

#if __SDSCC__
#undef __ARM_NEON__
#undef __ARM_NEON
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#define __ARM_NEON__
#define __ARM_NEON
#else
#include "opencv/cv.h"
#include "opencv/highgui.h"
#include "opencv2/imgproc/imgproc.hpp"
#endif

#include "common/xf_sw_utils.h"

#if __SDSCC__
#include "sds_lib.h"
#define TIME_STAMP_INIT  unsigned int clock_start, clock_end;  clock_start = sds_clock_counter();
#define TIME_STAMP  { clock_end = sds_clock_counter(); printf("elapsed time %lu \n", clock_end-clock_start); clock_start = sds_clock_counter();  }
#endif

//#include  "hls_video.h"
//#include <ap_fixed.h>
//#include "common/xf_axi.h"
//#include "common/xf_infra.h"

#define _USE_MATH_DEFINES
#include "math.h"


//for xfOpenCV functions
//#include "imgproc/xf_threshold.hpp"


struct ROI{ //coordinates of the Region of Interest
	int
	x1, y1,
	x2, y2,
	x3, y3,
	x4, y4;
	bool valid = false;
};

void blobsDetectionCV(std::string imageFileName, ROI roi, std::string outPrefix);

//constants for the algorithm
#define ERODE_KERNEL_SHAPE XF_SHAPE_CROSS
#define ERODE_FILTER_SIZE 3
#define ERODE_ITERATIONS 6

#define DILATE_KERNEL_SHAPE XF_SHAPE_CROSS
#define DILATE_FILTER_SIZE 5
#define DILATE_ITERATIONS 4

#define PI 3.14159265358979323846

#define MIN_BLOB_AREA 5000
#define MIN_BLOB_ROUNDNESS 0.015
#define MIN_BLOB_ROUNDNESS_2 4000

#endif//_BLOBS_OPENCV_H_
