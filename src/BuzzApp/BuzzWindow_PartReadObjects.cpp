#include "BuzzWindow.h"

using namespace ci;
using namespace ci::app;

void BuzzWindow::readObject(void* desireReadObjectAddress, int type) {
	if (desireReadObjectAddress == nullptr) {
		return;
	}

	if (!_spyClient) {
		_spyClient = std::make_shared<BuzzSpyClient>();
	}
	auto& currentMonitorProcessName = _spyClient->getProcessName();
	if (currentMonitorProcessName.empty()) {
		return;
	}

	bool res = _spyClient->checkTargetAvaible();
	if (res == false) {
		res = _spyClient->restartMonitorProcess();
	}

	if (res == false) {
		return;
	}

	switch (type)
	{
	case 0:
		readMatObject(desireReadObjectAddress);
		break;
	default:
		break;
	}
}

void BuzzWindow::readMatObject(void* desireReadObjectAddress) {
	_spyClient->readCVMat(desireReadObjectAddress, [this](ImageRawData* &ptr) {
		SurfaceRef surFace;
		if (ptr->pixelFormat == RawDataPixelFormat::BGR) {
			surFace = Surface::create((uint8_t*)ptr->data, ptr->width, ptr->height, ptr->stride, cinder::SurfaceChannelOrder::BGR);
		}
		else if (ptr->pixelFormat == RawDataPixelFormat::RGB) {
			surFace = Surface::create((uint8_t*)ptr->data, ptr->width, ptr->height, ptr->stride, cinder::SurfaceChannelOrder::RGB);
		}
		else if (ptr->pixelFormat == RawDataPixelFormat::BGRA) {
			surFace = Surface::create((uint8_t*)ptr->data, ptr->width, ptr->height, ptr->stride, cinder::SurfaceChannelOrder::BGRA);
		}
		else if (ptr->pixelFormat == RawDataPixelFormat::RGBA) {
			surFace = Surface::create((uint8_t*)ptr->data, ptr->width, ptr->height, ptr->stride, cinder::SurfaceChannelOrder::RGBA);
		}
		else if (ptr->pixelFormat == RawDataPixelFormat::GrayScale) {
			//surFace = Surface::create((uint8_t*)ptr->data, ptr->width, ptr->height, ptr->stride, cinder::SurfaceChannelOrder::CHAN_GREEN);
			surFace = Surface::create(ptr->width, ptr->height, false, cinder::SurfaceChannelOrder::BGR);
			auto pixelBytes = surFace->getPixelBytes();
			auto diff = surFace->getRowBytes();
			auto csur = surFace->getData();
			auto cimg = ptr->data;
			auto stride = ptr->stride;
			for (int i = 0; i < ptr->height; i++) {
				auto c1 = cimg;
				auto c1End = c1 + ptr->width;
				auto c3 = csur;

				while(c1 < c1End) {
					*(c3) = *c1;
					*(c3+1) = *c1;
					*(c3+2) = *c1++;
					c3 += pixelBytes;
				}

				cimg += stride;
				csur += diff;
			}
		}
		if (surFace) {
			_tex = gl::Texture2d::create(*surFace.get());
		}
	});
}