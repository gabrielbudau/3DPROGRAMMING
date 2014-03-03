#include "V3.h"
using namespace std;

V3 V3 :: operator+(const V3&  other){
	float localX = x + other.x;
	float localY = y + other.y;
	float localZ = z + other.z;
	return V3(localX, localY, localZ);
}

V3 V3 :: operator-(const V3&  other){
	float localX = x - other.x;
	float localY = y - other.y;
	float localZ = z - other.z;
	return V3(localX, localY, localZ);
}
V3 V3 :: operator=(const V3& other){
	x = other.x;
	y = other.y;
	z = other.z;
	return *this;
}
V3 V3 :: operator*(const V3& o){
	return
		E3.sMult(x*o.y) +
		E1.sMult(y*o.z) +
		E2.sMult(z*o.x) -
		E1.sMult(z*o.y) -
		E2.sMult(x*o.z) -
		E3.sMult(y*o.x);

}
ostream& operator<<(ostream& outStream, V3& obj)
{
	outStream << '<' << obj.getX() << ", " << obj.getY() << ", " << obj.getZ() << '>';
	return outStream;
}
istream& operator>>(istream& inStream, V3& obj){
	cout << "x = ";
	inStream >> obj.x;
	cout << "y = ";
	inStream >> obj.y;
	cout << "z = ";
	inStream >> obj.z;
	return inStream;
}
float V3::getX() { return x; }
float V3::getY() { return y; }
float V3::getZ() { return z; }
void V3::setX(float _x){ x = _x; }
void V3::setY(float _y){ x = _y; }
void V3::setZ(float _z){ x = _z; }
V3 V3::sAdd(float s){
	return V3(x + s, y + s, z + s);
}
V3 V3::sSub(float s){
	return V3(x - s, y - s, z - s);
}
V3 V3::sMult(float s){
	return V3(x * s, y * s, z * s);
}
V3 V3::sDiv(float s){
	return V3(x / s, y / s, z / s);
}