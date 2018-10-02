#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/freeglut.h>
#include <iostream>
#include <cmath>
#include <algorithm>
#include "mathclass.h"

using namespace jhm;

GLdouble rotMatrix[16] =
{
	1, 0, 0, 0,
	0, 1, 0, 0,
	0, 0, 1, 0,
	0, 0, 0, 1
};

/* set global variables */
int width, height;

bool leftButton = false;
bool rightButton = false;
GLfloat mousePosX, mousePosY;
double lastX, lastY, lastZ;

int animateTime = 0;

double translateX;

double neckRotateAngleX;
double neckRotateAngleY;
double neckRotateAngleZ;

double rightUpperArmRotateAngleX;
double rightUpperArmRotateAngleY;
double rightUpperArmRotateAngleZ;
double rightLowerArmRotateAngleZ;
double rightHandRotateAngleX;
double rightHandRotateAngleY;
double rightHandRotateAngleZ;

double leftUpperArmRotateAngleX;
double leftUpperArmRotateAngleY;
double leftUpperArmRotateAngleZ;
double leftLowerArmRotateAngleZ;
double leftHandRotateAngleX;
double leftHandRotateAngleY;
double leftHandRotateAngleZ;

double rightUpperLegRotateAngleX;
double rightUpperLegRotateAngleY;
double rightUpperLegRotateAngleZ;
double rightLowerLegRotateAngleX;

double leftUpperLegRotateAngleX;
double leftUpperLegRotateAngleY;
double leftUpperLegRotateAngleZ;
double leftLowerLegRotateAngleX;

/* vectors that makes the rotation and translation of the cube */
vector eye(0.0f, 0.0f, 100.0f);
vector ori(0.0f, 0.0f, 0.0f);
vector up(0.0f, 1.0f, 0.0f);

vector loadBasisY() {
  return up.normalize();
}

vector loadBasisZ() {
  return (eye - ori).normalize();
}

vector loadBasisX() {
  return loadBasisY() * loadBasisZ();
}

void loadGlobalCoord()
{
	glLoadIdentity();
	gluLookAt(eye[0], eye[1], eye[2], ori[0], ori[1], ori[2], up[0], up[1], up[2]);
	glMultMatrixd(rotMatrix);
}

#define TRANSLATE_CONSTANT 0.5

void translate(vector v, double len) {
  vector transVec = v * len;
  eye += transVec;
  ori += transVec;
  loadGlobalCoord();
}

//------------------------------------------------------------------------
// Moves the screen based on mouse pressed button
//------------------------------------------------------------------------

void glutMotion(int x, int y)
{
  if (leftButton) {
    double trackballX = (x - 1000.0f) / 20;
    double trackballY = (1000.0f - y) / 20;
    double trackballZ;
    position tmp(trackballX, trackballY, 0);
    double length = tmp.norm();

    if (length <= 40.0f) trackballZ = sqrt(1600 - length * length);
    else {
      trackballX = trackballX * 40.0f / length;
      trackballY = trackballY * 40.0f / length;
      trackballZ = 0;
    }

    vector lastVec = loadBasisX() * lastX + loadBasisY() * lastY + loadBasisZ() * lastZ;
    vector currVec = loadBasisX() * trackballX + loadBasisY() * trackballY + loadBasisZ() * trackballZ;
    vector rotAxis = (currVec * lastVec).normalize();

    double cosAngle = (lastVec % currVec) / (len(lastVec) * len(currVec));
    double cosHalfAngle = sqrt((1 + cosAngle) / 2);
    double sinHalfAngle = sqrt(1 - cosHalfAngle * cosHalfAngle);

    quater rotQuater(cosHalfAngle, sinHalfAngle * rotAxis.x(), sinHalfAngle * rotAxis.y(), sinHalfAngle * rotAxis.z());
    eye = ori + rotate(rotQuater, eye - ori);
    up = rotate(rotQuater, up);

    lastX = trackballX;
    lastY = trackballY;
    lastZ = trackballZ;
  }
	if (rightButton) {
		double dx = x - mousePosX;
		double dy = y - mousePosY;

		mousePosX = x;
		mousePosY = y;

    vector transVec = loadBasisX() * -1 * dx + loadBasisY() * dy;
    position delta(dx, dy, 0);
    double len = delta.norm() * 0.002;
    translate(transVec, len);

		loadGlobalCoord();
	}
	return;
}

