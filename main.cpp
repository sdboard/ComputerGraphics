//
//  main.cpp
//  OpenGL_test
//
//  Created by Stephen Board on 1/10/18.
//  Copyright © 2018 Stephen Board. All rights reserved.
//  not really

#include <vector>
#include <iostream>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#include "Chaikins.hpp"


using namespace std;

vector<Coordinate> generate_points(vector<Coordinate> control_points){
 
    vector<Coordinate> points;
    
    if (control_points.size() == 1) {
        
        return control_points;
        
    } else {
        
        // Iterate through our control points
        for (int p = 0; p < (control_points.size()-1); p++) {
            
            Coordinate v0 = control_points[p];
            
            Coordinate v1 = control_points[p+1];
            
            // Find the Midpoint
            GLfloat newX = (v0.x + v1.x)/2;
            GLfloat newY = (v0.y + v1.y)/2;
            
            
            points.push_back(Coordinate(newX, newY));
            
        }
        
        vector<Coordinate> toReturn = {control_points[0]};
        
        vector<Coordinate> recurse = generate_points(points);
        
        for(int i = 0; i < recurse.size(); i++) {
            toReturn.push_back((recurse[i]));
        }
        
        toReturn.push_back(control_points[control_points.size()-1]);
        
        return toReturn;
    }
}

vector<Coordinate> recursive_call(vector<Coordinate> control_points){
    vector<Coordinate> toReturn = generate_points(control_points);
    for (int i = 0; i < 3; i ++){
        toReturn = generate_points(toReturn);
    }
    
    return toReturn;
}

void setup() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // clears the screen
}




void display(){
    //drawings
    
    
    Coordinate a = Coordinate(-0.6f, 0.0f);
    Coordinate b = Coordinate(-0.4f, 0.6f);
    Coordinate c = Coordinate(0.4f, 0.6f);
    Coordinate d = Coordinate(0.6f, 0.0f);
    
    vector<Coordinate> points = {a, b, c, d};
    
    vector<Coordinate> newPoints;
    
    newPoints = recursive_call(points);
    
    
    
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glColor3f(0.0f, 0.0f, 0.0f); //set our color to BLACK
    
    
    glBegin(GL_LINES);
    
    for (int i = 0; i < newPoints.size()-1; i++) {
        
    glVertex2f(newPoints[i].x, newPoints[i].y);
    glVertex2f(newPoints[i+1].x, newPoints[i+1].y);

    }
    
    glVertex2f(a.x, a.y);
    glVertex2f(b.x, b.y);
    glVertex2f(b.x, b.y);
    glVertex2f(c.x, c.y);
    glVertex2f(c.x, c.y);
    glVertex2f(d.x, d.y);

    
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
    GLfloat x=0.6, y=0.4;
    Coordinate P(x,y);
    std::cout << P.x<<"\n";
    
    
    
    return 0;
}
