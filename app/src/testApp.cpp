#include "testApp.h"

	//ofxfft and marek tuke

#define BUFFER_SIZE 2048
#define NUM_BARS 128.f
float fftSmoothing = 0.9;
float threshold = 0.04;
bool zoom = false;
float volume = 0;
float pitch = 0;

inline void spaceReplace(std::string & myStr)
{
	/* replace all spaces in the name to satisfy XML*/
	size_t found;
	found=myStr.find_first_of(" ");
	while (found!=string::npos)
	{
            //myStr.erase(found,1);
		myStr[found]='_';
		
		found=myStr.find_first_of(" ",found+1);
	}
}

//--------------------------------------------------------------
void testApp::setup(){
	
		// load the settings
	settings.loadSettings("app_settings.xml");
	
		//ofxFFT and marekt tuke demo
	ofSoundStreamSetup(0,1,this, 44100, BUFFER_SIZE, 1);	
	fft = ofxFft::create(BUFFER_SIZE, OF_FFT_WINDOW_BARTLETT);
	fftOutput = new float[fft->getBinSize()];
	fftOutputSmoothed = new float[fft->getBinSize()];
	memset(fftOutputSmoothed, 0, fft->getBinSize()*sizeof(float));
	
	inputFilter.q = 1;
	inputFilter.type = HI;
	inputFilter.calc_filter_coeffs(150);	
	
		//mouse house
	
	bShowMouse = false;
	
		//ofxVideoGrabber
	
	ofSetFrameRate(30);

	camWidth 		= 640;	// try to grab at this size.
	camHeight 		= 480;
	appWidth        = ofGetWidth();
	appHeight       = ofGetHeight();
	mytimeThen		= 0.0f;
	myframeRate     = 0.0f;
	myframes        = 0.0f;

	ofSetVerticalSync(true);
	ofSetLogLevel(OF_LOG_VERBOSE);

    vidGrabber.setDeviceID(0);


    sdk = new Libdc1394Grabber;
	//sdk->setFormat7(VID_FORMAT7_0);
	sdk->listDevices();
	sdk->setDiscardFrames(true);
	sdk->set1394bMode(false);
	//sdk->setROI(0,0,320,200);
	//sdk->setDeviceID("b09d01008bc69e:0");

	vidGrabber.setVerbose(true);
		//bool result = vidGrabber.initGrabber( camWidth, camHeight, VID_FORMAT_YUV422, VID_FORMAT_RGB, 30, true, sdk, settings );

    //bool result = vidGrabber.initGrabber( camWidth, camHeight, VID_FORMAT_YUV422, VID_FORMAT_RGB, 30 );
	// or like this:
	//Libsdk* dc1394Grabber = new Libsdk(); //stupid already above!
	bool result = vidGrabber.initGrabber( camWidth, camHeight, VID_FORMAT_GREYSCALE, VID_FORMAT_GREYSCALE, 30, true, sdk);
	// or like this:
	//bool result = vidGrabber.initGrabber( camWidth, camHeight, VID_FORMAT_YUV411, VID_FORMAT_RGB, 30, true, new Libsdk, new ofxIIDCSettings);



	if(result) {
	    ofLog(OF_LOG_VERBOSE,"Camera successfully initialized.");
	} else {
	    ofLog(OF_LOG_FATAL_ERROR,"Camera failed to initialize.");
	}
	
		//warping
	
	videoGrayscaleCvImage.allocate(camWidth, camHeight);
	videoBgImage.allocate(camWidth, camHeight);
	videoDiffImage.allocate(camWidth, camHeight);
	videoGrayscaleCvImagePreWarp.allocate(camWidth, camHeight);
	videoFloatBgImage.allocate(camWidth, camHeight);
		// set background color to be white: 
		//ofBackground(127, 127, 127);
	
	
	srcPts[0].set(0,0);
	srcPts[1].set(camWidth,0);
	srcPts[2].set(camWidth,camHeight);
	srcPts[3].set(0,camHeight);
	
	dstPts[0].set(0,0);
	dstPts[1].set(camWidth,0);
	dstPts[2].set(camWidth,camHeight);
	dstPts[3].set(0,camHeight);	
	
	
	
		//white image for warp debugging
	allWhiteImage.allocate(camWidth, camHeight);
	cvZero(allWhiteImage.getCvImage());
	allWhiteImage.invert();
		//black image for clearing
	allBlackImage.allocate(camWidth, camHeight);
	cvZero(allBlackImage.getCvImage());
	
		//FBO
	ofBackground(0,0,0);
    rm.allocateForNScreens(1, settings.PROJECTION_W, settings.PROJECTION_H);
    rm.loadFromXml("fboSettings.xml");
	
    guiIn   = ofRectangle(10, 40, 640, 480);
    guiOut  = ofRectangle(guiIn.x + guiIn.width + 30, 40, 640, 480);
	
	bShowWhiteRect = false;		

	
	//ofxControlPanel
	
	ofxControlPanel::setBackgroundColor(simpleColor(30, 30, 60, 200));
	ofxControlPanel::setTextColor(simpleColor(240, 50, 50, 255));
	
	gui.loadFont("MONACO.TTF", 8);		
	gui.setup("HANDL White Heat", 0, 0, settings.GUI_W, settings.GUI_H);
	gui.addPanel("Flock and Stats", 4, false);
	ofxControlPanel::setBackgroundColor(simpleColor(70, 70, 30, 200));	
	gui.addPanel("Warp", 4, false);
	ofxControlPanel::setBackgroundColor(simpleColor(70, 70, 30, 200));	
	gui.addPanel("Camera", 4, false);	
	ofxControlPanel::setBackgroundColor(simpleColor(30, 30, 30, 200));	
	
		//--------- PANEL 0 Flock and stats
	gui.setWhichPanel(0);
	
	gui.setWhichColumn(0);	
		//some dummy vars we will update to show the variable lister object
	elapsedTime		= ofGetElapsedTimef();
	appFrameCount	= ofGetFrameNum();	
	appFrameRate	= ofGetFrameRate();
	
	vector <guiVariablePointer> vars;
	vars.push_back( guiVariablePointer("elapsed time", &elapsedTime, GUI_VAR_FLOAT, 2) );
	vars.push_back( guiVariablePointer("elapsed frames", &appFrameCount, GUI_VAR_INT) );
	vars.push_back( guiVariablePointer("app fps", &appFrameRate, GUI_VAR_FLOAT, 2) );
	
	vars.push_back( guiVariablePointer("mouse x", &mouseX, GUI_VAR_INT) );
	vars.push_back( guiVariablePointer("mouse y", &mouseY, GUI_VAR_INT) );
	vars.push_back( guiVariablePointer("audio level", &volume, GUI_VAR_FLOAT, 2));
	
	gui.addVariableLister("app vars", vars);
	
	gui.addSlider("Randomise Audio Level", "FLOCK_RANDOM_AUDIO_LEVEL", 0.7f, 0.f, 1.f, false);
	
	gui.addChartPlotter("Audio Level", guiStatVarPointer("audio level", &volume, GUI_VAR_FLOAT, true, 2), 200, 100, 200, 0, 1);	
	
	gui.setWhichColumn(1);
	
	gui.addSlider("separation", "FLOCK_SEPARATION", 1.5f, -5.0f, 5.0f, false);
	gui.addSlider("alignment", "FLOCK_ALIGNMENT", 1.0f, -5.0f, 5.0f, false);	
	gui.addSlider("cohesion", "FLOCK_COHESION", 1.0f, -5.0f, 5.0f, false);	
	gui.addSlider("max speed", "FLOCK_MAX_SPEED", 4.0f, 0.1f, 20.f, false);
	gui.addSlider("max force", "FLOCK_MAX_FORCE", 0.1f, 0.01f, 1.f, false);
	
	gui.setWhichColumn(2);
	gui.addToggle("project background?", "WARP_B_PROJECT_BG", true);
	gui.addToggle("project outline?", "WARP_B_PROJECT_OUTLINE", false);
	gui.addToggle("randomise flock properties?", "FLOCK_RANDOMISE", false);
	
	//------- PANEL 1 	"Warp"
	gui.setWhichPanel(1);
	
	gui.setWhichColumn(0);
	gui.addSlider2D("pta", "WARP_CV_MANAGER_PANEL_VIDEO_PTA", 0, 0, 0, camWidth, 0, camHeight, true);
	gui.addSlider2D("ptd", "WARP_CV_MANAGER_PANEL_VIDEO_PTD", 0, camHeight, 0, camWidth, 0, camHeight, true);
	gui.addToggle("learn background ", "WARP_B_LEARN_BG", true);
	gui.addToggle("progressive learn?", "WARP_B_LEARN_BG_PROGRESSIVE", true);	
	
	gui.setWhichColumn(1);
	gui.addSlider2D("ptb", "WARP_CV_MANAGER_PANEL_VIDEO_PTB", camWidth, 0, 0, camWidth, 0, camHeight, true);
	gui.addSlider2D("ptc", "WARP_CV_MANAGER_PANEL_VIDEO_PTC", camWidth, camHeight, 0, camWidth, 0, camHeight, true);
	gui.addSlider("learn rate", "WARP_B_LEARN_BG_PROGRESSIVE_RATE", .1f, 0.0f, 1000.0f, false);			  
	gui.addSlider("threshold ", "WARP_THRESHOLD", 127, 0, 255, true);
	vector <string> warpDiffNames;
	warpDiffNames.push_back("abs diff");
	warpDiffNames.push_back("greater than");
	warpDiffNames.push_back("less than");
	gui.addTextDropDown("difference mode", "WARP_DIFF_MODE", 0, warpDiffNames);	
	
	gui.setWhichColumn(2);
	gui.addDrawableRect("grey pre warp", &videoGrayscaleCvImagePreWarp, 200, 150);
	gui.addDrawableRect("grey warped", &videoGrayscaleCvImage, 200, 150);
	gui.addDrawableRect("grey bg", &videoBgImage, 200, 150);	

	gui.setWhichColumn(3);
	gui.addDrawableRect("diff", &videoDiffImage, 200, 150);		
	gui.addDrawableRect("contour finder", &videoContourFinder, 200, 150);
	gui.addSlider("minimum blob size (% of view):", "WARP_MIN_BLOB_SIZE", 1.f, 0.01f, 10.0f, false);
	gui.addSlider("maximum blob size (% of view):", "WARP_MAX_BLOB_SIZE", 50.f, 1.f, 100.f, false);
		//guiTypeSlider * addSlider(string sliderName, string xmlName, float value , float min, float max, bool isInt);
	gui.addToggle("ignore nested blobs", "WARP_FIND_HOLES", false);	
	gui.addSlider("max num blobs to find ", "WARP_N_BLOBS_CONSIDERED", 3, 0, 20, true);
	
	//------- PANEL  2 	"Camera"
		
		//camera attempt
	
    sdk->getAllFeatureValues();
	gui.setWhichPanel(2);
    gui.setWhichColumn(0);

	int tmp_index = -1;
	int tmp_featureID = -1;
	bool whitebalance = false;
	int featureID = -1;

    /* setup feature gui sliders */
    for (int i=0; i < sdk->availableFeatureAmount; ++i)
    {
        if(sdk->featureVals[i].isPresent) {
            featureID = sdk->featureVals[i].feature;

            if( featureID == FEATURE_WHITE_BALANCE) {
                tmp_index = i;
                tmp_featureID = featureID;
                whitebalance = true;
            }
            else if(featureID == FEATURE_TRIGGER) {
                //not using any features above this
                break;
            }
            else {
                string name = sdk->featureVals[i].name;
                string xmlName = name;
                transform(xmlName.begin(), xmlName.end(), xmlName.begin(), ::toupper);

                float minVal = sdk->featureVals[i].minVal;
                float maxVal = sdk->featureVals[i].maxVal;
                float currVal = sdk->featureVals[i].currVal;
                gui.addButtonSlider(name,xmlName, currVal, minVal, maxVal, true);
            }
        }
    }

    /* setup white balance 2d slider */
    if(whitebalance)
    {
        string name = sdk->featureVals[tmp_index].name;
        string xmlName = name;
        transform(xmlName.begin(), xmlName.end(), xmlName.begin(), ::toupper);
        spaceReplace(xmlName);

        float minVal = sdk->featureVals[tmp_index].minVal;
        float maxVal = sdk->featureVals[tmp_index].maxVal;
        float currVal = sdk->featureVals[tmp_index].currVal;
        float currVal2 = sdk->featureVals[tmp_index].currVal2;
        gui.addSlider2D(name,xmlName, currVal, currVal2, minVal, maxVal, minVal, maxVal, false);

    }

    gui.setWhichColumn(1);

    /* setup features modes */
    for (int i=0; i < sdk->availableFeatureAmount; ++i)
    {
        if(sdk->featureVals[i].isPresent) {
            featureID = sdk->featureVals[i].feature;

            if (featureID == FEATURE_TRIGGER)
            {
                //not using any features above this
                break;
            }
            if( featureID != FEATURE_WHITE_BALANCE)
            {
                string name = sdk->featureVals[i].name + " mode";
                string xmlName = name;

                transform(xmlName.begin(), xmlName.end(), xmlName.begin(), ::toupper);
                spaceReplace(xmlName);

                int defaultval = FEATURE_MODE_MANUAL;

//                if(videoGrabber->featureVals[i].hasAutoModeActive) {
//                    defaultval = FEATURE_MODE_AUTO;
//					videoGrabber->setFeatureMode(FEATURE_MODE_AUTO, featureID );
//                } else {
					sdk->setFeatureMode(FEATURE_MODE_MANUAL, featureID );
//				}
                vector <string> modelist;
                modelist.push_back("MANUAL");


                if(sdk->featureVals[i].hasAutoMode)
                {
                    modelist.push_back("AUTO");
                    if(sdk->featureVals[i].hasOnePush )
                    {
                        modelist.push_back("ONE PUSH AUTO");
                    }
                }

                gui.addTextDropDown(name,xmlName, defaultval, modelist);
            }
        }
    }

    /* setup whitebalance modes */
    if(whitebalance)
    {
                featureID = FEATURE_WHITE_BALANCE;
                string name = sdk->featureVals[tmp_index].name + " mode";
                string xmlName = name;
                transform(xmlName.begin(), xmlName.end(), xmlName.begin(), ::toupper);
                spaceReplace(xmlName);

                int defaultval = FEATURE_MODE_MANUAL;
//                if(videoGrabber->featureVals[tmp_index].hasAutoModeActive) {
//                    defaultval = FEATURE_MODE_AUTO;
//					videoGrabber->setFeatureMode(FEATURE_MODE_AUTO, featureID );
//                } else {
					sdk->setFeatureMode(FEATURE_MODE_MANUAL, featureID );
//				}
                vector <string> modelist;
                modelist.push_back("MANUAL");

                if(sdk->featureVals[tmp_index].hasAutoMode)
                {
                    modelist.push_back("AUTO");
                    if(sdk->featureVals[tmp_index].hasOnePush )
                    {
                        modelist.push_back("ONE PUSH AUTO");
                    }
                }

                gui.addTextDropDown(name,xmlName, defaultval, modelist);

    }	
	
	gui.setWhichColumn(2);
	gui.addDrawableRect("Camera Image", &videoGrayscaleCvImagePreWarp, 200, 150);
	gui.addDrawableRect("Camera Warped", &videoGrayscaleCvImage, 200, 150);	
	
		//SETTINGS AND EVENTS
	
		//load from xml!
	gui.loadSettings("controlPanelSettings.xml");
	
		//if you want to use events call this after you have added all your gui elements
	gui.setupEvents();
	gui.enableEvents();
	
		//  -- SPECIFIC EVENTS -- this approach creates an event group and only sends you events for the elements you describe. 
		//	vector <string> list;
		//	list.push_back("FIELD_DRAW_SCALE");
		//	list.push_back("DIFF_MODE");
		//	gui.createEventGroup("TEST_GROUP", list);
		//	ofAddListener(gui.getEventGroup("TEST_GROUP"), this, &testApp::eventsIn);
	
		//  -- PANEL EVENTS -- this approach gives you back an ofEvent for only the events from panel 2, camera
	ofAddListener(gui.getEventsForPanel(2), this, &testApp::cameraEventsIn);
	
		//  -- this gives you back an ofEvent for all events in this control panel object
	ofAddListener(gui.guiEvent, this, &testApp::eventsIn);
	
	f.setup(settings.PROJECTION_W, settings.PROJECTION_H);
	
	for(int i=0; i<137; i++) { //should be 1337
		f.addBoid();
	}
}

