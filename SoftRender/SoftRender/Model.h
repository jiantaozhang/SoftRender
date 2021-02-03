#pragma once

#include <vector>
#include <string>
#include "vec.h"
#include "tgaimage.h"

using namespace std;

class Model
{
public:
	Model(const char* file);
	~Model();

	int nverts();
	int nfaces();
	vec3 vert(int i);
	int* face(int i);	// int[3]

private:

	vector<vec3> _verts;
	vector<int*> _faces;
};


