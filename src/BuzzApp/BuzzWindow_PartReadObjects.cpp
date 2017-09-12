#include "BuzzWindow.h"
#include "spy_interfaces.h"

#include "drawobjs/BuzzPoint.h"
#include "drawobjs/BuzzRect.h"
#include "drawobjs/BuzzPolygon.h"
#include "drawobjs/BuzzContainer.h"

using namespace ci;
using namespace ci::app;

void BuzzWindow::readObject(void* desireReadObjectAddress, int type) {
	if (desireReadObjectAddress == nullptr) {
		return;
	}

	if (type < 0 || type >= (int)CustomCommandId::CUSTOM_COMMAND_END) {
		return;
	}

	if (!_spyClient) {
		_spyClient = std::make_shared<BuzzSpyClient>();
	}

	auto& currentMonitorProcessName = _spyClient->getProcessName();
	if (currentMonitorProcessName.empty()) {
		this->onStartMonitorProcess(nullptr);
	}

	bool res = _spyClient->checkTargetAvaible();
	if (res == false) {
		res = _spyClient->restartMonitorProcess();
	}

	if (res == false) {
		return;
	}

	typedef void (BuzzWindow::*ReadObjectFunc) (void*);

	static ReadObjectFunc readObjectFuncs[] = {
		&BuzzWindow::readCVMatObject,
		&BuzzWindow::readCVPointObject,
		&BuzzWindow::readCVPoint2fObject,
		&BuzzWindow::readCVRectObject,
		&BuzzWindow::readCVContourObject,
		&BuzzWindow::readCVContoursObject,
	};

	(this->*readObjectFuncs[type])(desireReadObjectAddress);

	// need update FBO on draw method because of new object created by above handlers
	needUpdate();
}

void BuzzWindow::readCVMatObject(void* desireReadObjectAddress) {
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

void BuzzWindow::readCVPointObject(void* desireReadObjectAddress) {
	_spyClient->readCVPoint(desireReadObjectAddress, [this](PointRawData* &ptr) {		
		BuzzPoint* bzPoint = new BuzzPoint((float)ptr->x, (float)ptr->y);
		BuzzDrawObjRef objRef(bzPoint);
		_drawingObjects.push_back(objRef);
	});
}

void BuzzWindow::readCVPoint2fObject(void* desireReadObjectAddress) {
	_spyClient->readCVPoint2f(desireReadObjectAddress, [this](Point2fRawData* &ptr) {
		BuzzPoint* bzPoint = new BuzzPoint(ptr->x, ptr->y);
		BuzzDrawObjRef objRef(bzPoint);
		_drawingObjects.push_back(objRef);
	});
}

void BuzzWindow::readCVRectObject(void* desireReadObjectAddress) {
	_spyClient->readCVRect(desireReadObjectAddress, [this](RectRawData* &ptr) {
		BuzzRect* bzRect = new BuzzRect(ptr->x, ptr->y, ptr->width, ptr->height);
		BuzzDrawObjRef objRef(bzRect);
		_drawingObjects.push_back(objRef);
	});
}

BuzzPolygon* convertToBuzzOject(PointArrayRawData* ptr) {
	BuzzPolygon* bzPolygon = new BuzzPolygon();

	PointRawData* p = ptr->points;
	PointRawData* end = p + ptr->n;

	while (p < end)
	{
		bzPolygon->addPoint((float)p->x, (float)p->y);
		p++;
	}

	return bzPolygon;
}

void BuzzWindow::readCVContourObject(void* desireReadObjectAddress) {
	_spyClient->readCVContour(desireReadObjectAddress, [this](PointArrayRawData* &ptr) {
		BuzzPolygon* bzPolygon = convertToBuzzOject(ptr);
		BuzzDrawObjRef objRef(bzPolygon);
		_drawingObjects.push_back(objRef);
	});
}

void BuzzWindow::readCVContoursObject(void* desireReadObjectAddress) {
	_spyClient->readCVContours(desireReadObjectAddress, [this](PointsArrayRawData* &ptr) {
		BuzzContainer* bzContainer = new BuzzContainer();

		PointArrayRawData* rowData = ptr->rowsData;
		for (int i = 0; i < ptr->rowCount; i++) {
			int rowSize = sizeof(rowData->n) + rowData->n * sizeof(PointRawData);

			auto polygon = convertToBuzzOject(rowData);
			bzContainer->addObject(BuzzDrawObjRef(polygon));
			
			rowData = (PointArrayRawData*)((char*)rowData + rowSize);
		}

		BuzzDrawObjRef objRef(bzContainer);
		_drawingObjects.push_back(objRef);
	});
}