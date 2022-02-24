#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxSvg.h"


class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);


		void reset();
		void updateSystem();
		void updateRTree();

		vector<int> findNeighbours(ofVec3f ag, int id);
		void computeDesiredVelocity(int id, vector<int> neighbours);
		void updateVelocityAndPosition(int id);
		float computeFieldHere(ofVec3f p);
		ofVec3f computeVectorFieldHere(ofVec3f p);


		//----PARAMS
		bool running;
		bool gui_on;
		int reset_counter;
		int count;
		bool debug;
		bool resetting;



		//----LINES
		ofxSVG base;
		ofRectangle layout;
		vector<ofPolyline> curves_vec;
		vector<ofVec3f> desired_velocities;
		float values_bias;
		ofPolyline containment;




		//----GRAPHICS
		ofColor background_col;
		ofColor shape_col;
		vector<ofColor> colorz;
		ofFbo drawFbo;



		//----GUIs
		ofxPanel gui;
		ofParameterGroup curve_gui;
		ofParameter<float> w_layout;
		ofParameter<float> h_layout;
		ofParameter<int> base_points;
		ofParameter<int> base_vertexes;
		ofParameter<bool> based;
		ofParameter<bool> based_field;
		ofParameter<bool> save;
		ofParameter<bool> stop;
		ofParameter<bool> freeze;
		ofParameter<float> draw_vertex;
		ofParameter<float> draw_field;
		ofParameter<bool> draw_neighbourhood;
		ofParameter<bool> draw_separation;
		//-------------------------------
		ofParameter<float> NeighborhoodRadiusSquare;
		ofParameter<float> CohesionStrength;
		ofParameter<float> AlignmentStrength;
		ofParameter<float> SeparationStrength;
		ofParameter<float> SeparationRadius;
		ofParameter<float> FieldStrength;
		ofParameter<float> ScalarStrength;
		ofParameter<float> FieldScale;
		ofParameter<float> FieldTime;
		ofParameter<float> MaxAngle;
		ofParameter<float> MaxSpeed;
		ofParameter<float> Inertia;
		ofParameter<float> BoundingBoxSize;
		ofParameter<float> ContainmentStrength;
		ofParameter<float> values_ratio;




		//RTree PointsRTree;

};
