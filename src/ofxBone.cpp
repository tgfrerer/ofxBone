//
//     _____    ___     
//    /    /   /  /     testVmdLoader
//   /  __/ * /  /__    (c) ponies & light, 2012. All rights reserved.
//  /__/     /_____/    poniesandlight.co.uk
//
//  ofxBone.cpp
//  Created by tim on 12/11/2012.
//  
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#include "ofxBone.h"

void ofxBone::customDraw(){

	

	ofVec3f p1(0);
	ofVec3f p2(0);

	if (tr1::shared_ptr<ofxBone> p = parent.lock()){
		p1 = p1 * p->getGlobalTransformMatrix() * ofMatrix4x4::getInverseOf(getGlobalTransformMatrix());
	} else {
//		ofMatrix4x4 m4 = getGlobalTransformMatrix();
//		p1 = ofVec3f(0) * m4;
		p1 = p1 * ofMatrix4x4::getInverseOf(getGlobalTransformMatrix()) ;
	}
	
	ofDrawArrow(p1, p2, 5);
	ofDrawAxis(30);

	

}


// ----------------------------------------------------------------------

void ofxBone::draw(){
	ofPushMatrix();
	ofMultMatrix(getGlobalTransformMatrix());
	customDraw();
	ofPopMatrix();
};

// ----------------------------------------------------------------------

void ofxBone::setParent(tr1::shared_ptr<ofxBone> parent_){
	parent = tr1::weak_ptr<ofxBone>(parent_);
	localTransformMatrix =  localTransformMatrix  * ofMatrix4x4::getInverseOf( parent_->getGlobalTransformMatrix());
};

// ----------------------------------------------------------------------

tr1::shared_ptr<ofxBone> ofxBone::getParent(){
	tr1::shared_ptr<ofxBone> p = parent.lock();
	return (p);
};

// ----------------------------------------------------------------------

void ofxBone::setTransformMatrix(ofMatrix4x4 m_){
	localTransformMatrix = m_;
};

// ----------------------------------------------------------------------

void ofxBone::setOrientation(ofQuaternion orientation_){
	localTransformMatrix.setRotate(orientation_);
};

// ----------------------------------------------------------------------

void ofxBone::setGlobalOrientationAtJoint(ofQuaternion orientation_){
	if (tr1::shared_ptr<ofxBone> p = parent.lock()) {
		p->setGlobalOrientation(orientation_);
		localJointTransformMatrix.makeRotationMatrix(p->getOrientation());
	} else {
		setOrientationAtJoint(orientation_);
		localJointTransformMatrix.makeRotationMatrix(orientation_);
	}
};


// ----------------------------------------------------------------------

const ofQuaternion ofxBone::getOrientationAtJoint() const{
		return localJointTransformMatrix.getRotate();
};

// ----------------------------------------------------------------------

void ofxBone::setOrientationAtJoint(ofQuaternion orientation_){
	if (tr1::shared_ptr<ofxBone> p = parent.lock()) {
		p->setOrientation(orientation_);
		localJointTransformMatrix.makeRotationMatrix(p->getOrientation());
	} else {
		localJointTransformMatrix.makeRotationMatrix(orientation_);
	}
};

// ----------------------------------------------------------------------

void ofxBone::setPosition(ofVec3f position_){
	if (tr1::shared_ptr<ofxBone> p = parent.lock()) {
		localTransformMatrix.setTranslation(position_);
	} else {
		localTransformMatrix.setTranslation(position_);
//		localTransformMatrix = localTransformMatrix * ofMatrix4x4::getInverseOf(getJointTransformMatrix());
	}
};

// ----------------------------------------------------------------------

void ofxBone::setGlobalOrientation(ofQuaternion orientation_){
	ofQuaternion orientationInParentBoneSpace;
	if (tr1::shared_ptr<ofxBone> p = parent.lock()) {
		orientationInParentBoneSpace = orientation_ * p->getGlobalOrientation().inverse();
	} else {
		orientationInParentBoneSpace = orientation_ ;
	}
	localTransformMatrix.setRotate(orientationInParentBoneSpace);
};

// ----------------------------------------------------------------------

void ofxBone::setGlobalPosition(ofVec3f position_){
	ofVec3f positionInParentBoneSpace;
	if (tr1::shared_ptr<ofxBone> p = parent.lock()){
		positionInParentBoneSpace = position_ * ofMatrix4x4::getInverseOf(p->getGlobalTransformMatrix());
	} else {
		positionInParentBoneSpace = position_ ;
	}
	setPosition(positionInParentBoneSpace);
};