//--------------------------------------------------------------
void testApp::update(){

	ofBackground(0,0,0);

	if(volume>threshold) {
			// find highest peak
		float max = 0;
		int maxI = -1;
		for(int i = 0; i < NUM_BARS; i++) {
			if(max<fftOutputSmoothed[i]) {
				max = fftOutputSmoothed[i];
				maxI = i;
			}
		}
		if(maxI>-1) {
			pitch = maxI;
		}
	}	
	
	vidGrabber.update();

	if (vidGrabber.isFrameNew()){
        calculateCaptureFramerate();
	}

    sprintf(buf,"App framerate : %f",ofGetFrameRate());

		//ofxControlPanel
		//some dummy vars we will update to show the variable lister object
	elapsedTime		= ofGetElapsedTimef();
	appFrameCount	= ofGetFrameNum();	
	appFrameRate	= ofGetFrameRate();

		//warper
	
	srcPts[0].set( gui.getValueI("WARP_CV_MANAGER_PANEL_VIDEO_PTA", 0), gui.getValueI("WARP_CV_MANAGER_PANEL_VIDEO_PTA", 1));
	srcPts[1].set( gui.getValueI("WARP_CV_MANAGER_PANEL_VIDEO_PTB", 0), gui.getValueI("WARP_CV_MANAGER_PANEL_VIDEO_PTB", 1));
	srcPts[2].set( gui.getValueI("WARP_CV_MANAGER_PANEL_VIDEO_PTC", 0), gui.getValueI("WARP_CV_MANAGER_PANEL_VIDEO_PTC", 1));
	srcPts[3].set( gui.getValueI("WARP_CV_MANAGER_PANEL_VIDEO_PTD", 0), gui.getValueI("WARP_CV_MANAGER_PANEL_VIDEO_PTD", 1));
	
	bool bLearnBg				= gui.getValueB("WARP_B_LEARN_BG");
	bool bLearnBgProgressive	= gui.getValueB("WARP_B_LEARN_BG_PROGRESSIVE");
	int threshold				= gui.getValueI("WARP_THRESHOLD");
	float minBlobSize			= (((float)gui.getValueI("WARP_MIN_BLOB_SIZE"))/100.f)*camWidth*camHeight;
	float maxBlobSize			= (((float)gui.getValueI("WARP_MAX_BLOB_SIZE"))/100.f)*camWidth*camHeight;
	int nBlobsConsidered		= gui.getValueI("WARP_N_BLOBS_CONSIDERED");
	bool bFindHoles				= gui.getValueB("WARP_FIND_HOLES");
	float rateOfProgression		= gui.getValueF("WARP_B_LEARN_BG_PROGRESSIVE_RATE") * .00001; //progressive from opentsps
	int warpDiffMode			= gui.getValueI("WARP_DIFF_MODE");
	
	if (vidGrabber.isFrameNew()){
		
		//videoColorCvImage.setFromPixels(video.getPixels(), width, height);
		
		videoGrayscaleCvImagePreWarp.setFromPixels(vidGrabber.getPixels(), camWidth, camHeight);
		
		videoGrayscaleCvImage.warpIntoMe(videoGrayscaleCvImagePreWarp, srcPts, dstPts);
		
		if (ofGetElapsedTimef() < 1.5f){
			videoBgImage = videoGrayscaleCvImage;
		}
		
		if (bLearnBg){ 
			videoBgImage = videoGrayscaleCvImage;
			gui.setValueB("WARP_B_LEARN_BG", false);
		}
		
		if (bLearnBgProgressive){
			if (bLearnBg){
				videoFloatBgImage = videoBgImage;
			}
			
			videoFloatBgImage.addWeighted(videoGrayscaleCvImage, rateOfProgression);
			videoBgImage = videoFloatBgImage;		
		}
		
		if( warpDiffMode == 0 ){ //abs
			videoDiffImage.absDiff(videoGrayscaleCvImage, videoBgImage);
		}else if( warpDiffMode == 1 ){ //greater
			videoDiffImage = videoGrayscaleCvImage;
			videoDiffImage -= videoBgImage;
		}else if( warpDiffMode == 2 ){ //less
			videoDiffImage = videoBgImage;
			videoDiffImage -= videoGrayscaleCvImage;	
		}	
		
		
		videoDiffImage.threshold(threshold);
		
		videoContourFinder.findContours(videoDiffImage, minBlobSize, maxBlobSize, nBlobsConsidered, bFindHoles, true);
	}	
	
		//attract to blob centres and alter the controls of the simulation
	
	
		//on button press put what about on clap?
	
	
	if(gui.getValueB("FLOCK_RANDOMISE") || (volume > gui.getValueF("FLOCK_RANDOM_AUDIO_LEVEL"))){
			//then we should randomise all those lovely values
		randomiseGuiValueSafely("FLOCK_SEPARATION");
		randomiseGuiValueSafely("FLOCK_ALIGNMENT");
		randomiseGuiValueSafely("FLOCK_COHESION");
		randomiseGuiValueSafely("FLOCK_MAX_SPEED");
		randomiseGuiValueSafely("FLOCK_MAX_FORCE");
		
		gui.setValueB("FLOCK_RANDOMISE", false); //turn it off so its not random all the time
	}
	
	float sep = gui.getValueF("FLOCK_SEPARATION");
	float ali = gui.getValueF("FLOCK_ALIGNMENT");	
	float coh = gui.getValueF("FLOCK_COHESION");
	float newMaxSpeed = gui.getValueF("FLOCK_MAX_SPEED");
	float newMaxForce = gui.getValueF("FLOCK_MAX_FORCE");
	
	f.update(&videoContourFinder, sep, ali, coh, newMaxSpeed, newMaxForce);
	
	gui.update();
}