//------------------------------------------------------------------------
// Function that handles mouse input
//------------------------------------------------------------------------
void glutMouse(int button, int state, int x, int y)
{
	switch ( button )
	{
		case GLUT_LEFT_BUTTON:
      if (state == GLUT_DOWN)
      {
        lastX = (x - 1000.0f) / 20;
        lastY = (1000.0f - y) / 20;
        position tmp(lastX, lastY, 0);
        double length = tmp.norm();

        if (length <= 40.0f) lastZ = sqrt(1600 - length * length);
        else {
          lastX = lastX * 40.0f / length;
          lastY = lastY * 40.0f / length;
          lastZ = 0;
        }

        leftButton = true;
      }
      else if (state == GLUT_UP) leftButton = false;
			break;
		case GLUT_RIGHT_BUTTON:
			if (state == GLUT_DOWN)
			{
				mousePosX = x;
				mousePosY = y;
				rightButton = true;
			}
			else if (state == GLUT_UP)
			{
				rightButton = false;
			}
			break;
		case 3:break;
		default:break;
	}
	return;
}

void cube(position p1, position p2) {
  
  glColor3f(46.0 / 255, 46.0 / 255, 184.0 / 255);
  glBegin(GL_POLYGON);
  glVertex3f(p1.x(), p1.y(), p1.z());
  glVertex3f(p2.x(), p1.y(), p1.z());
  glVertex3f(p2.x(), p2.y(), p1.z());
  glVertex3f(p1.x(), p2.y(), p1.z());
  glEnd();
/*
  glColor3f(1.0, 1.0, 1.0);
  glBegin(GL_LINE_STRIP);
  glVertex3f(p1.x(), p1.y(), p1.z());
  glVertex3f(p2.x(), p1.y(), p1.z());
  glVertex3f(p2.x(), p2.y(), p1.z());
  glVertex3f(p1.x(), p2.y(), p1.z());
  glVertex3f(p1.x(), p1.y(), p1.z());
  glEnd();
*/
  glColor3f(46.0 / 255, 86.0 / 255, 184.0 / 255);
  glBegin(GL_POLYGON);
  glVertex3f(p1.x(), p1.y(), p1.z());
  glVertex3f(p1.x(), p2.y(), p1.z());
  glVertex3f(p1.x(), p2.y(), p2.z());
  glVertex3f(p1.x(), p1.y(), p2.z());
  glEnd();
/*
  glColor3f(1.0, 1.0, 1.0);
  glBegin(GL_LINE_STRIP);
  glVertex3f(p1.x(), p1.y(), p1.z());
  glVertex3f(p1.x(), p2.y(), p1.z());
  glVertex3f(p1.x(), p2.y(), p2.z());
  glVertex3f(p1.x(), p1.y(), p2.z());
  glVertex3f(p1.x(), p1.y(), p1.z());
  glEnd();
*/
  glColor3f(86.0 / 255, 46.0 / 255, 184.0 / 255);
  glBegin(GL_POLYGON);
  glVertex3f(p1.x(), p2.y(), p1.z());
  glVertex3f(p2.x(), p2.y(), p1.z());
  glVertex3f(p2.x(), p2.y(), p2.z());
  glVertex3f(p1.x(), p2.y(), p2.z());
  glEnd();
/*
  glColor3f(1.0, 1.0, 1.0);
  glBegin(GL_LINE_STRIP);
  glVertex3f(p1.x(), p2.y(), p1.z());
  glVertex3f(p2.x(), p2.y(), p1.z());
  glVertex3f(p2.x(), p2.y(), p2.z());
  glVertex3f(p1.x(), p2.y(), p2.z());
  glVertex3f(p1.x(), p2.y(), p1.z());
  glEnd();
*/
  glColor3f(46.0 / 255, 126.0 / 255, 184.0 / 255);
  glBegin(GL_POLYGON);
  glVertex3f(p2.x(), p2.y(), p1.z());
  glVertex3f(p2.x(), p1.y(), p1.z());
  glVertex3f(p2.x(), p1.y(), p2.z());
  glVertex3f(p2.x(), p2.y(), p2.z());
  glEnd();
/*
  glColor3f(1.0, 1.0, 1.0);
  glBegin(GL_LINE_STRIP);
  glVertex3f(p2.x(), p2.y(), p1.z());
  glVertex3f(p2.x(), p1.y(), p1.z());
  glVertex3f(p2.x(), p1.y(), p2.z());
  glVertex3f(p2.x(), p2.y(), p2.z());
  glVertex3f(p2.x(), p2.y(), p1.z());
  glEnd();
*/
  glColor3f(126.0 / 255, 46.0 / 255, 184.0 / 255);
  glBegin(GL_POLYGON);
  glVertex3f(p2.x(), p1.y(), p1.z());
  glVertex3f(p1.x(), p1.y(), p1.z());
  glVertex3f(p1.x(), p1.y(), p2.z());
  glVertex3f(p2.x(), p1.y(), p2.z());
  glEnd();
/*
  glColor3f(1.0, 1.0, 1.0);
  glBegin(GL_LINE_STRIP);
  glVertex3f(p2.x(), p1.y(), p1.z());
  glVertex3f(p1.x(), p1.y(), p1.z());
  glVertex3f(p1.x(), p1.y(), p2.z());
  glVertex3f(p2.x(), p1.y(), p2.z());
  glVertex3f(p2.x(), p1.y(), p1.z());
  glEnd();
*/
  glColor3f(46.0 / 255, 46.0 / 255, 184.0 / 255);
  glBegin(GL_POLYGON);
  glVertex3f(p1.x(), p1.y(), p2.z());
  glVertex3f(p1.x(), p2.y(), p2.z());
  glVertex3f(p2.x(), p2.y(), p2.z());
  glVertex3f(p2.x(), p1.y(), p2.z());
  glEnd();
/*
  glColor3f(1.0, 1.0, 1.0);
  glBegin(GL_LINE_STRIP);
  glVertex3f(p1.x(), p1.y(), p2.z());
  glVertex3f(p1.x(), p2.y(), p2.z());
  glVertex3f(p2.x(), p2.y(), p2.z());
  glVertex3f(p2.x(), p1.y(), p2.z());
  glVertex3f(p1.x(), p1.y(), p2.z());
  glEnd();
*/
}

