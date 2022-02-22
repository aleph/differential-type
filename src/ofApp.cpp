#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	vector<ofColor> colorz{ ofColor::aliceBlue, ofColor::lightCoral };
	bkg = colorz[0];
	shp = colorz[1];

	curve_gui.add(size.set("size", .1, .001, 1.));
	curve_gui.add(square.set("square", true));
	curve_gui.add(ascend.set("ascend", false));
	curve_gui.add(straight.set("straight", false));
	curve_gui.add(maxRadius.set("maxRadius", 60., 1., 100.));
	curve_gui.add(minRadius.set("minRadius", 40., -100., 100.));
	curve_gui.add(fieldScale.set("fieldScale", 1., .001, 100.));
	curve_gui.add(lateralScale.set("lateralScale", 1., -1., 2.));
	curve_gui.add(change.set("change", 1., .001, 100.));
	curve_gui.add(noisePos.set("noisePos", 60., 0., 100.));
	curve_gui.add(margin.set("margin", 10., 0., 1000.));
	curve_gui.add(height.set("height", .1, 0., 2.));
	curve_gui.add(mess.set("mess", 10., 0., 200.));
	curve_gui.add(points.set("points", 0., 0., 1000000000000000000000.));
	gui.setup(curve_gui);
	gui.setPosition(ofGetWidth() - 300, ofGetHeight() - 300);
	gui.setSize(128, 128);
	reset();

	base.load("test_1.svg");

	for (int i = 0; i < base.getNumPath(); i++) {
		ofPath p = base.getPathAt(i);
		// svg defaults to non zero winding which doesn't look so good as contours
		//p.setPolyWindingMode(OF_POLY_WINDING_ODD);
		vector<ofPolyline> lines = p.getOutline();
		for (int j = 0;j < (int)lines.size();j++) {
			outlines.push_back(lines[j].getResampledBySpacing(1));
		}
	}
}

//--------------------------------------------------------------
void ofApp::update(){
	if (stop == false) {
		updateRadiuses();

		updateCurve();
		counter++;
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofEnableAlphaBlending();
	//ofBackground(bkg);

	ofNoFill();
	ofSetLineWidth(3);
	ofSetColor(shp);

	if (stop && !saved) ofBeginSaveScreenAsSVG("test.svg");

	ofBeginShape();
	ofCurveVertex(curves[0][originalVertices.size() - 1]);
	for (int i = 0; i < curves.size(); i++) {
		if (i == curves.size() - 1) ofSetLineWidth(6);
		//ofCurveVertex(curves[i][originalVertices.size() - 1]);
		for (int j = 0; j < originalVertices.size(); j++) {
			ofCurveVertex(curves[i][j]);
		}
		//ofCurveVertex(curves[i][0]);
		//ofCurveVertex(curves[i][1]);
	}
	ofCurveVertex(curves[curves.size() - 1][0]);
	ofEndShape();

	if (stop && !saved) {
		ofEndSaveScreenAsSVG();
		saved = true;
	}

	//outlines[3].draw();
		
	gui.draw();
}

void ofApp::reset() {
	startPoint = ofVec3f(ofGetWindowWidth() * .5, ofGetWindowHeight() * .5, 0.);
	stop = false;
	saved = false;
	counter = 0;
	points = 0;

	curves.clear();
	curve.clear();
	float radius = size * glm::min(ofGetWindowWidth(), ofGetWindowHeight());

	if (ascend) startPoint.y = radius * 1.2 * height;

	if (square) {
		curve.setClosed(true);
		curve.addVertex(startPoint.x - radius, (startPoint.y + radius * height) , 0.);
		curve.addVertex(startPoint.x + radius, (startPoint.y + radius * height), 0.);
		curve.addVertex(startPoint.x + radius, (startPoint.y - radius * height), 0.);
		curve.addVertex(startPoint.x - radius, (startPoint.y - radius * height), 0.);
	}
	else {
		curve.arc(startPoint, radius, radius *  height, 0, 360, 100);
	}

	originalVertices = curve.getVertices();

	updateRadiuses();
}

void ofApp::updateRadiuses() {
	radiuses.clear();
	noisePos += change;
	for (int i = 0; i < originalVertices.size(); i++) {
		float newRadius = ofNoise(originalVertices[i].x * fieldScale, originalVertices[i].y * fieldScale, noisePos * fieldScale);
		radiuses.push_back(newRadius * maxRadius + minRadius);
	}
}

void ofApp::updateCurve() {
	vector<glm::vec3> oldVertices = curve.getVertices();
	curves.push_back(oldVertices);

	float messValue = (ofRandom(-1., 1.) * mess);
	ofVec3f varianceVector;

	for (int i = 0; i < oldVertices.size(); i++) {
		if (straight) {
			varianceVector = (ofVec3f(0., 1., 0.) * radiuses[i]);
		}
		else if (ascend) {
			varianceVector = (glm::normalize(curve[i] - ofVec3f(ofGetWindowWidth() * .5, 0., 0.)) * radiuses[i]);
		}
		else {
			varianceVector = (glm::normalize(curve[i] - startPoint) * radiuses[i]);
		}
		varianceVector.x *= lateralScale;

		curve[i] += varianceVector;

		curve[i] += (glm::normalize(curve[i] - ofVec3f(/*ofGetWindowWidth() * .5*/0., curve[i].y, 0.)) * messValue);
		
		if (curve[i].x > ofGetWindowWidth() - margin || curve[i].x < margin ||
			curve[i].y > ofGetWindowHeight() - margin || curve[i].y < margin) {
			stop = true;
		}
	}

	points += oldVertices.size();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if (key == 'r') reset();

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	ofColor oldColor = shp;
	shp = bkg;
	bkg = oldColor;
}