//--------------------------------------------------------------
void testApp::draw(){
    ofSetColor(255, 255, 255, 255);
	
	bool bProjectBackground = gui.getValueB("WARP_B_PROJECT_BG");
	bool bProjectOutline	= gui.getValueB("WARP_B_PROJECT_OUTLINE");
	
    rm.startOffscreenDraw();
	
	if(bShowWhiteRect){
		allWhiteImage.draw(0,0,settings.PROJECTION_W,settings.PROJECTION_H);
	}else {
		allBlackImage.draw(0,0,settings.PROJECTION_W,settings.PROJECTION_H); //hacktastic
		if (bProjectBackground) {
			videoBgImage.draw(0, 0, settings.PROJECTION_W, settings.PROJECTION_H);
		}
		if (bProjectOutline) {
				//videoContourFinder.draw(0, 0, 1024, 768);
			
			drawContourFinder(0,0,settings.PROJECTION_W,settings.PROJECTION_H);
		}		
		
//		videoDiffImage.draw(0, 0,1024,768);
		f.draw(1.f-volume); //fun fun fun!mail
			//videoContourFinder.draw(0,0,1024,768);	
	}
	
	
    rm.endOffscreenDraw();
	
    ofSetColor(255, 255, 255, 255);
	
    rm.drawInputDiagnostically(guiIn.x, guiIn.y, guiIn.width, guiIn.height);
    rm.drawOutputDiagnostically(guiOut.x, guiOut.y, guiOut.width, guiOut.height);
	
		//    glPushMatrix();
		//	glTranslatef(1440, 0, 0);
		//	ofSetColor(0, 0, 0, 255);
		//	ofRect(0, 0, 1024, 768);
		//	ofSetColor(255, 255, 255, 255);
		//	rm.drawScreen(0);
		//		//rm.drawScreen(1);
		//    glPopMatrix();
    glPushMatrix();
	glTranslatef(10, guiIn.y+guiIn.height + 30, 0);
	glScalef(0.5f,0.5f,1.f);
	ofSetColor(255, 255, 255, 255);
	rm.drawScreen(0);
    glPopMatrix();	
	
    glPushMatrix();
	glTranslatef(settings.GUI_W, 0, 0);
	ofSetColor(255, 255, 255, 255);
	rm.drawScreen(0);
    glPopMatrix();	
	
	
    ofDrawBitmapString("internal texture points", 10, 228);
    ofDrawBitmapString("texture warping points", 535, 228);
	
    ofDrawBitmapString("screen 1", 10, 290);
		//ofDrawBitmapString("screen 2", 710, 290);
	
    ofDrawBitmapString("s - to save to xml   l - to reload from xml    r - reset coordinates\n", 10, 16);
	
    /* Framerate display */
	ofDrawBitmapString(buf,30,appHeight - 40);
	ofDrawBitmapString(buf2,30,appHeight - 20);
	
	ofSetColor(0xffffff);
	gui.draw();	
}

