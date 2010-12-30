/*
 *  Boid.cpp
 *  boid
 *
 *  Created by Jeffrey Crouse on 3/29/10.
 *  Copyright 2010 Eyebeam. All rights reserved.
 *
 */

#include "Boid.h"

Boid::Boid() {
	Boid(1024.f, 768.f);
}

Boid::Boid(float _worldWidth, float _worldHeight){
	
	worldWidth = _worldWidth;
	worldHeight = _worldHeight;
	
    loc.x = ofRandomWidth();
	loc.y = ofRandomHeight();
	vel = 0;
	acc = 0;
    r = 3.f;
//    maxspeed = 1;
//    maxforce = 0.1;
    maxspeed = 4.f;
    maxforce = 0.1f;
	

	loc.x = ofRandom(0.f, worldWidth);
	loc.y = ofRandom(0.f, worldHeight);
	
	bounce = 1.2f;
}

// Method to update location
void Boid::update(vector<Boid*> boids, float _sep, float _ali, float _coh, float _maxSpeed, float _maxForce) {
	
	setMaxSpeed(_maxSpeed);
	setMaxForce(_maxForce);
	
	flock(boids, _sep, _ali, _coh);
	
    vel += acc;   // Update velocity
    vel.x = ofClamp(vel.x, -maxspeed, maxspeed);  // Limit speed
	vel.y = ofClamp(vel.y, -maxspeed, maxspeed);  // Limit speed
    loc += vel;
    acc = 0;  // Reset accelertion to 0 each cycle
	
//	if (loc.x < -r) loc.x = worldWidth+r;
//    if (loc.y < -r) loc.y = worldHeight+r;
//    if (loc.x > worldWidth+r) loc.x = -r;
//    if (loc.y > worldHeight+r) loc.y = -r;
//	not looping, bouncing
	
	if (loc.y < 0) {
		vel.y *= -bounce;
		loc.y = 0.f;
    }
	
	if (loc.y > worldHeight) {
		vel.y *= -bounce;
		loc.y = worldHeight;
    }
	
    if ((loc.x > worldWidth) || (loc.x < 0)) {
		vel.x *= -bounce;
    }  
}

void Boid::seek(ofPoint target) {
    acc += steer(target, false);
}

void Boid::arrive(ofPoint target) {
    acc += steer(target, true);
}

void Boid::repel(ofPoint target) {
		//acc -= steer(target, true); // just guessing for now! NOT RIGHT
	
	float g = 1000;
	
	ofPoint dir = target - loc;							// Calculate direction of force
	float distance = ofDist(loc.x, loc.y, target.x, target.y);	// Distance between objects
	
	dir /= distance;											// Normalize vector (distance doesn't matter here, we just want this vector for direction)
														//cout << dir.x << ", " << dir.y << endl;
	distance = ofClamp(distance, -50.0, 50.0);							// Keep distance within a reasonable range
	float force = -1.0 * g / (distance * distance);					// Repelling force is inversely proportional to distance
	
	
	acc += dir * force;	// Get force vector --> magnitude * direction
}

void Boid::attract2(ofPoint target){
//	ofPoint Attractor::calcGravForce(Thing t){
//		ofPoint dir = loc - t.loc;							// Calculate direction of force
//		float d = ofDist(loc.x, loc.y, t.loc.x, t.loc.y);   // Distance between objects
//		d = ofClamp(d,5.0,25.0);							// Limiting the distance to eliminate "extreme" results for very close or very far objects
//		dir /=d ;											// Normalize vector (distance doesn't matter here, we just want this vector for direction)
//		float force = (G * mass * t.mass) / (d * d);		// Calculate gravitional force magnitude
//		dir *= force;										// Get force vector --> magnitude * direction
//		return dir;
//	}
	
	float G = 9.8;
	
	ofPoint direction = target - loc;
	float distance = ofDist(loc.x, loc.y, target.x, target.y);
	distance = ofClamp(distance,5.0,25.0);
	
	direction /= distance;
	float force = G / (distance * distance);
	direction *= force;
	
	acc += direction; //this is the just the same as above but with a +1 instead of -1 in the multple and a different G
	
}

// A method that calculates a steering vector towards a target
// Takes a second argument, if true, it slows down as it approaches the target
ofPoint Boid::steer(ofPoint target, bool slowdown) {
    ofPoint steer;  // The steering vector
    ofPoint desired = target - loc;  // A vector pointing from the location to the target
    float d = ofDist(target.x, target.y, loc.x, loc.y); // Distance from the target is the magnitude of the vector
    
	// If the distance is greater than 0, calc steering (otherwise return zero vector)
    if (d > 0) {
		
		desired /= d; // Normalize desired
		// Two options for desired vector magnitude (1 -- based on distance, 2 -- maxspeed)
		if ((slowdown) && (d < 100.0f)) {
			desired *= maxspeed * (d/100.0f); // This damping is somewhat arbitrary
		} else {
			desired *= maxspeed;
		}
		// Steering = Desired minus Velocity
		steer = desired - vel;
		steer.x = ofClamp(steer.x, -maxforce, maxforce); // Limit to maximum steering force
		steer.y = ofClamp(steer.y, -maxforce, maxforce); 
    }
    return steer;
}

