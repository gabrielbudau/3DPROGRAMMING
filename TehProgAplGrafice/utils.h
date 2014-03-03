#include <iostream>
#include <fstream>
#include <vector>
#include <glut.h> 
#include <utility>
#include "V3.h"
using namespace std;

#ifndef UTILS_H
#define UTILS_H

#define NOT_POLYGON 0
#define CONVEX 1
#define CONCAVE 2
#define NECOPLANARE 0
#define COPLANARE 1
#define COUNTERCLOCKWISE 3
#define CLOCKWISE 4

vector<V3> readFromFile(GLfloat points[][3], int size);
int testCC(vector<V3>& V);//test daca punctele date formeaza un poligon convex sau concav
int testC(vector<V3>& V);//testarea coplanaritatii
int ClockWise(vector<V3>& P, vector<double>& PointsType);//testeaza daca un poligon este Clockwise sau CounterClockwise
bool checkTriangle(unsigned int i, unsigned int j, unsigned int k, vector<V3> Points);
bool isInside(V3 a, V3 b, V3 c, V3 point);
float area(V3 a, V3 b, V3 c);

#endif
