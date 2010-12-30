/*
 *  Flock.cpp
 *  flock
 *
 *  Created by Jeffrey Crouse on 3/30/10.
 *  Copyright 2010 Eyebeam. All rights reserved.
 *
 */

#include "Flock.h"

Flock::Flock() {

}

void Flock::setup(float _worldWidth, float _worldHeight){
	worldWidth = _worldWidth;
	worldHeight = _worldHeight;
}

void Flock::update(ofxCvContourFinder* videoContourFinder, float sep, float ali, float coh, float _maxSpeed, float _maxForce) {
	
	int numBlobs = videoContourFinder->blobs.size();
	int numBoids = boids.size();
	int halfBoids = numBoids/2;
	
	ofPoint simulationScale = ofPoint(worldWidth/videoContourFinder->getWidth(), worldHeight/videoContourFinder->getHeight());
	
	if(videoContourFinder != NULL){
		for (int j=0; j < numBlobs; j++) {
			for(int i=0; i<numBoids; i++) {
				ofPoint blobCentroid = videoContourFinder->blobs[j].centroid;
				
				blobCentroid *= simulationScale; //scale for the simluation wolly!
				
				if(i<halfBoids) {
					boids[i]->seek(blobCentroid);
				} else {
					boids[i]->arrive(blobCentroid);
				}
			}
		}		
	} //this is fine for now...

//	if(videoContourFinder != NULL){
//		for (int j=0; j < numBlobs; j++) {
//			for(int i=0; i<numBoids; i++) {
//				boids[i]->repel(videoContourFinder->blobs[j].centroid); //not working yet, look at repel example for the real deal
//			}
//		}		
//	}
	
//	if(videoContourFinder != NULL){
//		for (int j=0; j < numBlobs; j++) {
//			for(int i=0; i<numBoids; i++) {
//				boids[i]->attract2(videoContourFinder->blobs[j].centroid); //not working yet, look at repel example for the real deal
//			}
//		}		
//	}	
	
	for(int i=0; i<boids.size(); i++) {
		boids[i]->update(boids, sep, ali, coh, _maxSpeed, _maxForce);
	}
}

void Flock::draw(float brightness) {
	for(int i=0; i<boids.size(); i++) {
		boids[i]->draw(brightness);
	}
}

void Flock::addBoid() {
	boids.push_back( new Boid(worldWidth, worldHeight) );
}