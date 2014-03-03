#include <iostream>
using namespace std;

#ifndef V3_H
#define V3_H
class V3{
public:
	V3() : x(0.0), y(0.0), z(0.0) {};
	V3(float _x, float _y, float _z)
		: x(_x), y(_y), z(_z){};
	V3(const V3 &obj){
		x = obj.x;
		y = obj.y;
		z = obj.z;
	}
	float getX();
	float getY();
	float getZ();
	void setX(float _x);
	void setY(float _y);
	void setZ(float _z);
	V3 operator+(const V3& other);
	V3 operator-(const V3& other);
	V3 operator=(const V3& other);
	V3 operator*(const V3& other);
	friend ostream& operator<<(ostream& outStream, V3& obj);
	friend istream& operator>>(istream& inStream, V3& obj);
	V3 sMult(float scalar);
	V3 sDiv(float scalar);
	V3 sAdd(float scalar);
	V3 sSub(float scalar);
private:
	float x, y, z;

};
#endif

#ifndef E1
#define E1 V3(1.0, 0.0, 0.0)
#endif

#ifndef E2
#define E2 V3(0.0, 1.0, 0.0)
#endif

#ifndef E3
#define E3 V3(0.0, 0.0, 1.0)
#endif