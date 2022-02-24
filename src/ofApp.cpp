#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup() {
	colorz = vector<ofColor>{ ofColor::white, ofColor::black, ofColor::lightCoral };
	background_col = colorz[0];
	shape_col = colorz[1];

	curve_gui.add(w_layout.set("w_layout", 1000., 1., 10000.));
	curve_gui.add(h_layout.set("h_layout", 1000., 1., 10000.));
	curve_gui.add(base_points.set("base_points", 100, 0, 1000));
	curve_gui.add(base_vertexes.set("base_vertexes", 50, 0, 1000));
	curve_gui.add(based.set("based", false, false, true));
	curve_gui.add(based_field.set("based_field", false, false, true));
	curve_gui.add(save.set("save", false, false, true));
	curve_gui.add(stop.set("stop", false, false, true));
	curve_gui.add(freeze.set("freeze", true, false, true));
	curve_gui.add(values_ratio.set("values_ratio", 0.4, 0.0, 1.0));
	curve_gui.add(draw_vertex.set("draw_vertex", 4, 0, 10));
	curve_gui.add(draw_field.set("draw_field", 0.1, 0.0, 1.0));
	curve_gui.add(draw_neighbourhood.set("draw_neighbourhood", false, false, true));
	curve_gui.add(draw_separation.set("draw_separation", false, false, true));

	//----------------------------------------------
	curve_gui.add(NeighborhoodRadiusSquare.set("NeighborhoodRadiusSquare", .6, 0.0, 1.0));
	curve_gui.add(CohesionStrength.set("CohesionStrength", .3, 0.0, 1.0));
	curve_gui.add(AlignmentStrength.set("AlignmentStrength", .15, 0.0, 1.0));
	curve_gui.add(SeparationStrength.set("SeparationStrength", .8, 0.0, 1.0));
	curve_gui.add(SeparationRadius.set("SeparationRadius", .4, 0.0, 1.0));
	curve_gui.add(FieldStrength.set("FieldStrength", .8, 0.0, 1.0));
	curve_gui.add(ScalarStrength.set("ScalarStrength", .8, 0.0, 1.0));
	curve_gui.add(FieldScale.set("FieldScale", .034, 0.0, 0.2));
	curve_gui.add(FieldTime.set("FieldTime", 5.15, 0.0, 10.0));
	curve_gui.add(MaxAngle.set("MaxAngle", 180, 0.0, 360));
	curve_gui.add(MaxSpeed.set("MaxSpeed", .05, 0.0, 1.0));
	curve_gui.add(Inertia.set("Inertia", 0.97, 0.0, 1.0));
	curve_gui.add(BoundingBoxSize.set("BoundingBoxSize", 800.0, 0.0, 1000.0));
	curve_gui.add(ContainmentStrength.set("ContainmentStrength", 200.0, 0.0, 1000.0));
	//----------------------------------------------

	gui.setup(curve_gui);
	gui.setPosition(ofGetWidth() - 220, 0);
	gui.setSize(128, 128);


	debug = false;
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
	values_bias = min(w_layout, h_layout) * values_ratio;


	vector<glm::vec3> cont_vertexes{ ofPoint((w_layout - BoundingBoxSize) * 0.5, (h_layout - BoundingBoxSize) * 0.5), ofPoint(BoundingBoxSize, (h_layout - BoundingBoxSize) * 0.5), ofPoint(BoundingBoxSize, BoundingBoxSize), ofPoint((w_layout - BoundingBoxSize) * 0.5, BoundingBoxSize) };
	containment = ofPolyline(cont_vertexes);
	containment.close();

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
			ofVec3f velocity = ofVec3f(ofRandom(-1.0, 1.0), ofRandom(-1.0, 1.0));
			velocity.limit(MaxSpeed * values_bias);

			new_poly.addVertex(ofVec3f(ofRandom(0.2, 0.8) * w_layout, ofRandom(0.2, 0.8) * h_layout, 0.0f));
			new_poly.addVertex(new_poly[0] + velocity);
			for (int i = 2; i < base_vertexes; i++) {
				new_poly.addVertex(ofVec3f(new_poly[0].x + ofRandom(-0.1, 0.1) * values_bias, new_poly[0].y + ofRandom(-0.1, 0.1) * values_bias));
			}

			if (debug)
				cout << "new_poly: " << curves_vec.size() << endl;
			curves_vec[i] = new_poly;
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

	if (stop) {
		running = false;
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

	if (draw_field > 0.0) {
		ofSetLineWidth(1);
		ofSetColor(colorz[2]);

		float test_num = 20.0;
		for (int i = 0.5; i < test_num; i++) {
			for (int j = 0.5; j < test_num; j++) {
				ofVec3f test_point(i * w_layout / test_num, j * h_layout / test_num);

				ofVec3f vector_field = computeVectorFieldHere(test_point);
				float scalar_field = computeFieldHere(test_point);

				ofDrawLine(test_point, test_point + vector_field * values_bias * draw_field);
				ofDrawCircle(test_point, scalar_field * values_bias * draw_field * 0.5);
			}
		}
	}

	ofNoFill();
	if (draw_neighbourhood) {
		for (int i = 0; i < curves_vec.size(); i++) {
			ofDrawCircle(curves_vec[i][1], NeighborhoodRadiusSquare * values_bias);
		}
	}

	if (draw_separation) {
		for (int i = 0; i < curves_vec.size(); i++) {
			ofDrawCircle(curves_vec[i][1], SeparationRadius * values_bias);
		}
	}


	if (debug)
		cout << "_drawing";
	ofSetColor(shape_col);
	for (int i = 0; i < curves_vec.size(); i++) {
		ofNoFill();
		ofSetLineWidth(1);

		//curves_vec[i].draw();

		if (draw_vertex > 0) {
			ofFill();
			ofSetLineWidth(1);
			for (int j = 0; j < curves_vec[i].size(); j++) {
				ofDrawCircle(curves_vec[i][j], draw_vertex * (0.5 + 0.5 * ((curves_vec[i].size() - j) / (float)curves_vec[i].size())));
			}
		}
	}


	if (save) {
		if (debug)
			cout << "_start-saving";

		ofBeginSaveScreenAsSVG("test.svg");
	}


	for (int i = 0; i < curves_vec.size(); i++) {
		ofNoFill();
		ofSetLineWidth(1);

		ofBeginShape();
		for (int j = 0; j < curves_vec[i].size(); j++) {
			ofCurveVertex(curves_vec[i][j]);
		}
		ofEndShape();
	}


	if (save) {
		ofEndSaveScreenAsSVG();
		save = false;
	}

	//ofSetLineWidth(10);
	//ofSetColor(0.0);
	//containment.draw();
		
	if (gui_on) {
		gui.draw();
		ofDrawBitmapStringHighlight("FPS: " + ofToString(ofGetFrameRate()), glm::vec2(ofGetWidth() - 110, ofGetHeight() - 15));
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

		if (id != i && possible_neighbour.squareDistance(ag) < (NeighborhoodRadiusSquare * values_bias * values_bias)) {
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
	//if (position.x < (w_layout - BoundingBoxSize) * 0.5)
	//	desiredVelocity += ofVec3f((w_layout - BoundingBoxSize) * 0.5 -position.x, 0, 0);
	//else if (position.x > BoundingBoxSize)
	//	desiredVelocity += ofVec3f(BoundingBoxSize - position.x, 0, 0);

	//if (position.y < (h_layout - BoundingBoxSize) * 0.5)
	//	desiredVelocity += ofVec3f(0, (h_layout - BoundingBoxSize) * 0.5 -position.y, 0);
	//else if (position.y > BoundingBoxSize)
	//	desiredVelocity += ofVec3f(0, BoundingBoxSize - position.y, 0);

	//desiredVelocity *= (ContainmentStrength /** values_bias*/);

	ofPoint closestPoint = containment.getClosestPoint(position);
	if (debug)
		cout << "closest_point" << ofToString(closestPoint) << endl;

	ofVec3f getAway = position - closestPoint;
	if (getAway.length() < (SeparationRadius * values_bias))
	{
		getAway /= (getAway.length() * getAway.length());
		desiredVelocity += getAway;
	}

	//desiredVelocity.limit(MaxSpeed * values_bias);
	desiredVelocity *= (ContainmentStrength * values_bias);


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
		desiredVelocity += cohesion * CohesionStrength /** values_bias*/;

		// . . . . . . . . . . . . . ALIGNMENT BEHAVIOUR  . . . . . . . . . . . . .
		ofVec3f alignment(0.0, 0.0);

		for (int i = 0; i < neighbours.size(); i++) {
			alignment += (curves_vec[neighbours[i]][0] - curves_vec[neighbours[i]][1]);
		}

		alignment /= neighbours.size();

		desiredVelocity += AlignmentStrength * alignment  /** values_bias*/;

		// . . . . . . . . . . . . . SEPARATION BEHAVIOUR  . . . . . . . . . . . . .
		ofVec3f separation(0.0, 0.0);

		for (int i = 0; i < neighbours.size(); i++) {
			float distanceToNeighbour = position.distance(curves_vec[neighbours[i]][1]);

			if (distanceToNeighbour < (SeparationRadius * values_bias))
			{
				ofVec3f getAway = position - curves_vec[neighbours[i]][1];
				getAway /= (getAway.length() * distanceToNeighbour);
				separation += getAway;
			}
		}

		desiredVelocity += SeparationStrength * separation  * values_bias;

	}


	// -------------------------------- FIELD --------------------------------

	ofVec3f field = computeVectorFieldHere(position);
	desiredVelocity += field * FieldStrength * values_bias;

	if (!based_field) {
		float field_scalar = computeFieldHere(position);
		desiredVelocity += desiredVelocity * field * ScalarStrength;
	}
	else {
		desiredVelocity += ofVec3f(0.0, 0.0);
	}

	// -------------------------------- UPDATE --------------------------------
	desired_velocities[id] = desiredVelocity;

}

void ofApp::updateVelocityAndPosition(int id) {

	// steering
	ofVec3f velocity = curves_vec[id][0] - curves_vec[id][1];
	velocity = Inertia * velocity + (1.0 - Inertia) * desired_velocities[id];

	// limit velocity to MaxSpeed
	velocity.limit(MaxSpeed * values_bias);

	// update trails
	for (int i = curves_vec[id].size() - 1; i > 1; i--) {
		curves_vec[id][i] = curves_vec[id][i - 1];
	}

	if (!freeze) {
		curves_vec[id][1] += velocity;
	}
	curves_vec[id][0] = curves_vec[id][1] + velocity;

}

float ofApp::computeFieldHere(ofVec3f p) {
	p *= (FieldScale * 0.1);
	p += ofVec3f(0, 0, FieldTime);
	float value = ofNoise(p);

	return value;
}

ofVec3f ofApp::computeVectorFieldHere(ofVec3f p) {
	p *= (FieldScale * 0.1);
	p += ofVec3f(0, 0, FieldTime);
	float angle = ofSignedNoise(p) * MaxAngle;
	ofVec3f field(1.0, 0.0, 0.0);
	field.rotate(angle, ofVec3f(0.0, 0.0, 1.0));

	return field;
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

	if (key == 's') {
		save = true;
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
