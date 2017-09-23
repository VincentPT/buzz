#pragma once
#include "../spy/spy_interfaces.h"
#include "spylib_interfaces.h"
#include "spylib.h"

/// read pointer of cv::Mat object
SPYLIB_API ReturnData readCVMatObject(void* objectAddres);
/// read pointer of cv::Point object
SPYLIB_API ReturnData readCVPointObject(void* objectAddres);
/// read pointer of cv::Point2f object
SPYLIB_API ReturnData readCVPointFObject(void* objectAddres);
/// read pointer of cv::Rect object
SPYLIB_API ReturnData readCVRectObject(void* objectAddres);
/// read pointer of std::vector<cv::Point> object
SPYLIB_API ReturnData readCVContour(void* objectAddres);
/// read pointer of std::vector<std::vector<cv::Point>> object
SPYLIB_API ReturnData readCVContours(void* objectAddres, SortContourMode sortMode);
/// sample API to read a null pointer and return a sample json string
SPYLIB_API ReturnData readDummyTree(void* dummy);