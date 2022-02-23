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
	//----------------------------------------------
	curve_gui.add(NeighborhoodRadiusSquare.set("NeighborhoodRadiusSquare", .0, 0.0, 1.0));
	curve_gui.add(CohesionStrength.set("CohesionStrength", .0, 0.0, 1.0));
	curve_gui.add(AlignmentStrength.set("AlignmentStrength", .0, 0.0, 1.0));
	curve_gui.add(SeparationStrength.set("SeparationStrength", .0, 0.0, 1.0));
	curve_gui.add(SeparationRadius.set("SeparationRadius", .0, 0.0, 1.0));
	curve_gui.add(FieldStrength.set("FieldStrength", .0, 0.0, 1.0));
	curve_gui.add(FieldScale.set("FieldScale", .0, 0.0, 1.0));
	curve_gui.add(FieldTime.set("FieldTime", .0, 0.0, 1.0));
	curve_gui.add(MaxAngle.set("MaxAngle", 90, 0.0, 360));
	curve_gui.add(MaxSpeed.set("MaxSpeed", .3, 0.0, 1.0));
	curve_gui.add(Inertia.set("Inertia", 0.97, 0.0, 1.0));
	curve_gui.add(BoundingBoxSize.set("BoundingBoxSize", 1000.0, 0.0, 1000.0));
	curve_gui.add(ContainmentStrength.set("ContainmentStrength", .0, 0.0, 1.0));

	gui.setup(curve_gui);
	gui.setPosition(ofGetWidth() - 256, 0);
	gui.setSize(128, 128);


	debug = true;
	reset_counter = 0;
	resetting = false;
	reset();
}


void ofApp::reset() {
	if (debug)
		cout << "_resetting" << endl;

	layout = ofRectangle(0, 0, w_layout, h_layout);
	if (debug)
		cout << "_resetting" << endl;
	curves_vec.resize(base_points);
	if (debug)
		cout << "_resetting" << endl;
	desired_velocities.resize(base_points);

	if (debug)
		cout << "_resetting" << endl;
	running = true;
	gui_on = false;
	count = 0;

	if (debug)
		cout << "_setting-curves" << endl;

	if (based) {
		base.load("test_1.svg");

		for (int i = 0; i < base.getNumPath(); i++) {
			ofPath p = base.getPathAt(i);
			// svg defaults to non zero winding which doesn't look so good as contours
			//p.setPolyWindingMode(OF_POLY_WINDING_ODD);
			vector<ofPolyline> lines = p.getOutline();
			for (int j = 0; j < lines.size(); j++) {
				//outlines.push_back(lines[j].getResampledBySpacing(1));
				curves_vec.push_back(lines[i]);
				desired_velocities.push_back(ofVec3f(0.0,0.0));
			}
		}
	}
	else {
		// Build lines
		for (int i = 0; i < base_points; i++) {
			ofPolyline new_poly;
			new_poly.addVertex(ofVec3f(ofRandom(0.2, 0.8) * w_layout, ofRandom(0.2, 0.8) * h_layout, 0.0f));
			for (int i = 1; i < base_vertexes; i++) {
				new_poly.addVertex(ofVec3f(new_poly[0].x + ofRandom(-0.5, 0.5) * w_layout * 1/(float)base_points, new_poly[0].y + ofRandom(-1.0, 1.0) * h_layout * 1 / (float)base_points));
			}

			if (debug)
				cout << "new_poly: " << curves_vec.size() << endl;
			curves_vec[i] =new_poly;
			desired_velocities[i] = ofVec3f(0.0, 0.0);
		}
	}

}

//--------------------------------------------------------------
void ofApp::update(){
	if (resetting) {
		reset();

		resetting = false;
	}

	if (running) {
		if (debug) {
			cout << "_" << endl;
			cout << "counter:" << count;
		}
			

		if (curves_vec.size() < 500)
			updateSystem();
		else
			updateRTree();

		count++;
	}

	if (debug)
		cout << "_updated";
}

