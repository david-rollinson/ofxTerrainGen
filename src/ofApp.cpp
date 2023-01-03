#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(0,0,0);

    //Turns on depth testing so rendering happens according to z-depth rather than draw order.
    ofEnableDepthTest();
    
    //disable arbtrary textures, then load in any necessary textures to wrap to mesh.
    ofDisableArbTex();
    //ofLoadImage(textureVariable, "textureInputFile.jpg");
    
    ofSetColor(255, 0, 0);
    terrain.set(100,100,10,10,OF_PRIMITIVE_TRIANGLE_STRIP);
    cam.lookAt(terrain);
    //set noise properties for both x and y.
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    //draw mesh
    cam.begin();
    terrain.drawWireframe();
    ofMesh * terPtr = terrain.getMeshPtr();
    for(int i = 0; i < terPtr->getVertices().size(); i++) {
        terPtr->getVertices()[i].z += noiseCoords(terPtr->getVertices()[i].x, terPtr->getVertices()[i].y, ofGetFrameNum());
        std::cout << "value: " << terPtr->getVertices()[i].x << endl;
    }
    cam.end();
}

//--------------------------------------------------------------
//GATHER NOISE VALS.
float ofApp::noiseCoords(float _x, float _y, float _z){
    return ofNoise(_x,_y,_z);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
