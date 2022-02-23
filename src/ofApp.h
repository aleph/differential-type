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
		void mousePressed(int x, int y, int button);

		void reset();


		//----PARAMS
		bool running;
		bool gui_on;
		int reset_counter;
		int count;



		//----LINES
		ofxSVG base;
		ofRectangle layout;
		vector<ofPolyline> lines_vec;




		//----GRAPHICS
		ofColor background_col;
		ofColor shape_col;




		

		//void updateCurve();
		//void updateRadiuses();


		//ofVec3f startPoint;
		//bool stop;
		//bool saved;
		//int counter;

		//ofPolyline curve;
		//vector<vector<glm::vec3>> curves;

		//vector<vector<vector<glm::vec3>>> m_curves;

		//vector<float> radiuses;
		//vector<glm::vec3> originalVertices;

		ofxPanel gui;
		ofParameterGroup curve_gui;
		ofParameter<float> w_layout;
		ofParameter<float> h_layout;
		ofParameter<int> base_points;
		ofParameter<int> base_vertexes;
		ofParameter<bool> based;
		ofParameter<bool> save;
		ofParameter<float> draw_vertex;


		//vector<ofPolyline> outlines;
		/*vector<ofPath::Command> commands;*/
};