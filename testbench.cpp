#include "Blobs_OpenCV.h"
#include "CBD_OpenCV_ref.h"

void printROI(ROI roi){
	std::cout << "\nROI: ";
	if (roi.valid){
	std::cout << "VALID\n";
	std::cout << "x1  " << roi.x1 << "  y1  " << roi.y1 << "\n";
	std::cout << "x2  " << roi.x2 << "  y2  " << roi.y2 << "\n";
	std::cout << "x3  " << roi.x3 << "  y3  " << roi.y3 << "\n";
	std::cout << "x4  " << roi.x4 << "  y4  " << roi.y4 << "\n\n";
	}
	else{
		std::cout << "INVALID\n\n";
	}
}

int main(int argc, char** argv){
	ROI roi = cornerBorderDimensionDetectionCV("data/Blobs/blobs_1.jpg", "test_");
	printROI(roi);
	return 0;
}
