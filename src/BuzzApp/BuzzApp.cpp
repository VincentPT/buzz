#include "BuzzApp.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/Rand.h"
#include <list>
#include "BuzzWindow.h"

using namespace ci;
using namespace ci::app;
using namespace std;

BuzzApp* g_instance = nullptr;
BuzzApp::BuzzApp() {
	g_instance = this;
}
BuzzApp::~BuzzApp() {
	g_instance = nullptr;
}

BuzzApp* BuzzApp::getInstance() {
	return g_instance;
}

void BuzzApp::setup()
{
	WindowRef defaultNativeWindow = getWindow();

	auto firstWindow = new BuzzWindow(defaultNativeWindow);
	firstWindow->setTitle("buzz");
	
	//FILE* fp = NULL;
	//fopen_s(&fp, "img.dat", "rb");
	//if (fp != NULL) {
	//	fseek(fp, 0, SEEK_END);
	//	auto fileSize = ftell(fp);
	//	fseek(fp, 0, SEEK_SET);

	//	uint8_t* data = (uint8_t*) malloc(fileSize);
	//	fread_s(data, fileSize, 1, fileSize, fp);

	//	Surface mySurface(data, 717, 478, 2151, cinder::SurfaceChannelOrder::BGR);
	//	firstWindowData->_tex = gl::Texture2d::create(mySurface);

	//	fclose(fp);

	//	free(data);
	//}

	//createNewWindow();
}

void BuzzApp::createNewWindow()
{
	//auto secondWindowData = new WindowData;
	//app::WindowRef newWindow = createWindow( Window::Format().size( 400, 400 ) );
	//newWindow->setUserData(secondWindowData);

	//secondWindowData->_tex = gl::Texture2d::create(loadImage("C:\\Users\\minh\\Pictures\\sample1_l.jpg"));
	//
	//// for demonstration purposes, we'll connect a lambda unique to this window which fires on close
	//int uniqueId = getNumWindows();
	//newWindow->getSignalClose().connect(
	//		[uniqueId,this] { this->console() << "You closed window #" << uniqueId << std::endl; }
	//	);
	
	auto newWindow = new BuzzWindow("buzz#" + std::to_string(getNumWindows()), 960, 540);
}

void BuzzApp::mouseDrag( MouseEvent event )
{
}

void BuzzApp::keyDown( KeyEvent event )
{
	if( event.getChar() == 'f' )
		setFullScreen( ! isFullScreen() );
	else if( event.getChar() == 'w' )
		createNewWindow();
}

void BuzzApp::draw()
{
	//gl::clear( Color( 0.1f, 0.1f, 0.15f ) );
	gl::clear(Color(1.0f, 1.0f, 1.0f));
	
	/*if (data->_tex) {
		gl::draw(data->_tex);
	}*/

	/*gl::color( data->mColor );	
	gl::begin( GL_LINE_STRIP );
	for( auto pointIter = data->mPoints.begin(); pointIter != data->mPoints.end(); ++pointIter ) {
		gl::vertex( *pointIter );
	}
	gl::end();*/
}

// This line tells Cinder to actually create the application
CINDER_APP( BuzzApp, RendererGl )