#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetWindowTitle("Terrain Generator");
    //RENDER PRELIMS.
    ofBackground(0,0,0);
    ofDisableAlphaBlending();
    ofDisableArbTex();
    
    //TEXTURE SETUP.
    //disable arbtrary textures, then load in any necessary textures to wrap to mesh.
    ofDisableArbTex();
    //ofLoadImage(textureVariable, "textureInputFile.jpg");
    
    //MESH SETUP.
    ofSetColor(255, 0, 0);
    terrain.set(100,100,10,10,OF_PRIMITIVE_TRIANGLE_STRIP);
    cam.lookAt(terrain);
    
    //NOISE SETUP.
    //set noise properties for both x and y.
    
    //GUI SETUP.
    gui.setup();
    gui.add(amplitude.setup("amplitude", 20, 0, 50));
    gui.add(cols_rows.setup("Cols/Rows", 10, 1, 100));
    gui.add(size.setup("size", 10, 10, 400));
    gui.add(iterateNoise.setup("iterate", true));
    
    //use a custom value instead of innate frame num of the sketch - allows for "play and pause" style interaction.
    frameNum = 0;
}

//--------------------------------------------------------------
void ofApp::update(){
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    //draw mesh
    gui.draw();
    //Turns on depth testing so rendering happens according to z-depth rather than draw order.
    ofEnableDepthTest();
    cam.begin();
    terrain.drawWireframe();
    terrain.setResolution(cols_rows, cols_rows);
    terrain.set(size,size);
    //use the mesh pointer so we don't have to copy any values.
    ofMesh * terPtr = terrain.getMeshPtr();
    if(iterateNoise){
        frameNum ++;
    }
    for(int i = 0; i < terPtr->getVertices().size(); i++) {
        terPtr->getVertices()[i].z = amplitude*noiseCoords(terPtr->getVertices()[i].x, terPtr->getVertices()[i].y, frameNum*0.01);
        //std::cout << "value: " << terPtr->getVertices()[i].x << endl; //use this to check that vertices are being altered accordingly.
    }

    cam.end();
    ofDisableDepthTest();
    
    //TODO: add lighting with gui interaction.
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
