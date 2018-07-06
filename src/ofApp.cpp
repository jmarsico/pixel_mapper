#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    ofxJSONElement config;
    config.open("config.json");
    
    int olaPort = config["olaPort"].asInt();
    int numUniv = config["numUniv"].asInt();
    numLights = config["numLights"].asInt();
    csvFileName = config["mapCsvFile"].asString();
    
    olaSender.setup(olaPort, 1);
    
    samplerFbo.allocate(640,480);
    
    csv.load(csvFileName);
    
    grabber.setup(640, 480);
    thresholded.allocate(320,240, OF_IMAGE_COLOR);
    
    gui.setup();
    gui.add(FPS.set("FPS"));
    gui.add(bStartMapping.set("start mapping", false));
    gui.add(bIsMapping.set("is mapping", false));
    gui.add(currLight.set("current light", 0, 0, numLights));
    gui.add(numRecorded.set("num recorded", 0, 0, 600));
    gui.add(bSaveToCsv.set("save csv", true));
    gui.add(bgThresh.set("bgthresh", 50, 0, 255));
    gui.add(minSize.set("min size", 3, 1, 30 ));
    gui.add(maxSize.set("max size", 50, 10, 100));
    gui.add(dilation.set("dilation", 10, 1, 50));
    gui.add(sleep.set("sleep millis", 100, 0, 1000));
    gui.add(bResetThresh.set("reset thresh", false));
    gui.add(bPlay.set("use map play", false));
    gui.add(contrast.set("contrast", 1.0, 0.0, 2.0));
    gui.add(sampleSize.set("smoothing", 15, 1, 100));
    gui.add(bLoadLightPoints.set("load light points", false));
    gui.loadFromFile("settings.xml");
    

    player.load("firework.mov");
    player.play();
    
}

//--------------------------------------------------------------
void ofApp::update(){

    FPS = ofToString((int)ofGetFrameRate());
    
    //start the mapping process
    if(bStartMapping){
        background.reset();
        bIsMapping = true;
        currentLight = 1;
        numRecorded = 0;
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
    contourFinder.setMinAreaRadius(minSize);
    contourFinder.setMaxAreaRadius(maxSize);
    int d = dilation;
    ofxCv::dilate(thresholded, d);
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
            ofSleepMillis(sleep);
            ofxCv::RectTracker& tracker = contourFinder.getTracker();
//            for(int i = 0; i < contourFinder.size(); i++) {
                ofPoint center = ofxCv::toOf(contourFinder.getCenter(0));
                float x = center.x/grabber.getWidth();
                float y = center.y/grabber.getHeight();
                ofPoint p;
                p.set(x,y);
                points.push_back(p);
//            }
            currentLight++;
            
        }
        numRecorded = points.size();
        
    }
    
    olaSender.sendToOla();
    
    if(bSaveToCsv){
        saveToCsv();
        bSaveToCsv = false;
    }
    
    if(bLoadLightPoints) {
        loadLightPoints();
        bLoadLightPoints = false;
    }
    
    
    if(bPlay){
        player.update();
        drawContent();

        
        
        if(player.isFrameNew()){
            for(auto &lp : lightPoints){
                lp.setAvgSamplingSize(sampleSize);
                lp.setCurrentCol(ofColor(samplerPix.getColor((int)lp.getLoc().x, (int)lp.getLoc().y)));
               
                int r = (contrast*(lp.getAvgCol().r - 128))+128;
                int g = (contrast*(lp.getAvgCol().g - 128))+128;
                int b = (contrast*(lp.getAvgCol().b - 128))+128;
                
                olaSender.setChannel(lp.getUniverse(), lp.getStartChan() + 0, r);
                olaSender.setChannel(lp.getUniverse(), lp.getStartChan() + 1, g);
                olaSender.setChannel(lp.getUniverse(), lp.getStartChan() + 2, b);
                
            }
            olaSender.sendToOla();
        }
    }
}

//--------------------------------------------------------------
void ofApp::drawContent(){
    
    
    
    
    samplerFbo.begin();
    ofClear(0,0,0,0);
    ofFill();
    ofSetColor(255);
    player.draw(0,0,samplerFbo.getWidth(), samplerFbo.getHeight());
//    ofSetColor(255,0,0);
//    ofDrawCircle(mouseX, mouseY, 40);
    samplerFbo.end();
    
    samplerFbo.readToPixels(samplerPix);
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofBackground(0);
    
    if(bPlay == false){
        ofPushMatrix();
        ofTranslate(0, 300);
        ofSetColor(255);
        grabber.draw(0,0);
        ofSetColor(255,0,255);
        for(auto& p : points){
            ofNoFill();
            ofDrawCircle(p.x * grabber.getWidth(), p.y*grabber.getHeight(), 4);
        }
        ofPopMatrix();

        ofPushMatrix();
        ofTranslate(0+10+grabber.getWidth(), 300);
        ofImage t;
        ofSetColor(255);
        t.setFromPixels(thresholded.getPixels());
        t.draw(0,0);
        ofSetColor(0,255,255);
        contourFinder.draw();
        ofPopMatrix();
    } else {
        
        ofPushMatrix();
        ofTranslate(0,300);
        ofSetColor(255,255,255,255);
        samplerFbo.draw(0,0);
        for(auto& lp : lightPoints){
            ofFill();
            ofSetColor(lp.getAvgCol());
            
            ofDrawCircle(lp.getLoc(), 5);
            ofNoFill();
            ofSetColor(0,255,0);
            ofDrawCircle(lp.getLoc(), 6);
        }
        ofPopMatrix();
    }
    
    gui.draw();
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
void ofApp::loadLightPoints(){
    csvLoader.loadLightPoints(csvFileName);
    lightPoints.clear();
    for(int i = 0; i < csvLoader.getNumLights(); i++){
        LightPoint lp;
        float x = csvLoader.getLightLoc(i).x * samplerFbo.getWidth();
        float y = csvLoader.getLightLoc(i).y * samplerFbo.getHeight();
        lp.setup(ofVec2f(x,y), 1, ((i+1)*3) - 2);
        lightPoints.push_back(lp);
        
        ofLog() << "new light point: " << lp.getLoc().x << " " << lp.getLoc().y <<  " chan: " << lp.getStartChan();
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
