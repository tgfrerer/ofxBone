//
//     _____    ___     
//    /    /   /  /     testVmdLoader
//   /  __/ * /  /__    (c) ponies & light, 2012. All rights reserved.
//  /__/     /_____/    poniesandlight.co.uk
//
//  ofxBone.h
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

#ifndef GUARD__testVmdLoader__ofxBone__
#define GUARD__testVmdLoader__ofxBone__

#include "ofMain.h"

// a bone is very similar to an ofNode, with the significant difference that
// bones parents and children are linked through joints.
// one bone's target is its child bone's joint:
//
//			o...joint of bone 0
//			§...target of bone 0, joint of bone 1
//			&...target of bone 1, joint of bone 2
//			>...target of bone 2
//
//			o---(0)---§----(1)---&---(2)---->



class ofxBone {
	tr1::weak_ptr<ofxBone>				parent;
	ofMatrix4x4							localTransformMatrix;
	ofMatrix4x4							localJointTransformMatrix;
	
	virtual void customDraw();
	void								setTransformMatrix(ofMatrix4x4 m_);
	
public:
	
	void								setOrientation(ofQuaternion orientation_);
	void								setOrientationAtJoint(ofQuaternion orientation_);

	void								setPosition(ofVec3f position_);
	void								setGlobalOrientation(ofQuaternion orientation_);
	void								setGlobalOrientationAtJoint(ofQuaternion orientation_);
	
	void								setGlobalPosition(ofVec3f position_);
	void								setParent(tr1::shared_ptr<ofxBone> parent_);
	
	void								draw();

	tr1::shared_ptr<ofxBone>			getParent();
	
	const ofQuaternion					getOrientation()				const;
	const ofQuaternion					getOrientationAtJoint()			const;
	const ofMatrix4x4					getLocalTransformMatrix()		const;
	const ofMatrix4x4					getJointTransformMatrix()		const;
	const ofVec3f						getJointPosition()				const;
	
	const ofMatrix4x4					getGlobalTransformMatrix()		const;
	const ofMatrix4x4					getGlobalJointTransformMatrix()	const;

	const ofQuaternion					getGlobalOrientationAtJoint()	const;
	const ofQuaternion					getGlobalOrientation()			const;
	const ofVec3f						getGlobalPosition()				const;
	const ofVec3f						getGlobalJointPosition()		const;
	
	
	ofxBone()
	:parent()	// will automatically initialize as a NULL shared ptr.
	{};
	
};

// --------------------------------------------------------
// this is only here for testing purposes:

class ofxBoneIKchain {
public:
	vector<tr1::shared_ptr<ofxBone> >		boneChain;
	tr1::shared_ptr<ofxBone>				endEffector;
	ofVec3f									targetPositionInGlobalSpace;
	
	
	void solve();
};




#endif /* defined(__testVmdLoader__ofxBone__) */