//--------------------------------------------------------------
void ofApp::draw(){

	if (debug)
		cout << "_drawing-background";
	ofFill();
	ofSetColor(background_col);
	ofDrawRectangle(layout);

	ofSetColor(shape_col);

	if (save) {
		if (debug)
			cout << "_start-saving";

		ofBeginSaveScreenAsSVG("test.svg");
	}

	if (debug)
		cout << "_drawing";
	for (int i = 0; i < curves_vec.size(); i++) {
		ofNoFill();
		ofSetLineWidth(2);

		curves_vec[i].draw();

		if (draw_vertex > 0) {
			ofFill();
			ofSetLineWidth(1);
			for (int j = 0; j < curves_vec[i].size(); j++) {
				ofDrawCircle(curves_vec[i][j], draw_vertex);
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
		ofDrawBitmapStringHighlight("FPS: " + ofToString(ofGetFrameRate()), glm::vec2(ofGetWidth() - 96, ofGetHeight() - 8));
	}
}

void ofApp::updateSystem() {
	//curves_vec[i][1]: position
	//curves_vec[i][0]: velocity

	if (debug)
		cout << "_compute";
	for (int i = 0; i < curves_vec.size(); i++) {
		vector<int> neighbours = findNeighbours(curves_vec[i][1], i);
		computeDesiredVelocity(i, neighbours);
	}

	if (debug)
		cout << "_update";
	// update velocity and position
	for (int i = 0; i < curves_vec.size(); i++) {
		updateVelocityAndPosition(i);
	}

}


vector<int> ofApp::findNeighbours(ofVec3f ag, int id) {
	vector<int> neighbours;

	for (int i = 0; i < curves_vec.size(); i++) {
		ofVec3f possible_neighbour = curves_vec[i][1];

		if (id != i && possible_neighbour.squareDistance(ag) < NeighborhoodRadiusSquare) {
			neighbours.push_back(i);
		}
	}

	return neighbours;
}


void ofApp::computeDesiredVelocity(int id, vector<int> neighbours) {

	ofVec3f position = curves_vec[id][1];
	ofVec3f velocity = curves_vec[id][0] - curves_vec[id][1];
	ofVec3f desiredVelocity(0.0, 0.0);

	// ------------------------------ CONTAINMENT -----------------
	if (position.x < 0.0)
		desiredVelocity += ofVec3f(-position.x, 0, 0);
	else if (position.x > BoundingBoxSize)
		desiredVelocity += ofVec3f(BoundingBoxSize - position.x, 0, 0);

	if (position.y < 0.0)
		desiredVelocity += ofVec3f(0, -position.y, 0);
	else if (position.y > BoundingBoxSize)
		desiredVelocity += ofVec3f(0, BoundingBoxSize - position.y, 0);

	desiredVelocity *= ContainmentStrength;


	// -------------------------------- FLOCKING --------------------------------
	if (neighbours.size() > 0)
	{
		// . . . . . . . . . . . . . COHESION BEHAVIOUR  . . . . . . . . . . . . .

		// find neighbours average
		ofVec3f average(0.0, 0.0);

		for (int i = 0; i < neighbours.size(); i++) {
			average += curves_vec[neighbours[i]][1];
		}

		average /= neighbours.size();

		// go there
		ofVec3f cohesion = average - position;

		// update desired velocity
		desiredVelocity += CohesionStrength * cohesion;

		// . . . . . . . . . . . . . ALIGNMENT BEHAVIOUR  . . . . . . . . . . . . .
		ofVec3f alignment(0.0, 0.0);

		for (int i = 0; i < neighbours.size(); i++) {
			alignment += curves_vec[neighbours[i]][0];
		}

		alignment /= neighbours.size();

		desiredVelocity += AlignmentStrength * alignment;

		// . . . . . . . . . . . . . SEPARATION BEHAVIOUR  . . . . . . . . . . . . .
		ofVec3f separation(0.0, 0.0);

		for (int i = 0; i < neighbours.size(); i++) {
			float distanceToNeighbour = position.distance(curves_vec[neighbours[i]][1]);

			if (distanceToNeighbour < SeparationRadius)
			{
				ofVec3f getAway = position - curves_vec[neighbours[i]][1];
				getAway /= (getAway.length() * distanceToNeighbour);
				separation += getAway;
			}
		}

		desiredVelocity += SeparationStrength * separation;

	}


	// -------------------------------- FIELD --------------------------------
	ofVec3f p(position);
	p *= FieldScale;
	p += ofVec3f(0, 0, FieldTime);
	float angle = ofSignedNoise(p) * MaxAngle;
	ofVec3f field(1.0, 0.0, 0.0);
	field.rotate(angle, ofVec3f(0.0, 0.0, 1.0));

	desiredVelocity += FieldStrength * field;


	// -------------------------------- UPDATE --------------------------------
	desired_velocities[id] = desiredVelocity;

}

void ofApp::updateVelocityAndPosition(int id) {

	// steering
	ofVec3f velocity = Inertia * velocity + (1.0 - Inertia) * desired_velocities[id];

	// limit velocity to MaxSpeed
	velocity.limit(MaxSpeed);

	// update trails
	for (int i = base_vertexes; i > 1; i--) {
		curves_vec[id][i] = curves_vec[id][i - 1];
	}

	curves_vec[id][1] += velocity;
	curves_vec[id][0] = curves_vec[id][1] + velocity;

}


void ofApp::updateRTree() {
	int RTree = 0;
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if (key == 'r') {
		resetting = true;
	}

	if (key == 'h') {
		gui_on = !gui_on;
	}
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

}



//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}


//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

// --------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}