void animate() {

  if (animateTime < 20) neckRotateAngleY = animateTime * 3;
  else if (animateTime < 60) neckRotateAngleY = 120 - animateTime * 3;
  else if (animateTime < 80) neckRotateAngleY = -240 + animateTime * 3;
  else neckRotateAngleY = 0;

  if (animateTime < 80) neckRotateAngleX = 0;
  else if (animateTime < 100) neckRotateAngleX = (animateTime - 80) * 3;
  else if (animateTime < 140) neckRotateAngleX = 120 - (animateTime - 80) * 3;
  else if (animateTime < 160) neckRotateAngleX = -240 + (animateTime - 80) * 3;
  else neckRotateAngleX = 0;

  if (animateTime < 160) neckRotateAngleZ = 0;
  else if (animateTime < 180) neckRotateAngleZ = (animateTime - 160) * 1.5;
  else if (animateTime < 220) neckRotateAngleZ = 60 - (animateTime - 160) * 1.5;
  else if (animateTime < 240) neckRotateAngleZ = -120 + (animateTime - 160) * 1.5;
  else neckRotateAngleZ = 0;

  if (animateTime < 250) rightUpperArmRotateAngleZ = 150;
  else if (animateTime < 300) rightUpperArmRotateAngleZ = 150 - (animateTime - 250) * 1.2;
  else rightUpperArmRotateAngleZ = 90;

  if (animateTime < 250) rightLowerArmRotateAngleZ = 30;
  else if (animateTime < 300) rightLowerArmRotateAngleZ = 30 - (animateTime - 250) * 0.6;
  else rightLowerArmRotateAngleZ = 0;

  if (animateTime < 250) leftUpperArmRotateAngleZ = -150;
  else if (animateTime < 300) leftUpperArmRotateAngleZ = -150 + (animateTime - 250) * 1.2;
  else leftUpperArmRotateAngleZ = -90;

  if (animateTime < 250) leftLowerArmRotateAngleZ = -30;
  else if (animateTime < 300) leftLowerArmRotateAngleZ = -30 + (animateTime - 250) * 0.6;
  else leftLowerArmRotateAngleZ = 0;

  if (animateTime < 320) leftHandRotateAngleZ = 0;
  else if (animateTime < 340) leftHandRotateAngleZ = (animateTime - 320) * 2.25;
  else if (animateTime < 360) leftHandRotateAngleZ = 180 - (animateTime - 320) * 6.75;
  else if (animateTime < 380) leftHandRotateAngleZ = -330 + (animateTime - 320) * 6;
  else if (animateTime < 400) leftHandRotateAngleZ = 120 - (animateTime - 320) * 1.5;
  else leftHandRotateAngleZ = 0;

  if (animateTime >= 340 && animateTime < 360) leftLowerArmRotateAngleZ = (animateTime - 340) * 1.5;
  if (animateTime >= 360 && animateTime < 380) leftLowerArmRotateAngleZ = 30 - (animateTime - 360) * 4.5;
  if (animateTime >= 380 && animateTime < 400) leftLowerArmRotateAngleZ = -60 + (animateTime - 380) * 3;
  if (animateTime >= 400) leftLowerArmRotateAngleZ = 0; 

  if (animateTime >= 360 && animateTime < 380) leftUpperArmRotateAngleZ = -90 + (animateTime - 360) * 1.5;
  if (animateTime >= 380 && animateTime < 400) leftUpperArmRotateAngleZ = -60 - (animateTime - 380) * 1.5;

  if (animateTime >= 380 && animateTime < 400) translateX = (animateTime - 380) * -0.1;

  
  if (animateTime < 420) rightHandRotateAngleZ = 0;
  else if (animateTime < 440) rightHandRotateAngleZ = (animateTime - 420) * -2.25;
  else if (animateTime < 460) rightHandRotateAngleZ = -180 + (animateTime - 420) * 6.75;
  else if (animateTime < 480) rightHandRotateAngleZ = 330 - (animateTime - 420) * 6;
  else if (animateTime < 500) rightHandRotateAngleZ = -120 + (animateTime - 420) * 1.5;
  else rightHandRotateAngleZ = 0;

  if (animateTime >= 440 && animateTime < 460) rightLowerArmRotateAngleZ = (animateTime - 440) * -1.5;
  if (animateTime >= 460 && animateTime < 480) rightLowerArmRotateAngleZ = -30 + (animateTime - 460) * 4.5;
  if (animateTime >= 480 && animateTime < 500) rightLowerArmRotateAngleZ = 60 - (animateTime - 480) * 3;
  if (animateTime >= 500) rightLowerArmRotateAngleZ = 0; 

  if (animateTime >= 460 && animateTime < 480) rightUpperArmRotateAngleZ = 90 - (animateTime - 460) * 1.5;
  if (animateTime >= 480 && animateTime < 500) rightUpperArmRotateAngleZ = 60 + (animateTime - 480) * 1.5;

  if (animateTime >= 480 && animateTime < 500) translateX = -2 + (animateTime - 480) * 0.1;


  if (animateTime >= 550 && animateTime < 570) rightUpperLegRotateAngleX = (animateTime - 550) * -2.25;
  if (animateTime >= 570 && animateTime < 590) rightUpperLegRotateAngleX = -45 + (animateTime - 570) * 2.25;
  if (animateTime >= 590) rightUpperLegRotateAngleX = 0;

  if (animateTime >= 550 && animateTime < 570) rightLowerLegRotateAngleX = (animateTime - 550) * -2.25;
  if (animateTime >= 570 && animateTime < 590) rightLowerLegRotateAngleX = -45 + (animateTime - 570) * 2.25;
  if (animateTime >= 590) rightLowerLegRotateAngleX = 0;

  if (animateTime >= 590 && animateTime < 610) leftUpperLegRotateAngleX = (animateTime - 590) * -2.25;
  if (animateTime >= 610 && animateTime < 630) leftUpperLegRotateAngleX = -45 + (animateTime - 610) * 2.25;
  if (animateTime >= 630) leftUpperLegRotateAngleX = 0;

  if (animateTime >= 590 && animateTime < 610) leftLowerLegRotateAngleX = (animateTime - 590) * -2.25;
  if (animateTime >= 610 && animateTime < 630) leftLowerLegRotateAngleX = -45 + (animateTime - 610) * 2.25;
  if (animateTime >= 630) leftLowerLegRotateAngleX = 0;
  
  
  if (animateTime >= 630 && animateTime < 650) rightUpperLegRotateAngleX = (animateTime - 630) * -2.25;
  if (animateTime >= 650 && animateTime < 670) rightUpperLegRotateAngleX = -45 + (animateTime - 650) * 2.25;
  if (animateTime >= 670) rightUpperLegRotateAngleX = 0;

  if (animateTime >= 630 && animateTime < 650) rightLowerLegRotateAngleX = (animateTime - 630) * -2.25;
  if (animateTime >= 650 && animateTime < 670) rightLowerLegRotateAngleX = -45 + (animateTime - 650) * 2.25;
  if (animateTime >= 670) rightLowerLegRotateAngleX = 0;

  if (animateTime >= 670 && animateTime < 690) leftUpperLegRotateAngleX = (animateTime - 670) * -2.25;
  if (animateTime >= 690 && animateTime < 710) leftUpperLegRotateAngleX = -45 + (animateTime - 690) * 2.25;
  if (animateTime >= 710) leftUpperLegRotateAngleX = 0;

  if (animateTime >= 670 && animateTime < 690) leftLowerLegRotateAngleX = (animateTime - 670) * -2.25;
  if (animateTime >= 690 && animateTime < 710) leftLowerLegRotateAngleX = -45 + (animateTime - 690) * 2.25;
  if (animateTime >= 710) leftLowerLegRotateAngleX = 0;

  
  if (animateTime >= 710 && animateTime < 730) rightUpperLegRotateAngleX = (animateTime - 710) * -2.25;
  if (animateTime >= 730 && animateTime < 750) rightUpperLegRotateAngleX = -45 + (animateTime - 730) * 2.25;
  if (animateTime >= 750) rightUpperLegRotateAngleX = 0;

  if (animateTime >= 710 && animateTime < 730) rightLowerLegRotateAngleX = (animateTime - 710) * -2.25;
  if (animateTime >= 730 && animateTime < 750) rightLowerLegRotateAngleX = -45 + (animateTime - 730) * 2.25;
  if (animateTime >= 750) rightLowerLegRotateAngleX = 0;

  if (animateTime >= 750 && animateTime < 770) leftUpperLegRotateAngleX = (animateTime - 750) * -2.25;
  if (animateTime >= 770 && animateTime < 790) leftUpperLegRotateAngleX = -45 + (animateTime - 770) * 2.25;
  if (animateTime >= 790) leftUpperLegRotateAngleX = 0;

  if (animateTime >= 750 && animateTime < 770) leftLowerLegRotateAngleX = (animateTime - 750) * -2.25;
  if (animateTime >= 770 && animateTime < 790) leftLowerLegRotateAngleX = -45 + (animateTime - 770) * 2.25;
  if (animateTime >= 790) leftLowerLegRotateAngleX = 0;

}

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	loadGlobalCoord();

  // Body
  glPushMatrix();
  {
  glTranslatef(translateX, 0, 0);
  
  position bodyPoint1(-0.7, 0, -0.7);
  position bodyPoint2(0.7, 8, 0.7);
  cube(bodyPoint1, bodyPoint2);

  /***************************************************
    Neck + Head
  ***************************************************/  

  glPushMatrix();
  {
    glTranslatef(0.0, 9.0, 0.0);
    glRotatef(neckRotateAngleX, 1, 0, 0);
    glRotatef(neckRotateAngleY, 0, 1, 0);
    glRotatef(neckRotateAngleZ, 0, 0, 1);

    // Neck
    position neckPoint1(-0.7, -0.5, -0.7);
    position neckPoint2(0.7, 1.0, 0.7);
    cube(neckPoint1, neckPoint2);

    glPushMatrix();
    {
      glTranslatef(0.0, 1.5, 0.0);

      // Head
      position headPoint1(-1.0, 0, -1.0);
      position headPoint2(1.0, 3, 1.0);
      cube(headPoint1, headPoint2);
    }
    glPopMatrix();
  }
  glPopMatrix();

  /***************************************************
    Right (Arm + Hand)
  ***************************************************/  

  glPushMatrix();
  {
    glTranslatef(-2.0, 8.0, 0.0);
    glRotatef(rightUpperArmRotateAngleX, 1, 0, 0);
    glRotatef(rightUpperArmRotateAngleY, 0, 1, 0);
    glRotatef(rightUpperArmRotateAngleZ, 0, 0, 1);

    // Right Upper Arm
    position rightUpArmPoint1(-0.7, 0, -0.7);
    position rightUpArmPoint2(0.7, 5, 0.7);
    cube(rightUpArmPoint1, rightUpArmPoint2);

    glPushMatrix();
    {
      glTranslatef(0.0, 5.5, 0.0);
      glRotatef(rightLowerArmRotateAngleZ, 0, 0, 1);

      // Right Lower Arm
      position rightDownArmPoint1(-0.7, 0, -0.7);
      position rightDownArmPoint2(0.7, 5, 0.7);
      cube(rightDownArmPoint1, rightDownArmPoint2);

      glPushMatrix();
      {
        glTranslatef(0.0, 5.5, 0.0);
        glRotatef(rightHandRotateAngleX, 1, 0, 0);
        glRotatef(rightHandRotateAngleY, 0, 1, 0);
        glRotatef(rightHandRotateAngleZ, 0, 0, 1);

        // Right Hand
        position rightHandPoint1(-0.7, 0, -0.7);
        position rightHandPoint2(0.7, 1.5, 0.7);
        cube(rightHandPoint1, rightHandPoint2);
      }
      glPopMatrix();
    }
    glPopMatrix();
  }
  glPopMatrix();
  
  /***************************************************
    Left (Arm + Hand)
  ***************************************************/  

  glPushMatrix();
  {
    glTranslatef(2.0, 8.0, 0.0);
    glRotatef(leftUpperArmRotateAngleX, 1, 0, 0);
    glRotatef(leftUpperArmRotateAngleY, 0, 1, 0);
    glRotatef(leftUpperArmRotateAngleZ, 0, 0, 1);

    // Left Upper Arm
    position leftUpArmPoint1(-0.7, 0, -0.7);
    position leftUpArmPoint2(0.7, 5, 0.7);
    cube(leftUpArmPoint1, leftUpArmPoint2);

    glPushMatrix();
    {
      glTranslatef(0.0, 5.5, 0.0);
      glRotatef(leftLowerArmRotateAngleZ, 0, 0, 1);

      // Left Lower Arm
      position leftDownArmPoint1(-0.7, 0, -0.7);
      position leftDownArmPoint2(0.7, 5, 0.7);
      cube(leftDownArmPoint1, leftDownArmPoint2);

      glPushMatrix();
      {
        glTranslatef(0.0, 5.5, 0.0);
        glRotatef(leftHandRotateAngleX, 1, 0, 0);
        glRotatef(leftHandRotateAngleY, 0, 1, 0);
        glRotatef(leftHandRotateAngleZ, 0, 0, 1);
        
        // Left Hand
        position leftHandPoint1(-0.7, 0, -0.7);
        position leftHandPoint2(0.7, 1.5, 0.7);
        cube(leftHandPoint1, leftHandPoint2);
      }
      glPopMatrix();
    }
    glPopMatrix();
  }
  glPopMatrix();

  /***************************************************
    Right (Leg + Foot)
  ***************************************************/  

  glPushMatrix();
  {
    glTranslatef(-1.5, -0.5, 0.0);
    glRotatef(rightUpperLegRotateAngleX, 1, 0, 0);
    glRotatef(rightUpperLegRotateAngleY, 0, 1, 0);
    glRotatef(rightUpperLegRotateAngleZ, 0, 0, 1);

    // Right Upper Leg
    position rightUpLegPoint1(-0.7, 0, -0.7);
    position rightUpLegPoint2(0.7, 7, 0.7);
    cube(rightUpLegPoint1, rightUpLegPoint2);

    glPushMatrix();
    {
      glTranslatef(0.0, 7.5, 0.0);
      glRotatef(rightLowerLegRotateAngleX, 1, 0, 0);

      // Right Lower Leg
      position rightDownLegPoint1(-0.7, 0, -0.7);
      position rightDownLegPoint2(0.7, 7, 0.7);
      cube(rightDownLegPoint1, rightDownLegPoint2);

      glPushMatrix();
      {
        glTranslatef(0.0, 7.5, 0.0);
        
        // Right Foot
        position rightFootPoint1(-0.7, 0, -0.7);
        position rightFootPoint2(0.7, 1.5, 1.7);
        cube(rightFootPoint1, rightFootPoint2);
      }
      glPopMatrix();
    }
    glPopMatrix();
  }
  glPopMatrix();

  /***************************************************
    Left (Leg + Foot)
  ***************************************************/  

  glPushMatrix();
  {
    glTranslatef(1.5, -0.5, 0.0);
    glRotatef(leftUpperLegRotateAngleX, 1, 0, 0);
    glRotatef(leftUpperLegRotateAngleY, 0, 1, 0);
    glRotatef(leftUpperLegRotateAngleZ, 0, 0, 1);

    // Left Upper Leg
    position leftUpLegPoint1(-0.7, 0, -0.7);
    position leftUpLegPoint2(0.7, 7, 0.7);
    cube(leftUpLegPoint1, leftUpLegPoint2);

    glPushMatrix();
    {
      glTranslatef(0.0, 7.5, 0.0);
      glRotatef(leftLowerLegRotateAngleX, 1, 0, 0);

      // Left Lower Leg
      position leftDownLegPoint1(-0.7, 0, -0.7);
      position leftDownLegPoint2(0.7, 7, 0.7);
      cube(leftDownLegPoint1, leftDownLegPoint2);

      glPushMatrix();
      {
        glTranslatef(0.0, 7.5, 0.0);
        
        // Left Foot
        position leftFootPoint1(-0.7, 0, -0.7);
        position leftFootPoint2(0.7, 1.5, 1.7);
        cube(leftFootPoint1, leftFootPoint2);
      }
      glPopMatrix();
    }
    glPopMatrix();
  }
  glPopMatrix();
  }
  glPopMatrix();

  animate();

  glutSwapBuffers();
}

