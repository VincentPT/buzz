#pragma once

enum class RawDataPixelFormat : unsigned short {
    RGB = 0,
    BGR,
	GrayScale,
	RGBA,
	BGRA,
	HSV,
	YUV
};

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

struct PointArrayRawData {
	int n;
	PointRawData points[1];
};

struct PointsArrayRawData {
	int rowCount;
	PointArrayRawData rowsData[1];
};

#pragma pack(pop)