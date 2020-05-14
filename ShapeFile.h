#ifndef SHAPEFILE_H
#define SHAPEFILE_H


#include <iostream>
#include "Shape.h"
using namespace std;


class ShapeFile
{
public:
	ShapeFile(ifstream& f);
	void Generate();
	void Print();

private:
	vector<shared_ptr<Shape>> shapes_;
	vector<shared_ptr<Shape>> composieShapes_;
};
#endif