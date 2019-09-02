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


//for xfOpenCV functions
//#include "imgproc/xf_threshold.hpp"



#endif//_BLOBS_OPENCV_H_
