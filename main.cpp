#include <iostream>
#include <fstream>
#include "Edge.h"
#include "Shape.h"
#include "ShapeFile.h"
using namespace std;

int main(int argc, char** argv)
{
	ifstream infile;
	if (argc == 2)
	{
		infile.open(argv[1]);
	}
	if (!infile)
	{
		cerr << "No input file!" << endl;
		return -1;
	}
	ShapeFile sfile(infile);
	sfile.Generate();
	sfile.Print();
	return 0;
}