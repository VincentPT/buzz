#include "BuzzApp.h"
#include <stdio.h>
#include <io.h>
#include <fcntl.h>

#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/Rand.h"
#include <list>
#include "BuzzWindow.h"
#include "utils/ClipboardViewer.h"

#include "utils/string_util.hpp"
#include "spy_interfaces.h"

using namespace ci;
using namespace ci::app;
using namespace std;

BuzzApp::BuzzApp() {
}
BuzzApp::~BuzzApp() {
}

void setupConsole() {
	//AllocConsole();

	//HANDLE handle_out = GetStdHandle(STD_OUTPUT_HANDLE);
	//int hCrt = _open_osfhandle((intptr_t)handle_out, _O_TEXT);
	//FILE* hf_out = _fdopen(hCrt, "w");
	//setvbuf(hf_out, NULL, _IONBF, 1);
	//*stdout = *hf_out;

	//HANDLE handle_in = GetStdHandle(STD_INPUT_HANDLE);
	//hCrt = _open_osfhandle((intptr_t)handle_in, _O_TEXT);
	//FILE* hf_in = _fdopen(hCrt, "r");
	//setvbuf(hf_in, NULL, _IONBF, 128);
	//*stdin = *hf_in;

	AllocConsole();
	freopen("CONIN$", "r", stdin);
	freopen("CONOUT$", "w", stdout);
	freopen("CONOUT$", "w", stderr);
}

void BuzzApp::setup()
{
	using namespace std::placeholders;

	setupConsole();

	WindowRef defaultNativeWindow = getWindow();

	auto firstWindow = new BuzzWindow(defaultNativeWindow);
	firstWindow->setTitle("buzz");

	ClipboardViewer* clipboardViewer = ClipboardViewer::getInstance();
	// register ANSI text handler
	clipboardViewer->connectANSI(std::bind(&BuzzApp::onClipboardTextChanged, this, _1));
}

void BuzzApp::createNewWindow()
{
	auto newWindow = new BuzzWindow("buzz#" + std::to_string(getNumWindows()), 960, 540);
}

void BuzzApp::mouseDrag( MouseEvent event )
{
}

void BuzzApp::keyDown( KeyEvent event )
{
/*	if( event.getChar() == 'f' )
		setFullScreen( ! isFullScreen() );
	else */
	if( event.getChar() == 'q' )
		createNewWindow();
}

void BuzzApp::update() {

}

void BuzzApp::draw() {
	auto currentNativeWindow = getWindow();
	auto currentWindow = currentNativeWindow->getUserData<BuzzWindow>();
	currentWindow->draw();
}

void BuzzApp::onClipboardTextChanged(const std::string& text) {
	auto activeWindow = this->getForegroundWindow();
	cout << "process for changing text in clipboard" << std::endl;

	void* address = ObjectInputerDlg::convertToAddress(text);
	if (address != nullptr) {
		auto buzzWindow = activeWindow->getUserData<BuzzWindow>();
		buzzWindow->showInputerWithAddress(address);
	}
	else {
		cout << "check clipboard for visual studio's copied text" << std::endl;
		// parse string copy from a row in Watch window of visual studio
		vector<string> elems;
		split(text, '\t', elems);

		if (elems.size() == 5) {
			auto name = elems[2];
			auto value = elems[3];
			auto stype = elems[4];
			if (stype.size() >= 2) {
				// remove end line characters at the end of last elemenet.
				stype.erase(stype.size() - 2, 2);
			}

			map<string, int> supportedTypes = {
				{ "cv::Mat *", (int)CustomCommandId::OPENCV_READ_MAT_OBJECT},
				{ "const cv::Mat *", (int)CustomCommandId::OPENCV_READ_MAT_OBJECT},
				{ "cv::Rect_<int> *", (int)CustomCommandId::OPENCV_READ_CVRECT_OBJECT },
				{ "const cv::Rect_<int> *", (int)CustomCommandId::OPENCV_READ_CVRECT_OBJECT },
				{ "cv::Point_<int> *", (int)CustomCommandId::OPENCV_READ_CVPOINT_OBJECT },
				{ "const cv::Point_<int> *", (int)CustomCommandId::OPENCV_READ_CVPOINT_OBJECT },
				{ "std::vector<cv::Point_<int>,std::allocator<cv::Point_<int> > > *", (int)CustomCommandId::OPENCV_READ_CVCONTOUR },
				{ "const std::vector<cv::Point_<int>,std::allocator<cv::Point_<int> > > *", (int)CustomCommandId::OPENCV_READ_CVCONTOUR },
				{ "std::vector<std::vector<cv::Point_<int>,std::allocator<cv::Point_<int> > >,std::allocator<std::vector<cv::Point_<int>,std::allocator<cv::Point_<int> > > > > *", (int)CustomCommandId::OPENCV_READ_CVCONTOURS },
				{ "const std::vector<std::vector<cv::Point_<int>,std::allocator<cv::Point_<int> > >,std::allocator<std::vector<cv::Point_<int>,std::allocator<cv::Point_<int> > > > > *", (int)CustomCommandId::OPENCV_READ_CVCONTOURS },
				{ "cv::Point_<float> *", (int)CustomCommandId::OPENCV_READ_CVPOINT2F_OBJECT },
				{ "const cv::Point_<float> *", (int)CustomCommandId::OPENCV_READ_CVPOINT2F_OBJECT },
			};
			auto it = supportedTypes.find(stype);

			if (it != supportedTypes.end()) {
				int type = it->second;
				auto spaceIdx = value.find(' ');
				// there is must be a space between address and object information
				if (spaceIdx == string::npos) {
					cout << "unrecognize text" << std::endl;
					return;
				}

				// get the address in value variable and convert it into address
				address = ObjectInputerDlg::convertToAddress(value.substr(0, spaceIdx));
				// check if the string is a valid pointer
				if (address == nullptr) {
					cout << "invalid pointer" << std::endl;
					return;
				}
				cout << "parse text successfully" << std::endl;

				auto buzzWindow = activeWindow->getUserData<BuzzWindow>();
				auto newDrawingObject = buzzWindow->readObject(address, type);
				if (newDrawingObject) {
					newDrawingObject->setName(name);
					buzzWindow->addNewObject(newDrawingObject);
				}
			}
			else {
				cout << "unsupported object in clipboard" << std::endl;
			}
		}
	}
}

// This line tells Cinder to actually create the application
CINDER_APP( BuzzApp, RendererGl )