#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    ofxJSONElement config;
    config.open("config.json");
    
    int olaPort = config["olaPort"].asInt();
    int numUniv = config["numUniv"].asInt();
    numLights = config["numLights"].asInt();
    string mapCsvFile = config["mapCsvFile"].asString();
    
    olaSender.setup(olaPort, 1);
    
    csv.load(mapCsvFile);
    
    grabber.setup(640, 480);
    thresholded.allocate(320,240, OF_IMAGE_COLOR);
    
    gui.setup();
    gui.add(FPS.set("FPS"));
    gui.add(bStartMapping.set("start mapping", false));
    gui.add(bIsMapping.set("is mapping", false));
    gui.add(currLight.set("current light", 0, 0, numLights));
    gui.add(bSaveToCsv.set("save csv", true));
    gui.add(bgThresh.set("bgthresh", 50, 0, 255));
    gui.add(bResetThresh.set("reset thresh", false));
    
}

//--------------------------------------------------------------
void ofApp::update(){

    FPS = ofToString((int)ofGetFrameRate());
    
    //start the mapping process
    if(bStartMapping){
        background.reset();
        bIsMapping = true;
        currentLight = 0;
        bStartMapping = false;
        points.clear();
    }
    
    if(currentLight > numLights) {
        bStartMapping = false;
        bIsMapping = false;
        saveToCsv();
//        currentLight = 0;
//        currentLight = 0;
    }
    
    currLight = currentLight;

    
    grabber.update();
    if(bResetThresh) {
        background.reset();
        bResetThresh = false;
    }
    background.setThresholdValue(bgThresh);
    background.update(grabber.getPixels(), thresholded);
    thresholded.update();
    contourFinder.setMinAreaRadius(1);
    contourFinder.setMaxAreaRadius(50);
    contourFinder.findContours(thresholded);
    
    
    if(bIsMapping){
        if(grabber.isFrameNew()){
            for(int i = 1; i <= numLights; i++){
                
                if(i == currentLight){
                    olaSender.setChannel(1,(i*3-2),(int)ofRandom(255));
                    olaSender.setChannel(1,(i*3-1), (int)ofRandom(20));
                    olaSender.setChannel(1,(i*3-0),(int)ofRandom(100));
                }
                else{
                    olaSender.setChannel(1,(i*3-2),0);
                    olaSender.setChannel(1,(i*3-1),0);
                    olaSender.setChannel(1,(i*3-0),0);
                }
            }
            
            ofxCv::RectTracker& tracker = contourFinder.getTracker();
            for(int i = 0; i < contourFinder.size(); i++) {
                ofPoint center = ofxCv::toOf(contourFinder.getCenter(i));
                float x = center.x/grabber.getWidth();
                float y = center.y/grabber.getHeight();
                ofPoint p;
                p.set(x,y);
                points.push_back(p);
            }
            currentLight++;
        }
        
        
    }
    
    olaSender.sendToOla();
    
    if(bSaveToCsv){
        saveToCsv();
        bSaveToCsv = false;
    }
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    gui.draw();
    
    
    
    ofPushMatrix();
    ofTranslate(0, 200);
    ofSetColor(255);
    grabber.draw(0,0);
    ofSetColor(255,0,255);
    for(auto& p : points){
        ofNoFill();
        ofDrawCircle(p.x * grabber.getWidth(), p.y*grabber.getHeight(), 4);
    }
    ofPopMatrix();

    ofPushMatrix();
    ofTranslate(0+10+grabber.getWidth(), 200);
    ofImage t;
    ofSetColor(255);
    t.setFromPixels(thresholded.getPixels());
    t.draw(0,0);
    ofSetColor(0,255,255);
    contourFinder.draw();
    ofPopMatrix();
    
}

//--------------------------------------------------------------
void ofApp::saveToCsv(){
//    csv.createFile("createfile.csv");
    csv.clear();
    for(int i = 0;  i< points.size(); i++){
        ofxCsvRow row;
        row.setFloat(0,points[i].x);
        row.setFloat(1, points[i].y);
        csv.addRow(row);
    }
    csv.save();
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
