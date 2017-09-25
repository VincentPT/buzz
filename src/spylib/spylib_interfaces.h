#pragma once

// user enum's declaration
enum class UserCommandId : unsigned short {
	OPENCV_READ_MAT_OBJECT = 0,
	OPENCV_READ_CVPOINT_OBJECT,
	OPENCV_READ_CVPOINT2F_OBJECT,
	OPENCV_READ_CVRECT_OBJECT,
	OPENCV_READ_CVCONTOUR,
	OPENCV_READ_CVCONTOURS,
	READ_DUMMYTREE,

	// this must be placed at the end
	PRE_DEFINED_COMMAND_COUNT,
};

enum class ReturnCode : int {
	Success = 0,
	Fail
};

enum class RawDataPixelFormat : unsigned short {
    RGB = 0,
    BGR,
	GrayScale,
	RGBA,
	BGRA,
	HSV,
	YUV
};

enum class SortContourMode : unsigned char {
	AsIs = 0,
	PointCountIncrease,
	PointCountDecrease,
	AreaIncrease,
	AreaDecrease
};

// user structure's declaration
#pragma pack(push, 1)
struct ImageRawData {
	unsigned short width;
    unsigned short height;
    int stride;
    RawDataPixelFormat pixelFormat;
    char data[2];
};

struct PointRawData {
	int x;
	int y;
};

struct Point2fRawData {
	float x;
	float y;
};

struct RectRawData {
	int x;
	int y;
	int width;
	int height;
};

struct RectArrayRawData {
	int rowCount;
	RectRawData rowsData[1];
};


struct PointArrayRawData {
	int n;
	PointRawData points[1];
};

struct PointsArrayRawData {
	int rowCount;
	PointArrayRawData rowsData[1];
};

struct RawBuffer {
	unsigned int size;
	char data[1];
};

#pragma pack(pop)
