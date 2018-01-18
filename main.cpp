//
//  main.cpp
//  OpenGL_test
//
//  Created by Stephen Board on 1/10/18.
//  Copyright © 2018 Stephen Board. All rights reserved.
//  not really

#include <iostream>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>

void setup() {
    
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // clears the screen
    
}



void display(){
    //where we do all the drawings
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glColor3f(0.0f, 0.0f, 0.0f); //set our color to BLACK
    
    // draw 4 points wtih coordinates (x,y) counter clockwise
    // (0.5,0.5), (-0.5,0.5), (0.5,-0.5), (-0.5,-0.5)
    // origin is at 0,0
    glBegin(GL_POINTS);
//    glVertex2f(0.5f, 0.5f);
//    glVertex2f(-0.5f, 0.5f);
//    glVertex2f(-0.5f, -0.5f);
//    glVertex2f(0.5f, -0.5f);
    glEnd();
    
    //draw the holy cross
    
    glBegin(GL_LINES);
    
    glVertex2f(0.0f, 0.0f);
    glVertex2f(0.0f, 0.6f);
    
    glVertex2f(-0.45f, 0.6f);
    glVertex2f(0.0f, 0.6f);
    
    glVertex2f(0.0f, 0.0f);
    glVertex2f(0.0f, -0.6f);
    
    glVertex2f(0.45f, -0.6f);
    glVertex2f(0.0f, -0.6f);
    
    glVertex2f(0.0f, 0.0f);
    glVertex2f(-0.45f, 0.0f);
    
    glVertex2f(-0.45f, 0.0f);
    glVertex2f(-0.45f, -0.6f);
    
    glVertex2f(0.0f, 0.0f);
    glVertex2f(0.45f, 0.0f);
    
    glVertex2f(0.45f, 0.0f);
    glVertex2f(0.45f, 0.6f);
    glLineWidth(20.5f);
    
    glEnd();
    glutSwapBuffers();
}


int main(int argc, char * argv[]) {
    // insert code here...
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB |GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(800,600);
    glutCreateWindow("My Window");
    setup();
    glutDisplayFunc(display);
    glutMainLoop();
    
    
    
    return 0;
}
