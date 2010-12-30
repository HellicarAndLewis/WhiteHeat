/*
 *  Flock.h
 *  flock
 *
 *  Created by Jeffrey Crouse on 3/30/10.
 *  Copyright 2010 Eyebeam. All rights reserved.
 *
 */

#ifndef FLOCK_H
#define FLOCK_H

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "Boid.h"

class Flock {
public:
	Flock();
	void setup(float _worldWidth, float _worldHeight);
	void update(ofxCvContourFinder* videoContourFinder, float sep, float ali, float coh, float _maxSpeed, float _maxForce);
	void draw(float brightness=1.f);
	void addBoid();
	
	vector<Boid*> boids;
	
	float worldWidth;
	float worldHeight;
};

#endif