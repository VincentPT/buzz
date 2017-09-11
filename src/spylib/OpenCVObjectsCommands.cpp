#include "OpenCVObjectsCommands.h"
#include "spylib_interfaces.h"

#include <iostream>

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

// using namespaces
using namespace std;
using namespace cv;

ReturnData readMatObject(void* objectAddres) {
	Mat& originImg = *(Mat*)objectAddres;
	Mat img = originImg;

	// currently, we change gray scale to color image
	// because some image lib, is not support load gray scale
	//if (img.channels() == 1) {
	//	Mat imgTmp;
	//	cvtColor(originImg, imgTmp, CV_GRAY2BGR);
	//	img = imgTmp;
	//}

	int stride = img.step[0];
	size_t sizeInBytes = stride * img.rows;
	int totalSize = sizeof(ImageRawData) - sizeof(ImageRawData::data) + sizeInBytes;
	ImageRawData* imgData = (ImageRawData*)malloc(totalSize);

	typedef decltype(ImageRawData::width) IMG_SIZE_T;

	imgData->width = (IMG_SIZE_T)img.cols;
	imgData->height = (IMG_SIZE_T)img.rows;
	imgData->stride = stride;
	if (img.channels() == 1) {
		imgData->pixelFormat = RawDataPixelFormat::GrayScale;
	}
	else if (img.channels() == 3) {
		imgData->pixelFormat = RawDataPixelFormat::BGR;
	}
	else if (img.channels() == 4) {
		imgData->pixelFormat = RawDataPixelFormat::BGRA;
	}

	const uchar* bitmapData;
	uchar* ptr = (uchar*)imgData->data;
	for (int i = 0; i < img.rows; i++) {
		bitmapData = img.ptr(i);
		memcpy_s(ptr, stride, bitmapData, stride);
		ptr += stride;
	}

	ReturnData returnData;

	returnData.customData = (char*)imgData;
	returnData.sizeOfCustomData = totalSize;

	return returnData;
}