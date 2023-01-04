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
    terPtr = terrain.getMeshPtr();
    
    //CAMERA SETUP.
    cam.setPosition(0, 0, -100);
    cam.lookAt(terrain);
    
    //GUI SETUP.
    gui.setup();
    gui.add(amplitude.setup("Amplitude", 20, 0, 500));
    gui.add(cols_rows.setup("Cols/Rows", 10, 1, 1000));
    gui.add(size.setup("Size", 100, 10, 4000));
    gui.add(iterateNoise.setup("Iterate", false));
    gui.add(iterationSpeed.setup("Speed", 0.001, 0.00001, 0.002));
    gui.add(waterHeight.setup("Water Height", 0, -1, 0));
    gui.add(landHeight.setup("Land Height", 1, 1, 0));
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
    
    if(iterateNoise){
        frameNum ++;
    }
    
    //Calculate new z values for mesh.
    for(int i = 0; i < terPtr->getVertices().size(); i++) {
        terPtr->addColor(ofColor(255)); //add a colour value that we can then manipulate according to the z noise value.
        terPtr->getVertices()[i].z = (amplitude*noiseCoords(terPtr->getVertices()[i].x, terPtr->getVertices()[i].y, frameNum*iterationSpeed)) - 1.5*amplitude;
        //std::cout << "value: " << terPtr->getVertices()[i].x << endl; //use this to check that vertices are being altered accordingly.
        if(terPtr->getVertices()[i].z > waterHeight) { //careful with operators here - z values increase away from the camera.
            terPtr->setColor(i, ofFloatColor(0,0,255));
        } else if(terPtr->getVertices()[i].z <= waterHeight) {
            terPtr->setColor(i, ofFloatColor(0,255,0));
        }
    }
    
    //Set draw type, size and resolution.
    switch(drawType) {
        case wireFrame:
            terrain.drawWireframe();
            break;
        case solidFill:
            terrain.draw();
            break;
        case verticesOnly:
            terrain.drawVertices();
            break;
    }
    terrain.setResolution(cols_rows, cols_rows);
    terrain.set(size,size);

    cam.end();
    ofDisableDepthTest();
    gui.draw();
    /*TODO: Add lighting with gui interaction.
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
    float n3_freq = 0.005;
    float n3_amp = 40;
    
    //declare scoped noise variables.
    float n1 = ofNoise(_x * n1_freq,_y * n1_freq,_z * n1_amp);
    float n2 = ofNoise(_x * n2_freq,_y * n2_freq,_z * n2_amp);
    float n3 = ofNoise(_x * n3_freq,_y * n3_freq,_z * n3_amp);
    
    return n1+n2+n3;
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
    switch(result){
        case 0:
            drawType = wireFrame;
            break;
        case 1:
            drawType = solidFill;
            break;
        case 2:
            drawType = verticesOnly;
            break;
    }
    transition +=1;
    result = transition % 3;
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
