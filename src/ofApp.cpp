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
    terrain.set(100,100,10,10,OF_PRIMITIVE_POINTS);
    terPtr = terrain.getMeshPtr();
    
    //CAMERA SETUP.
    restoreCamera();
    
    //LIGHTS SETUP.
    point1.setPointLight();
    
    //GUI SETUP.
    gui.setup();
    gui.add(amplitude.setup("Amplitude", 75, 0, 500));
    gui.add(size.setup("Size", 500, 10, 4000));
    gui.add(cols_rows.setup("Cols/Rows", 100, 1, 1000));
    gui.add(waterHeight.setup("Water Height", 25, 100, 0));
    gui.add(landHeight.setup("Land Height", -25, 0, -100));
    gui.add(iterationSpeed.setup("Speed", 0.001, 0.00001, 0.002));
    gui.add(iterateNoise.setup("Iterate", false));
    
    //DRAW MODE TOGGLE.
    drawMode.addListener(this, &ofApp::toggleDrawMode);
    gui.add(drawMode.setup("Toggle Draw Mode"));
    
    //LIGHT MODES.
    gui.add(lightPos.set("Light Position",ofVec3f(0,0,300), ofVec3f(-300,-300,-300), ofVec3f(300,300,300)));
    
    //COLOR MODES.
    gui.add(waterCol.setup("Water Color",ofColor(0, 0, 240), ofColor(0, 0), ofColor(255, 255)));
    gui.add(landCol.setup("Land Color",ofColor(0, 160, 0), ofColor(0, 0), ofColor(255, 255)));
    gui.add(peakCol.setup("Peak Color",ofColor(255, 255, 215), ofColor(0, 0), ofColor(255, 255)));
    
    //CAMERA CONTROL.
    restoreCam.addListener(this, &ofApp::restoreCamera); //reference function as event listener.
    gui.add(restoreCam.setup("Reset Camera Position"));
    
    //SAVE MESH.
    saveMesh.addListener(this, &ofApp::exportMesh);
    gui.add(saveMesh.setup("Export Mesh as .OBJ"));
    
    //use a custom value instead of innate frame num of the sketch - allows for "play and pause" style interaction.
    frameNum = 0;
}

//--------------------------------------------------------------
void ofApp::update(){
    //Update the light position and set direction.
    point1.setPosition(lightPos->x, lightPos->y, lightPos->z);
    point1.lookAt(terrain);
}

//--------------------------------------------------------------
void ofApp::draw(){
    //Turns on depth testing so rendering happens according to z-depth rather than draw order.
    ofEnableDepthTest();
    cam.begin();
    point1.enable();
    if(savingMesh == false){
    
    //DRAW ENTIRE MESH -------------------------------------
    
    if(iterateNoise){ frameNum ++; }
    
        //Calculate new z values for mesh.
        for(int i = 0; i < terPtr->getVertices().size(); i++) {
            terPtr->addColor(ofColor(255)); //add a colour value that we can then manipulate according to the z noise value.
            terPtr->getVertices()[i].z = (amplitude*noiseCoords(terPtr->getVertices()[i].x, terPtr->getVertices()[i].y, frameNum*iterationSpeed)) - 1.5*amplitude;
            //std::cout << "value: " << terPtr->getVertices()[i][2] << endl; //use this to check that vertices are being altered accordingly.
            if(terPtr->getVertices()[i].z > waterHeight) { //careful with operators here - z values increase away from the camera.
                terPtr->setColor(i, ofColor(waterCol));
            } else if(terPtr->getVertices()[i].z <= waterHeight && terPtr->getVertices()[i].z >= landHeight) {
                terPtr->setColor(i, ofColor(landCol));
            } else {
                terPtr->setColor(i, ofColor(peakCol));
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
    
    } else if (savingMesh == true){
        iterateNoise = false;
        
        //EXPORT TO OBJ FILE -----------------------------------
        
        string file_name = "test_" + ofGetTimestampString();
        ofFile obj(ofToDataPath(file_name + ".obj"), ofFile::WriteOnly); // creates a new file distinguishable by its time stamp.
        obj.create();
        obj.open("/Users/davidrollinson/Documents/Development/OF_ROOT/apps/myApps/terrain_generator/bin/data/" + file_name + ".obj", ofFile::WriteOnly);
        obj << "#vertices:\n";
            for(int i = 0 ; i < terPtr->getVertices().size(); i++) {
                obj << "v " << terPtr->getVertices()[i].x << " " << terPtr->getVertices()[i].y << " " << (amplitude*noiseCoords(terPtr->getVertices()[i].x, terPtr->getVertices()[i].y, frameNum*iterationSpeed)) - 1.5*amplitude << endl; //noise value must be used here in order to export without all z values reading as 0.
            }
            obj << "#faces:\n";
            for(int i = 0 ; i < terPtr->getIndices().size() / 3; i ++)
                obj << "f " << terPtr->getIndex(i*3) + 1 << " " << terPtr->getIndex(i*3+1) + 1 << " " << terPtr->getIndex(i*3+2) + 1 << endl; //objs start indexing at 1 so add 1 to all indices values.
            obj << "\n";
        obj.close();
        savingMesh = false;
        cout << "Saving Mesh - False " << endl;
    }

    cam.end();
//    point1.disable(); //this isn't necessary in this case.
    ofDisableDepthTest();
    
    glDisable(GL_LIGHTING); //Disable all lighting before drawing the gui.
    gui.draw();
    glEnable(GL_LIGHTING); //Re-enable before redrawing the canvas.
    
    /*TODO: Add shader functionality? */
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
    cam.setPosition(0, 0, -500);
    cam.lookAt(terrain);
}

//--------------------------------------------------------------
//TOGGLE DRAW MODE.
void ofApp::toggleDrawMode(){
    //iterate state.
    transition +=1;
    result = transition % 3;
    
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
}

//--------------------------------------------------------------
void ofApp::exportMesh(){
    savingMesh = true;
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