//--------------------------------------------------------------
void testApp::calculateCaptureFramerate()
{
    mytimeNow = ofGetElapsedTimef();
    if( (mytimeNow-mytimeThen) > 0.05f || myframes == 0 ) {
        myfps = myframes / (mytimeNow-mytimeThen);
        mytimeThen = mytimeNow;
        myframes = 0;
        myframeRate = 0.9f * myframeRate + 0.1f * myfps;
        sprintf(buf2,"Capture framerate : %f",myframeRate);
    }
    myframes++;
}

	//--------------------------------------------------------------
void testApp::eventsIn(guiCallbackData & data){
	
//		//lets send all events to our logger
//	if( data.groupName != "events logger"){
//		string logStr = data.groupName;
//		
//		if( data.fVal.size() ){
//			for(int i = 0; i < data.fVal.size(); i++) logStr += " - "+ofToString(data.fVal[i], 4);
//		}
//		if( data.iVal.size() ){
//			for(int i = 0; i < data.iVal.size(); i++) logStr += " - "+ofToString(data.iVal[i]);
//		}	
//		if( data.sVal.size() ){
//			for(int i = 0; i < data.sVal.size(); i++) logStr += " - "+data.sVal[i];
//		}
//		
//		logger.log(OF_LOG_NOTICE, "event - %s", logStr.c_str());
//	}
	
		// print to terminal if you want to 
		//this code prints out the name of the events coming in and all the variables passed
//	printf("testApp::eventsIn - name is %s - \n", data.groupName.c_str());
//	if( data.elementName != "" ){
//		printf(" element name is %s \n", data.elementName.c_str());
//	}
//	if( data.fVal.size() ){
//		for(int i = 0; i < data.fVal.size(); i++){
//			printf(" float value [%i] = %f \n", i, data.fVal[i]);
//		}
//	}
//	if( data.iVal.size() ){
//		for(int i = 0; i < data.iVal.size(); i++){
//			printf(" int value [%i] = %i \n", i, data.iVal[i]);
//		}
//	}	
//	if( data.sVal.size() ){
//		for(int i = 0; i < data.sVal.size(); i++){
//			printf(" string value [%i] = %s \n", i, data.sVal[i].c_str());
//		}
//	}
//	printf("\n");
}

	//--------------------------------------------------------------