void resize(int w, int h) {
	width = w;
	height = h;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (GLfloat)w / (GLfloat)h, .1f, 500.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void init() {      
  eye.setValue(0.0f, 0.0f, 100.0f);
  ori.setValue(0.0f, 0.0f, 0.0f);
  up.setValue(0.0f, 1.0f, 0.0f);
  animateTime = 0;

  translateX = 0;

  neckRotateAngleX = 0;
  neckRotateAngleY = 0;
  neckRotateAngleZ = 0;

  rightUpperArmRotateAngleX = 0;
  rightUpperArmRotateAngleY = 0;
  rightUpperArmRotateAngleZ = 150;
  rightLowerArmRotateAngleZ = 30;
  rightHandRotateAngleX = 0;
  rightHandRotateAngleY = 0;
  rightHandRotateAngleZ = 0;

  leftUpperArmRotateAngleX = 0;
  leftUpperArmRotateAngleY = 0;
  leftUpperArmRotateAngleZ = -150;
  leftLowerArmRotateAngleZ = -30;
  leftHandRotateAngleX = 0;
  leftHandRotateAngleY = 0;
  leftHandRotateAngleZ = 0;

  rightUpperLegRotateAngleX = 0;
  rightUpperLegRotateAngleY = 0;
  rightUpperLegRotateAngleZ = 180;
  rightLowerLegRotateAngleX = 0;

  leftUpperLegRotateAngleX = 0;
  leftUpperLegRotateAngleY = 0;
  leftUpperLegRotateAngleZ = 180;
  leftLowerLegRotateAngleX = 0;

  loadGlobalCoord();
}

void keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 27:
		exit(0);
		break;
  case 'i':
    init();
    break;
  case 'w':
    translate(loadBasisY(), -1 * TRANSLATE_CONSTANT);
    break;
  case 'a':
    translate(loadBasisX(), TRANSLATE_CONSTANT);
    break;
  case 's':
    translate(loadBasisY(), TRANSLATE_CONSTANT);
    break;
  case 'd':
    translate(loadBasisX(), -1 * TRANSLATE_CONSTANT);
    break;
  case 'q':
    animateTime -= 50;
    animateTime = std::max(animateTime, 0);
    break;
  case 'e':
    animateTime += 25;
    break;
	default:
		break;
	}
}


unsigned timeStep = 30;
void Timer(int unused)
{
	/* call the display callback and forces the current window to be displayed */
	glutPostRedisplay();
	glutTimerFunc(timeStep, Timer, 0);
  animateTime++;

  if (animateTime > 800) animateTime = 0;
}

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(2000, 2000);
	glutInitWindowPosition( 50, 0 );
	glutCreateWindow("Example");
  glEnable(GL_DEPTH_TEST);
  init();

//  glClearColor(1.0, 1.0, 1.0, 0.0);
	glutReshapeFunc(resize);
	glutDisplayFunc(display);
	glutTimerFunc(timeStep, Timer, 0);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(glutMouse);
	glutMotionFunc(glutMotion);

	glutMainLoop();

	return 0;
}
