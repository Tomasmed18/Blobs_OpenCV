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

void verifyDefects(std::string filename, std::string prefix){
	ROI roi = cornerBorderDimensionDetectionCV(filename, prefix + "_verification_0_");
	printROI(roi);
	blobsDetectionCV(filename, roi, prefix + "_verification_1_");

}

int main(int argc, char** argv){

	for (int i = 1; i < 7; i++){
		std::string fileName = "data/Blobs/blobs_" + std::to_string(i) + ".jpg";
		verifyDefects(fileName, std::to_string(i));
	}
	return 0;
}