void testApp::cameraEventsIn(guiCallbackData & data){
	
	cout << "Camera data, group name:" << data.groupName << ", element name: " << data.elementName << endl; 

	string str_modePattern = "_MODE";
	size_t found;
	
	found = data.groupName.find(str_modePattern);
	
	if(found!=string::npos) //if "_MODE" does exist in the string
	{
		
		cout << "_MODE Detected, not dealt with yet." << endl;
		//sdk->setFeatureMode(param_mode, param_id);
	}
	else {
		if(data.groupName == "WHITE_BALANCE") {
			sdk->setFeatureValue(data.fVal[0], data.fVal[1], Libdc1394GrabberFeatureHelper::libdcStringToAVidFeature(data.groupName));
		}
		else {
			sdk->setFeatureValue(data.fVal[0], Libdc1394GrabberFeatureHelper::libdcStringToAVidFeature(data.groupName));
		}
	}	
	
	
		//lets send all events to our logger
//	if( data.groupName != "events logger"){
//		string logStr = data.groupName;
//		
//		if( data.fVal.size() ){
//			for(int i = 0; i < data.fVal.size(); i++) logStr += " - "+ofToString(data.fVal[i], 4);
//		}
//		if( data.iVal.size() ){
//			for(int i = 0; i < data.iVal.size(); i++) logStr += " - "+ofToString(data.iVal[i]);
//		}	
//		if( data.sVal.size() ){
//			for(int i = 0; i < data.sVal.size(); i++) logStr += " - "+data.sVal[i];
//		}
//		
//		logger.log(OF_LOG_NOTICE, "cameraEvent - %s", logStr.c_str());
//	}
	
		// print to terminal if you want to 
		//this code prints out the name of the events coming in and all the variables passed
//	printf("testApp::cameraEventsIn - name is %s - \n", data.groupName.c_str());
//	if( data.elementName != "" ){
//		printf(" element name is %s \n", data.elementName.c_str());
//	}
//	if( data.fVal.size() ){
//		for(int i = 0; i < data.fVal.size(); i++){
//			printf(" float value [%i] = %f \n", i, data.fVal[i]);
//		}
//	}
//	if( data.iVal.size() ){
//		for(int i = 0; i < data.iVal.size(); i++){
//			printf(" int value [%i] = %i \n", i, data.iVal[i]);
//		}
//	}	
//	if( data.sVal.size() ){
//		for(int i = 0; i < data.sVal.size(); i++){
//			printf(" string value [%i] = %s \n", i, data.sVal[i].c_str());
//		}
//	}
//	printf("\n");
}

