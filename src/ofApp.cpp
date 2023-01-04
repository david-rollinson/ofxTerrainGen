#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    //RENDER PRELIMS.
    ofSetWindowTitle("Terrain Generator");
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
    
    //CAMERA SETUP.
    cam.setPosition(0, 0, -100);
    cam.lookAt(terrain);
    
    //GUI SETUP.
    gui.setup();
    gui.add(amplitude.setup("Amplitude", 20, 0, 50));
    gui.add(cols_rows.setup("Cols/Rows", 10, 1, 1000));
    gui.add(size.setup("Size", 100, 10, 4000));
    gui.add(iterateNoise.setup("Iterate", true));
    //BUTTON SETUP.
    restoreCam.addListener(this, &ofApp::restoreCamera); //reference function as event listener.
    gui.add(restoreCam.setup("Reset Camera Position"));
    
    drawMode.addListener(this, &ofApp::toggleDrawMode);
    gui.add(drawMode.setup("Toggle Draw Mode"));
    
    //use a custom value instead of innate frame num of the sketch - allows for "play and pause" style interaction.
    frameNum = 0;
}

//--------------------------------------------------------------
void ofApp::update(){
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    //Turns on depth testing so rendering happens according to z-depth rather than draw order.
    ofEnableDepthTest();
    cam.begin();
    
    //Set draw type.
    switch(drawType) {
        case wireFrame:
            terrain.drawWireframe();
            break;
        case solidFill:
            terrain.draw();
            break;
    }
    
    terrain.setResolution(cols_rows, cols_rows);
    terrain.set(size,size);
    //use the mesh pointer so we don't have to copy any values.
    ofMesh * terPtr = terrain.getMeshPtr();
    if(iterateNoise){
        frameNum ++;
    }
    for(int i = 0; i < terPtr->getVertices().size(); i++) {
        terPtr->getVertices()[i].z = amplitude*noiseCoords(terPtr->getVertices()[i].x, terPtr->getVertices()[i].y, frameNum*0.001);
        //std::cout << "value: " << terPtr->getVertices()[i].x << endl; //use this to check that vertices are being altered accordingly.
    }

    cam.end();
    ofDisableDepthTest();
    gui.draw();
    /*TODO: Add lighting with gui interaction. Add mechanic whereby color is manipulated on the basis of height.
      Allow export to obj file. Add shader functionality? */
}

//--------------------------------------------------------------
//GATHER NOISE VALS.
float ofApp::noiseCoords(float _x, float _y, float _z){
    
    //NOISE SETUP.
    //set noise properties.
    float n1_freq = 0.02;
    float n1_amp = 10;
    float n2_freq = 0.01;
    float n2_amp = 30;
    
    //declare scoped noise variables.
    float n1 = ofNoise(_x * n1_freq,_y * n1_freq,_z * n1_amp);
    float n2 = ofNoise(_x * n2_freq,_y * n2_freq,_z * n2_amp);
    
    return n1+n2;
}

//--------------------------------------------------------------
//RESTORE CAMERA COORDINATES.
void ofApp::restoreCamera(){
    cam.setPosition(0, 0, -100);
    cam.lookAt(terrain);
}

//--------------------------------------------------------------
//TOGGLE DRAW MODE.
void ofApp::toggleDrawMode(){
    //transition state here.
    if(drawType == wireFrame) {
        drawType = solidFill;
    } else if (drawType == solidFill){
        drawType = wireFrame;
    }
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
