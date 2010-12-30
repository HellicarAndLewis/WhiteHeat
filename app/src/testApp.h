#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"
#include "ofxVideoGrabber.h"
#include "ofxControlPanel.h"

//#include "videoMotionExample.h"
//#include "backgroundSubtractionExample.h"
//#include "customDrawer.h"

#include "renderManager.h"

#include "Flock.h"

#include "ofxFft.h"
#include "Filter.h"

#include "AppSettings.h"

class testApp : public ofBaseApp{

	public:

		void setup();
		void update();
		void draw();
	
		void eventsIn(guiCallbackData & data);
		void cameraEventsIn(guiCallbackData & data);

		void calculateCaptureFramerate();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
	
		void randomiseGuiValueSafely(string whichGuiValue);
	
	void audioReceived 	(float * input, int bufferSize, int nChannels); 
	
	void drawContourFinder(float xPos, float yPos, float drawWidth, float drawHeight);
	
		//ofxFFT and Marek tuke
	
		Filter inputFilter;
		ofxFft *fft;
		float *fftOutput;
		float *fftOutputSmoothed;
	
		//ofxvideo grabber

		ofxVideoGrabber 		vidGrabber;

		int 				camWidth;
		int 				camHeight;
		int                 appWidth;
		int                 appHeight;

        /*framerate display*/
		char                buf[255];
		char                buf2[255];
		float 	            mytimeNow, mytimeThen, myframeRate;
		float 	            myfps;
		float                 myframes;
	
		ofxControlPanel gui;
		
		float elapsedTime;
		int appFrameCount;
		float appFrameRate;
		
		ofTrueTypeFont TTF;
		
	
		//warp speed
		ofxCvGrayscaleImage		videoGrayscaleCvImage;
		ofxCvGrayscaleImage		videoBgImage;
		
		ofxCvGrayscaleImage		videoGrayscaleCvImagePreWarp;
		ofPoint		srcPts[4];
		ofPoint		dstPts[4];
		
		
		ofxCvGrayscaleImage		videoDiffImage;
		ofxCvContourFinder		videoContourFinder;	
	
		//mouse in my house
	bool bShowMouse;	
	
		//for warping out with the right debug look
	ofxCvGrayscaleImage allWhiteImage;
	ofxCvGrayscaleImage allBlackImage;
	bool bShowWhiteRect;
	
		//FBO warping now
	renderManager rm;
	
	ofRectangle guiIn;
	ofRectangle guiOut;	
	
	Libdc1394Grabber* sdk;
	
	Flock f;
	
		//progressive relearn background
	ofxCvFloatImage videoFloatBgImage;
};

#endif