//--------------------------------------------------------------
void testApp::keyPressed  (int key){
	switch (key){
		case 'f':{
			ofToggleFullscreen();
			break;				
		}
		case 'h':{
			gui.toggleView();	
			break;
		}
				//		case 's':{
				//			grabber.videoSettings();				//from ofxcontrolpanel, need to port these parts..
				//													//if you need to adjust the camera properties
				//			break;
				//		}	
		case 'm':{
			if(bShowMouse){
				ofHideCursor();
				bShowMouse = false;
			}else {
				ofShowCursor();
				bShowMouse = true;
			} 
			break;				
		} 		
		case 'w':{
			bShowWhiteRect = !bShowWhiteRect;	
			break;
		}
		case 's':{
			rm.saveToXml();	
			break;
		}
		case 'l':{
			rm.reloadFromXml();
			break;
		}
		case 'r':{
			rm.resetCoordinates();
			break;
		}					
		default:{
			break;
		}
	}
}


//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

	//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	if(!gui.hidden){
		gui.mouseDragged(x, y, button);		
	}else{
		rm.mouseDragInputPoint(guiIn, ofPoint(x, y));
		rm.mouseDragOutputPoint(guiOut, ofPoint( x, y));
	}
}

	//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	if(!gui.hidden){
		gui.mousePressed(x, y, button);		
	}else{
		if( !rm.mouseSelectInputPoint(guiIn, ofPoint(x, y)) ){
			rm.mouseSelectOutputPoint(guiOut, ofPoint( x,  y));
		}
	}	
}

	//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
	if(!gui.hidden){
		gui.mouseReleased();		
	}	
}

	//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
	
}

