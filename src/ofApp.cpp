#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	vector<ofColor> colorz{ ofColor::aliceBlue, ofColor::lightCoral };
	bkg = colorz[0];
	shp = colorz[1];
	size = 0.25;
}

//--------------------------------------------------------------
void ofApp::update(){
	startPoint = ofVec3f(ofGetWindowWidth() * .5, ofGetWindowHeight() * .5, 0.);
	startPoint *= 1. - size;
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackground(bkg);
	ofSetColor(shp);
	ofFill();
	ofDrawRectangle(startPoint, ofGetWindowWidth() * size, ofGetWindowHeight() * size);
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
	ofColor oldColor = shp;
	shp = bkg;
	bkg = oldColor;
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
