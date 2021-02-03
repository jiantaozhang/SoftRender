#include "Model.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>


using namespace std;

Model::Model(const char* file) {
	ifstream fin;
	fin.open(file, ifstream::in);
	if (fin.fail()) return;
	string line;
	while (!fin.eof()) {
		getline(fin, line);
		istringstream iss(line.c_str());
		char trash;
		if (!line.compare(0, 2, "v ")) {
			// 顶点
			iss >> trash;
			vec3 v;
			for (int i = 0; i < 3; i++)
				iss >> v.data[i];
			this->_verts.push_back(v);
		}
		else if (!line.compare(0, 2, "f ")) {
			// 面顶点
			int* f = new int[3];
			int itrash, idx;
			iss >> trash;
			// 文件的数据格式就是  [int][char][int][char][int]
			for (int i = 0; i < 3; i++)
			{
				if (iss >> idx >> trash >> itrash >> trash >> itrash)
				{
					idx--;
					f[i] = idx;
				}
				else
					break;

			}
			this->_faces.push_back(f);
			cout << "face: " << f[0] << ", " << f[1] << ", " << f[2] << endl;
		}
	}
	cout << "Model Info" << endl;
	cout << "## vertice count : " << _verts.size() << endl << "## face count: " << _faces.size() << endl;
}

Model::~Model()
{
}

int Model::nverts()
{
	return _verts.size();
}

int Model::nfaces()
{
	return _faces.size();
}

vec3 Model::vert(int i)
{
	return _verts[i];
}

int* Model::face(int i)
{
	return _faces[i];
}
