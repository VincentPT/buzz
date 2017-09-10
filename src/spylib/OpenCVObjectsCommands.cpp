#include "OpenCVObjectsCommands.h"
#include "spylib_interfaces.h"

#include <iostream>

#include <opencv2\core.hpp>

// using namespaces
using namespace std;
using namespace cv;

ReturnData readMatObject(void* objectAddres) {
	Mat& img = *(Mat*)objectAddres;

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
