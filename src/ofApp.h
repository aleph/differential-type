#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxSvg.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void reset();

		void updateCurve();
		void updateRadiuses();

		void keyPressed(int key);
		void mousePressed(int x, int y, int button);
		

		ofColor bkg;
		ofColor shp;
		ofVec3f startPoint;
		bool stop;
		bool saved;
		int counter;

		ofPolyline curve;
		vector<vector<glm::vec3>> curves;

		vector<vector<vector<glm::vec3>>> m_curves;

		vector<float> radiuses;
		vector<glm::vec3> originalVertices;

		ofxPanel gui;
		ofParameterGroup curve_gui;
		ofParameter<float> size;
		ofParameter<bool> square;
		ofParameter<bool> ascend;
		ofParameter<bool> straight;
		ofParameter<float> maxRadius;
		ofParameter<float> minRadius;
		ofParameter<float> fieldScale;
		ofParameter<float> change;
		ofParameter<float> noisePos;
		ofParameter<float> lateralScale;
		ofParameter<float> margin;
		ofParameter<float> height;
		ofParameter<float> mess;
		ofParameter<float> points;

		ofxSVG base;
		vector<ofPolyline> outlines;
		/*vector<ofPath::Command> commands;*/
};
