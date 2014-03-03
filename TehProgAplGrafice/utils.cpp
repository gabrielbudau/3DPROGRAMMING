#include "utils.h"
using namespace std;

vector<V3> readFromFile(GLfloat points[][3], int size){
	vector<V3> V;
	ifstream f("puncte.txt");
	for (int i = 0; i < size; i ++)
		V.push_back(V3(points[i][0], points[i][1], points[i][2]));
	f.close();
	return V;
}

int testCC(vector<V3>& P){
	vector<V3> V;
	int pos = 0, neg = 0, max;
	/*how many negative and positive elements are in V <0, 0, pos/neg> */

	if (P.size() > 2){
		for (unsigned int i = 0; i < P.size(); i++){
			if (i == P.size() - 2)
				V.push_back((P.at(i + 1) - P.at(i)) * ((P.at(0) - P.at(i + 1))));
			else if (i == P.size() - 1)
				V.push_back((P.at(0) - P.at(i)) * ((P.at(1) - P.at(0))));
			else
				V.push_back((P.at(i + 1) - P.at(i)) * ((P.at(i + 2) - P.at(i + 1))));
		}
	}
	else{
		return NOT_POLYGON;
	}



	for (unsigned int i = 0; i < V.size(); i++){
		if (V.at(i).getZ() < 0)
			neg++;
		else pos++;
	}
	max = pos;
	max = (neg > max) ? neg : max;
	for (unsigned int i = 0; i < V.size(); i++){
		if (max == pos)
		if (V.at(i).getZ() < 0)
			return CONCAVE;
		if (max == neg)
		if (V.at(i).getZ() >= 0)
			return CONCAVE;
	}
	return CONVEX;
	return 0;
}

int testC(vector<V3>& P){
	vector<V3> V;
	bool flag = true; //presupunem ca punctele sunt coplanare
	for (unsigned int i = 1; i < P.size(); i++){
		V.push_back(P.at(i) - P.at(0));
	}
	cout << "\n";
	for (unsigned int i = 0; i < V.size(); i++){
		cout << V.at(i);
		cout << "\n";
	}

	for (unsigned int i = 0; i < V.size(); i++){
		if (!(V.at(i).getZ() == 0.0)){
			flag = false;
			break;
		}
	}
	return flag ? COPLANARE : NECOPLANARE;
}

int ClockWise(vector<V3>& P, vector<double>& PType)
{
	int i, j, k;
	int n = P.size();
	int count = 0;
	double z;

	if (n < 3)
		return -1;

	for (i = 0; i < n; i++) {
		j = (i + 1) % n;
		k = (i + 2) % n;
		z = (P.at(j).getX() - P.at(i).getX()) * (P.at(k).getY() - P.at(j).getY());
		z -= (P.at(j).getY() - P.at(i).getY()) * (P.at(k).getX() - P.at(j).getX());
		PType.push_back(z);
		if (z < 0)
			count--;
		else if (z > 0)
			count++;
	}

	if (count > 0)
		return(COUNTERCLOCKWISE);
	else if (count < 0)
		return(CLOCKWISE);
	else
		return -1;
}

bool checkTriangle(unsigned int i, unsigned int j, unsigned int k, vector<V3> Points){
	bool isinside = false;
	unsigned int q;
	for (q = 0; q < Points.size(); q++){
		if ((q != i) && (q != j) && (q != k)){
			isinside = isInside(Points.at(i), Points.at(j), Points.at(k), Points.at(q));
			if (isinside)
				return isinside;
		}
	}
	return isinside;
}

/* A utility function to calculate area of triangle formed by (x1, y1),
(x2, y2) and (x3, y3) */
float area(V3 a, V3 b, V3 c)
{
	return abs((a.getX()*(b.getY() - c.getY()) + b.getX()*(c.getY() - a.getY()) + c.getX()*(a.getY() - b.getY())) / 2.0);
}

/* A function to check whether point P(x, y) lies inside the triangle formed
by A(x1, y1), B(x2, y2) and C(x3, y3) */
bool isInside(V3 a, V3 b, V3 c, V3 point)
{
	/* Calculate area of triangle ABC */
	float A = area(a, b, c);

	/* Calculate area of triangle PBC */
	float A1 = area(point, b, c);

	/* Calculate area of triangle PAC */
	float A2 = area(a, point, c);

	/* Calculate area of triangle PAB */
	float A3 = area(a, b, point);

	/* Check if sum of A1, A2 and A3 is same as A */
	return (A == A1 + A2 + A3);
}