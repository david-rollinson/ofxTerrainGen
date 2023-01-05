#pragma once

#include "ofMain.h"
//INCLUDE RELEVANT LIBRARIES.
#include "ofxGui.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
    //CUSTOM METHODS.
    float noiseCoords(float _x, float _y, float _z);
    void restoreCamera();
    void toggleDrawMode();
    void exportMesh();
		
    //DECLARE PLANE
    ofPlanePrimitive terrain;
    ofMesh * terPtr;
    
    //CAMERA AND GUI --------------
    
    ofEasyCam cam;
    ofxPanel gui;
    
    ofxToggle iterateNoise;
    ofxIntSlider cols_rows;
    ofxFloatSlider size;
    ofxFloatSlider iterationSpeed;
    
    //AMPLITUDES.
    ofxFloatSlider amplitude;
    ofxFloatSlider waterHeight;
    ofxFloatSlider landHeight;
    
    //COLOURS.
    ofxColorSlider waterCol;
    ofxColorSlider landCol;
    ofxColorSlider peakCol;
    
    //TOGGLES.
    ofxButton restoreCam;
    ofxButton drawMode;
    ofxButton saveMesh;
    bool savingMesh = false;
    
    //LIGHTS.
    ofLight point1;
    ofParameter <ofVec3f> lightPos;
    
    //----------------------------
    
    int frameNum;
    
    //DECLARE DRAW STATE MACHINE
    typedef enum {
    wireFrame,
    solidFill,
    verticesOnly
    } State;
    State drawType;
    int transition = 0;
    int result = 0;
};