//void testApp::parameterCallback(float param1, float param2, int param_mode, int param_id)
//{
//
//    if(param_mode != NULL_MODE)
//    {
//        //cout << "set mode called: "  << param_mode << " param_id = " << param_id << endl;
//        vidGrabber.setFeatureMode(param_mode, param_id);
//    }
//    else {
//        if(param_id == FEATURE_WHITE_BALANCE) {
//            vidGrabber.setFeatureValue(param1, param2, param_id);
//        }
//        else {
//            //cout << "set feature called: " << param1 << " " << param_id << endl;
//           vidGrabber.setFeatureValue(param1, param_id);
//        }
//    }
//
//}
	   
void testApp::randomiseGuiValueSafely(string whichGuiValue){
	float maxValue = gui.getMaxOfValueF(whichGuiValue);
	float minValue = gui.getMinOfValueF(whichGuiValue);
	
	float newValue = ofRandom(minValue, maxValue);
	
	gui.setValueF(whichGuiValue, newValue);
}

	//--------------------------------------------------------------
void testApp::audioReceived(float * input, int bufferSize, int nChannels){	
	
	inputFilter.filter(input, bufferSize);
	inputFilter.filter(input, bufferSize);
	
	fft->setSignal(input);
	fftOutput = fft->getAmplitude();
	int binSize = fft->getBinSize();
	
	for(int i = 0; i < binSize; i++) {
		if(fftOutput[i]<threshold) {
			fftOutput[i] = 0;
		}
		fftOutputSmoothed[i] = fftOutputSmoothed[i]*(fftSmoothing) + (1.f-fftSmoothing)*fftOutput[i];
	}
	
	for(int i = 0; i < bufferSize; i++) {
		if(input[i]>volume) {
			volume = input[i];
		} else {
			volume *= 0.9999;
		}
	}
}