void Boid::draw(float brightness) {
    // Draw a triangle rotated in the direction of velocity
	float angle = (float)atan2(-vel.y, vel.x);
    float theta =  -1.0*angle;
	float heading2D = ofRadToDeg(theta)+90;
	
	ofEnableAlphaBlending();
    ofSetColor(255, 255, 255, 255*brightness);
    ofFill();
    ofPushMatrix();
    ofTranslate(loc.x, loc.y);
    ofRotateZ(heading2D);
	ofTriangle(0, -r*2, -r, r*2, r, r*2); //ofTriangle is quicker according to: http://www.openframeworks.cc/forum/viewtopic.php?f=8&t=3026&view=previous	
		//	ofBeginShape(); 
//    ofVertex(0, -r*2);
//    ofVertex(-r, r*2);
//    ofVertex(r, r*2);
//    ofEndShape(true);
    ofPopMatrix();
	ofDisableAlphaBlending();
}

void Boid::flock(vector<Boid*> boids, float _sep, float _ali, float _coh) {
	ofPoint sep = separate(boids);
	ofPoint ali = align(boids);
	ofPoint coh = cohesion(boids);
	
	// Arbitrarily weight these forces
	sep *= _sep;
	ali *= _ali;
	coh *= _coh;
	
	acc += sep + ali + coh;
	
}

// Separation
// Method checks for nearby boids and steers away
ofPoint Boid::separate(vector<Boid*> boids) {
    float desiredseparation = 25.0f;
    ofPoint steer;
    int count = 0;
	
    // For every boid in the system, check if it's too close
    for (int i = 0 ; i < boids.size(); i++) {
		Boid& other = *boids[i]; //again due to memo on http://www.openframeworks.cc/forum/viewtopic.php?f=8&t=3026&view=previous
		float d = ofDist(loc.x, loc.y, other.loc.x, other.loc.y);
		// If the distance is greater than 0 and less than an arbitrary amount (0 when you are yourself)
		if ((d > 0) && (d < desiredseparation)) {
			// Calculate vector pointing away from neighbor
			ofPoint diff = loc - other.loc;
			diff /= d;			// normalize
			diff /= d;        // Weight by distance
			steer += diff;
			count++;            // Keep track of how many
		}
    }
    // Average -- divide by how many
    if (count > 0) {
		steer /= (float)count;
    }
	
	
    // As long as the vector is greater than 0
	float mag = sqrt(steer.x*steer.x + steer.y*steer.y);
    if (mag > 0) {
		// Implement Reynolds: Steering = Desired - Velocity
		steer /= mag;
		steer *= maxspeed;
		steer -= vel;
		steer.x = ofClamp(steer.x, -maxforce, maxforce);
		steer.y = ofClamp(steer.y, -maxforce, maxforce);
    }
    return steer;
}

// Alignment
// For every nearby boid in the system, calculate the average velocity
ofPoint Boid::align(vector<Boid*> boids) {
    float neighbordist = 50.0;
    ofPoint steer;
    int count = 0;
    for (int i = 0 ; i < boids.size(); i++) {
		Boid& other = *boids[i];
		
		float d = ofDist(loc.x, loc.y, other.loc.x, other.loc.y);
		if ((d > 0) && (d < neighbordist)) {
			steer += (other.vel);
			count++;
		}
    }
    if (count > 0) {
		steer /= (float)count;
    }
	
    // As long as the vector is greater than 0
	float mag = sqrt(steer.x*steer.x + steer.y*steer.y);
    if (mag > 0) {
		// Implement Reynolds: Steering = Desired - Velocity
		steer /= mag;
		steer *= maxspeed;
		steer -= vel;
		steer.x = ofClamp(steer.x, -maxforce, maxforce);
		steer.y = ofClamp(steer.y, -maxforce, maxforce);
    }
    return steer;
}

// Cohesion
// For the average location (i.e. center) of all nearby boids, calculate steering vector towards that location
ofPoint Boid::cohesion(vector<Boid*> boids) {
    float neighbordist = 50.0;
    ofPoint sum;   // Start with empty vector to accumulate all locations
    int count = 0;
    for (int i = 0 ; i < boids.size(); i++) {
		Boid& other = *boids[i];
		float d = ofDist(loc.x, loc.y, other.loc.x, other.loc.y);
		if ((d > 0) && (d < neighbordist)) {
			sum += other.loc; // Add location
			count++;
		}
    }
    if (count > 0) {
		sum /= (float)count;
		return steer(sum, false);  // Steer towards the location
    }
    return sum;
}