// ----------------------------------------------------------------------

const ofMatrix4x4 ofxBone::getGlobalTransformMatrix() const {
	if (tr1::shared_ptr<ofxBone> p = parent.lock()) {
		return getLocalTransformMatrix() * p->getGlobalTransformMatrix();
	} else {
		return getLocalTransformMatrix() * localJointTransformMatrix;
	}
}

// ----------------------------------------------------------------------

const ofMatrix4x4 ofxBone::getLocalTransformMatrix() const {
	return localTransformMatrix;
}

// ----------------------------------------------------------------------

const ofQuaternion ofxBone::getGlobalOrientation() const {
	return getGlobalTransformMatrix().getRotate();
}

// ----------------------------------------------------------------------

const ofQuaternion ofxBone::getOrientation() const {
	return getLocalTransformMatrix().getRotate();
}

// ----------------------------------------------------------------------

const ofVec3f ofxBone::getGlobalPosition() const {
	return getGlobalTransformMatrix().getTranslation();
}

// ----------------------------------------------------------------------

const ofVec3f ofxBone::getGlobalJointPosition() const {
	if (tr1::shared_ptr<ofxBone> p = parent.lock()){
		return (getJointTransformMatrix() * getGlobalTransformMatrix()).getTranslation();
	} else {
		return ofVec3f(0);
	}
}

// ----------------------------------------------------------------------

const ofVec3f ofxBone::getJointPosition() const {
	return getJointTransformMatrix().getTranslation();
}

// ----------------------------------------------------------------------

const ofMatrix4x4 ofxBone::getJointTransformMatrix() const {
	ofMatrix4x4 m;
	
	if (tr1::shared_ptr<ofxBone> p = parent.lock()){
		m = ofMatrix4x4::getInverseOf(getLocalTransformMatrix());
	} else {
		m = localJointTransformMatrix;
	}
	
	return m;
}

// ----------------------------------------------------------------------
const ofMatrix4x4 ofxBone::getGlobalJointTransformMatrix() const {
	ofMatrix4x4 m;
	if (tr1::shared_ptr<ofxBone> p = parent.lock()){
		m = getJointTransformMatrix() * getGlobalTransformMatrix();
	} else {
		m.setTranslation(0, 0, 0);
		m = m * localJointTransformMatrix;
	}
	return m;
}

// ----------------------------------------------------------------------

const ofQuaternion ofxBone::getGlobalOrientationAtJoint() const {
	return getGlobalJointTransformMatrix().getRotate();
}

// ----------------------------------------------------------------------
// INVERSE KINEMATICS
// ----------------------------------------------------------------------

void ofxBoneIKchain::solve(){
	ofVec3f targetPosition = targetPositionInGlobalSpace;
	
	for (int i=0; i<1; i++){	// increase loop count to solve with greater precision.
		for (int j=boneChain.size()-1; j>=0; --j){
			std::tr1::shared_ptr<ofxBone> bone = boneChain[j];
			// shoot ray from current bone origin to target
			// find relative angle between end effector and target
			
			const ofVec3f effectorPosition = endEffector->getGlobalPosition();
			const ofMatrix4x4 worldToBone = (ofMatrix4x4::getInverseOf(bone->getGlobalJointTransformMatrix()));
			
			ofVec3f localTarget =  targetPosition * worldToBone;
            ofVec3f localEffector = effectorPosition * worldToBone;
			
			if (localEffector.distanceSquared(localTarget) < 0.02) continue;
			
			float angle = localTarget.angle(localEffector);
			if (angle != angle ){
				// this is where we test whether the angle is NaN
				//				ofLogWarning() << "angle is NAN" ;
				continue;
			}
			if (fabs(angle) < 0.001) continue;
			ofVec3f axis = -(localTarget.getCrossed(localEffector)).getNormalized();
			
			ofQuaternion q, p;
			p = bone->getOrientationAtJoint();

			ofVec3f jointOldAxis;
			float jointOldAngle;
			
			// we generate a quaternion with the new orientation
			q.makeRotate(angle, axis);
			q = p * q;	// this rotates by (adds the new angle to) the current orientation
			
			// calculate the total angle
			q.getRotate(jointOldAngle, jointOldAxis);
			// clamp to the max angle.
			q.makeRotate(ofClamp(jointOldAngle,-60,80), jointOldAxis);
			bone->setOrientationAtJoint(q);
			
			
		}
		
	}

}

// ----------------------------------------------------------------------




