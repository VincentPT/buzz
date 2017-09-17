#include "OpenCVObjectsCommands.h"
#include "spylib_interfaces.h"

#include <iostream>

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

// using namespaces
using namespace std;
using namespace cv;

ReturnData readCVMatObject(void* objectAddres) {
	auto& img = *(const Mat*)objectAddres;

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

/// read pointer of cv::Point object
SPYLIB_API ReturnData readCVPointObject(void* objectAddres) {
	auto& point = *(const Point*)objectAddres;

	int totalSize = sizeof(PointRawData);
	PointRawData* rawData = (PointRawData*)malloc(totalSize);

	rawData->x = point.x;
	rawData->y = point.y;

	ReturnData returnData;
	returnData.sizeOfCustomData = totalSize;
	returnData.customData = (char*)rawData;
	return returnData;
}

/// read pointer of cv::Point2f object
SPYLIB_API ReturnData readCVPointFObject(void* objectAddres) {
	auto& point = *(const Point2f*)objectAddres;

	int totalSize = sizeof(Point2fRawData);
	Point2fRawData* rawData = (Point2fRawData*)malloc(totalSize);

	rawData->x = point.x;
	rawData->y = point.y;

	ReturnData returnData;
	returnData.sizeOfCustomData = totalSize;
	returnData.customData = (char*)rawData;
	return returnData;
}

/// read pointer of cv::Rect object
SPYLIB_API ReturnData readCVRectObject(void* objectAddres) {
	auto& rect = *(const Rect*)objectAddres;

	int totalSize = sizeof(RectRawData);
	RectRawData* rawData = (RectRawData*)malloc(totalSize);

	rawData->x = rect.x;
	rawData->y = rect.y;
	rawData->width = rect.width;
	rawData->height = rect.height;

	ReturnData returnData;
	returnData.sizeOfCustomData = totalSize;
	returnData.customData = (char*)rawData;
	return returnData;
}

/// read pointer of std::vector<cv::Point> object
SPYLIB_API ReturnData readCVContour(void* objectAddres) {
	auto& stdpoints = *(const std::vector<Point>*)objectAddres;

	int totalSize = (int)(sizeof(PointArrayRawData) - sizeof(PointArrayRawData::points) + sizeof(PointArrayRawData::points[0]) * stdpoints.size());
	PointArrayRawData* rawData = (PointArrayRawData*)malloc(totalSize);

	rawData->n = (int)stdpoints.size();
	auto& points = rawData->points;

	for (int i = 0; i < rawData->n; i++) {
		points[i].x = stdpoints[i].x;
		points[i].y = stdpoints[i].y;
	}	

	ReturnData returnData;
	returnData.sizeOfCustomData = totalSize;
	returnData.customData = (char*)rawData;
	return returnData;
}

/// read pointer of std::vector<std::vector<cv::Point>> object
SPYLIB_API ReturnData readCVContours(void* objectAddres, SortContourMode sortMode) {
	auto& contours = *(const std::vector<std::vector<cv::Point>>*)objectAddres;

	vector<int> indexes(contours.size());
	for (int i = 0; i < (int)indexes.size(); i++) {
		indexes[i] = i;
	}

	if (sortMode == SortContourMode::PointCountIncrease) {
		std::sort(indexes.begin(), indexes.end(), [&contours](int i1, int i2) {
			return contours[i1].size() < contours[i2].size();
		});
	}
	else if (sortMode == SortContourMode::PointCountDecrease) {
		std::sort(indexes.begin(), indexes.end(), [&contours](int i1, int i2) {
			return contours[i1].size() > contours[i2].size();
		});
	}
	else if (sortMode == SortContourMode::AreaIncrease) {
		vector<double> areas(contours.size());
		for (int i = 0; i < (int)areas.size(); i++) {
			areas[i] = contourArea(contours[i]);
		}

		std::sort(indexes.begin(), indexes.end(), [&areas](int i1, int i2) {
			return areas[i1] < areas[i2];
		});
	}
	else if (sortMode == SortContourMode::AreaDecrease) {
		vector<double> areas(contours.size());
		for (int i = 0; i < (int)areas.size(); i++) {
			areas[i] = contourArea(contours[i]);
		}

		std::sort(indexes.begin(), indexes.end(), [&areas](int i1, int i2) {
			return areas[i1] > areas[i2];
		});
	}

	int totalSize = 0;
	int rowSize;
	for (size_t i = 0; i < contours.size(); i++) {
		auto& stdpoints = contours[i];
		rowSize = (int)(sizeof(PointArrayRawData) - sizeof(PointArrayRawData::points) + sizeof(PointArrayRawData::points[0]) * stdpoints.size());
		totalSize += rowSize;
	}

	totalSize += sizeof(PointsArrayRawData) - sizeof(PointsArrayRawData::rowsData);

	PointsArrayRawData* rawData = (PointsArrayRawData*)malloc(totalSize);
	rawData->rowCount = (int)contours.size();
	PointArrayRawData* rowData = rawData->rowsData;
	for (size_t i = 0; i < contours.size(); i++) {
		auto& stdpoints = contours[indexes[i]];
		rowSize = (int)(sizeof(PointArrayRawData) - sizeof(PointArrayRawData::points) + sizeof(PointArrayRawData::points[0]) * stdpoints.size());

		rowData->n = (int)stdpoints.size();
		auto& points = rowData->points;
		for (size_t j = 0; j < stdpoints.size(); j++) {
			points[j].x = stdpoints[j].x;
			points[j].y = stdpoints[j].y;
		}

		rowData = (PointArrayRawData*)((char*)rowData + rowSize);
	}

	ReturnData returnData;
	returnData.sizeOfCustomData = totalSize;
	returnData.customData = (char*)rawData;
	return returnData;
}