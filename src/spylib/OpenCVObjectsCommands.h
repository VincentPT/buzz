#pragma once
#include "spy_interfaces.h"
#include "spylib_interfaces.h"

/// read pointer of cv::Mat object
ReturnData readCVMatObject(void* objectAddres);
/// read pointer of cv::Point object
ReturnData readCVPointObject(void* objectAddres);
/// read pointer of cv::Point2f object
ReturnData readCVPointFObject(void* objectAddres);
/// read pointer of cv::Rect object
ReturnData readCVRectObject(void* objectAddres);
/// read pointer of std::vector<cv::Point> object
ReturnData readCVContour(void* objectAddres);
/// read pointer of std::vector<std::vector<cv::Point>> object
ReturnData readCVContours(void* objectAddres, SortContourMode sortMode);
/// sample API to read a null pointer and return a sample json string
ReturnData readDummyTree(void* dummy);