void testApp::drawContourFinder(float xPos, float yPos, float drawWidth, float drawHeight){
		
	float scalex = 0.0f;
	float scaley = 0.0f;
		
	float contourWidth = videoContourFinder.getWidth();
	float contourHeight = videoContourFinder.getHeight();
		
	if( contourWidth != 0 ) { scalex = drawWidth/contourWidth; } else { scalex = 1.0f; }
	if( contourHeight != 0 ) { scaley = drawHeight/contourHeight; } else { scaley = 1.0f; }
	
		//	if(videoContourFinder.bAnchorIsPct){ //no anchoiring crap
//		xPos -= anchor.x * drawWidth;
//		yPos -= anchor.y * drawHeight;
//	}else{
//		xPos -= anchor.x;
//		yPos -= anchor.y;
		//	}
	
		// ---------------------------- draw the bounding rectangle
		//ofSetColor(0xDD00CC);
		//    ofSetColor(255,255,255, 192);
	glPushMatrix();
    glTranslatef( xPos, yPos, 0.0 );
    glScalef( scalex, scaley, 0.0 );
		//
		//	ofNoFill();
		//	for( int i=0; i<(int)blobs.size(); i++ ) {
		//		ofRect( blobs[i].boundingRect.x, blobs[i].boundingRect.y,
		//                blobs[i].boundingRect.width, blobs[i].boundingRect.height );
		//	}
	
		// ---------------------------- draw the blobs
		//ofSetColor(0x00FFFF);
	
	ofEnableAlphaBlending();
	
	ofSetColor(255,255,255, 192);
	
	ofFill();
	
	for( int i=0; i<(int)videoContourFinder.blobs.size(); i++ ) {

		ofBeginShape();
		for( int j=0; j<videoContourFinder.blobs[i].nPts; j++ ) {
			ofVertex( videoContourFinder.blobs[i].pts[j].x, videoContourFinder.blobs[i].pts[j].y );
		}
		ofEndShape();
		
	}
	glPopMatrix();
	
	ofNoFill();
	ofDisableAlphaBlending();
}