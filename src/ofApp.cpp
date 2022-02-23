#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup() {
	vector<ofColor> colorz{ ofColor::aliceBlue, ofColor::lightCoral };
	background_col = colorz[0];
	shape_col = colorz[1];

	curve_gui.add(w_layout.set("w_layout", 1000., 1., 10000.));
	curve_gui.add(h_layout.set("h_layout", 1000., 1., 10000.));
	curve_gui.add(base_points.set("base_points", 10, 0, 1000));
	curve_gui.add(base_vertexes.set("base_vertexes", 4, 0, 1000));
	curve_gui.add(based.set("based", false, false, true));
	curve_gui.add(save.set("save", false, false, true));
	curve_gui.add(draw_vertex.set("draw_vertex", 4, 0, 10));
	gui.setup(curve_gui);
	gui.setPosition(ofGetWidth() - 300, ofGetHeight() - 300);
	gui.setSize(128, 128);

	reset_counter = 0;
	reset();
}


void ofApp::reset() {
	layout = ofRectangle(0, 0, w_layout, h_layout);
	lines_vec.clear();

	running = true;
	gui_on = false;
	count = 0;


	if (based) {
		base.load("test_1.svg");

		for (int i = 0; i < base.getNumPath(); i++) {
			ofPath p = base.getPathAt(i);
			// svg defaults to non zero winding which doesn't look so good as contours
			//p.setPolyWindingMode(OF_POLY_WINDING_ODD);
			vector<ofPolyline> lines = p.getOutline();
			for (int j = 0; j < lines.size(); j++) {
				//outlines.push_back(lines[j].getResampledBySpacing(1));
				lines_vec.push_back(lines[i]);
			}
		}
	}
	else {
		// Build lines
		for (int i = 0; i < base_points; i++) {
			ofPolyline new_poly;
			new_poly.addVertex(ofVec3f(ofRandom(1.0) * w_layout, ofRandom(1.0) * h_layout, 0.0f));
			for (int i = 1; i < base_vertexes; i++) {
				new_poly.addVertex(ofVec3f(new_poly[0].x + ofRandom(-0.5, 0.5) * w_layout * 1/(float)base_points, new_poly[0].y + ofRandom(-1.0, 1.0) * h_layout * 1 / (float)base_points));
			}

			cout << "new_poly: " << new_poly.size() << endl;
			lines_vec.push_back(new_poly);
		}
	}



	//startPoint = ofVec3f(ofGetWindowWidth() * .5, ofGetWindowHeight() * .5, 0.);
	//stop = false;
	//saved = false;
	//counter = 0;
	//points = 0;

	//curves.clear();
	//curve.clear();
	//float radius = size * glm::min(ofGetWindowWidth(), ofGetWindowHeight());

	//if (ascend) startPoint.y = radius * 1.2 * height;

	//if (square) {
	//	curve.setClosed(true);
	//	curve.addVertex(startPoint.x - radius, (startPoint.y + radius * height) , 0.);
	//	curve.addVertex(startPoint.x + radius, (startPoint.y + radius * height), 0.);
	//	curve.addVertex(startPoint.x + radius, (startPoint.y - radius * height), 0.);
	//	curve.addVertex(startPoint.x - radius, (startPoint.y - radius * height), 0.);
	//}
	//else {
	//	curve.arc(startPoint, radius, radius *  height, 0, 360, 100);
	//}

	//originalVertices = curve.getVertices();

	//updateRadiuses();
}

//--------------------------------------------------------------
void ofApp::update(){
	if (running) {
		count++;
	}


	//if (stop == false) {
	//	updateRadiuses();

	//	updateCurve();
	//	counter++;
	//}
}

//--------------------------------------------------------------
void ofApp::draw(){

	ofFill();
	ofSetColor(background_col);
	ofDrawRectangle(layout);


	ofSetColor(shape_col);

	if (save) {
		ofBeginSaveScreenAsSVG("test.svg");
	}

	for (int i = 0; i < lines_vec.size(); i++) {
		ofNoFill();
		ofSetLineWidth(2);

		lines_vec[i].draw();

		if (draw_vertex > 0) {
			ofFill();
			ofSetLineWidth(1);
			for (int j = 0; j < lines_vec[i].size(); j++) {
				ofDrawCircle(lines_vec[i][j], draw_vertex);
			}
		}
	}


	//ofBeginShape();
	//ofCurveVertex(curves[0][originalVertices.size() - 1]);
	//for (int i = 0; i < curves.size(); i++) {
	//	if (i == curves.size() - 1) ofSetLineWidth(6);
	//	//ofCurveVertex(curves[i][originalVertices.size() - 1]);
	//	for (int j = 0; j < originalVertices.size(); j++) {
	//		ofCurveVertex(curves[i][j]);
	//	}
	//	//ofCurveVertex(curves[i][0]);
	//	//ofCurveVertex(curves[i][1]);
	//}
	//ofCurveVertex(curves[curves.size() - 1][0]);
	//ofEndShape();

	if (save) {
		ofBeginSaveScreenAsSVG("test.svg");
		save = false;
	}

		
	if (gui_on) {
		gui.draw();
	}
}

//void ofApp::updateRadiuses() {
//	radiuses.clear();
//	noisePos += change;
//	for (int i = 0; i < originalVertices.size(); i++) {
//		float newRadius = ofNoise(originalVertices[i].x * fieldScale, originalVertices[i].y * fieldScale, noisePos * fieldScale);
//		radiuses.push_back(newRadius * maxRadius + minRadius);
//	}
//}
//
//void ofApp::updateCurve() {
//	vector<glm::vec3> oldVertices = curve.getVertices();
//	curves.push_back(oldVertices);
//
//	float messValue = (ofRandom(-1., 1.) * mess);
//	ofVec3f varianceVector;
//
//	for (int i = 0; i < oldVertices.size(); i++) {
//		if (straight) {
//			varianceVector = (ofVec3f(0., 1., 0.) * radiuses[i]);
//		}
//		else if (ascend) {
//			varianceVector = (glm::normalize(curve[i] - ofVec3f(ofGetWindowWidth() * .5, 0., 0.)) * radiuses[i]);
//		}
//		else {
//			varianceVector = (glm::normalize(curve[i] - startPoint) * radiuses[i]);
//		}
//		varianceVector.x *= lateralScale;
//
//		curve[i] += varianceVector;
//
//		curve[i] += (glm::normalize(curve[i] - ofVec3f(/*ofGetWindowWidth() * .5*/0., curve[i].y, 0.)) * messValue);
//		
//		if (curve[i].x > ofGetWindowWidth() - margin || curve[i].x < margin ||
//			curve[i].y > ofGetWindowHeight() - margin || curve[i].y < margin) {
//			stop = true;
//		}
//	}
//
//	points += oldVertices.size();
//}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if (key == 'r') reset();

	if (key == 'h') {
		gui_on = !gui_on;
	}
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
}
