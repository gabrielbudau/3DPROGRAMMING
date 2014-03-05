#include <freeglut.h>
#include <stdio.h>
#include "Vector3.h"

void printVector(Vector3 &vec)
{
	printf("%4g, %4g, %4g\n", vec.x, vec.y, vec.z);
}
int main()
{
	printVector(Vector3(1, 2, 3) + Vector3(7, 2, 5));
	printVector(Vector3(3, 4, 5) - Vector3(2, 9, 3));
	printVector(Vector3(1, 2, 3) * 5);
	printf("%4g\n", Vector3(1, 2, 3).Dot(Vector3(3, 2, 1)));
	printVector(Vector3(1, 2, 3)*(Vector3(4, 5, 6)));
	system("PAUSE");
}