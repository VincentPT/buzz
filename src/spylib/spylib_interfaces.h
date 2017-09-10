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

#pragma pack(pop)