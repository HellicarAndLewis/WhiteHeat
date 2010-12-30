/*
 *  Boid.h
 *  boid
 *
 *  Created by Jeffrey Crouse on 3/29/10.
 *  Copyright 2010 Eyebeam. All rights reserved.
 *
 */

#ifndef BOID_H
#define BOID_H

#include "ofMain.h"

class Boid {
public:
	Boid();
	Boid(float _worldWidth, float _worldHeight);
	
	void update(vector<Boid*> boids, float _sep, float _ali, float _coh, float _maxSpeed, float _maxForce);
	void draw(float brightness);
	void seek(ofPoint target);
	void arrive(ofPoint target);
	void repel(ofPoint target);
	void attract2(ofPoint target);
	ofPoint steer(ofPoint target, bool slowdown);
	
	void flock(vector<Boid*> boids, float _sep, float _ali, float _coh);
	ofPoint separate(vector<Boid*> boids);
	ofPoint align(vector<Boid*> boids);
	ofPoint cohesion(vector<Boid*> boids);

private:	
	void setMaxSpeed(float _maxSpeed){
		maxspeed = _maxSpeed;
	}
	
	void setMaxForce(float _maxForce){
		maxforce = _maxForce;
	}	
	
	ofPoint loc;
	ofPoint vel;
	ofPoint acc;
	float r;
	float maxforce;
	float maxspeed;
	
	float worldWidth;
	float worldHeight;
	
	float bounce;
};

#endif