

#include <iostream>
#include <fstream>

#include "tgaimage.h"
#include "model.h"
#include "common.h"


const TGAColor red = TGAColor(255, 0, 0, 255);
const TGAColor white(255, 255, 255, 255);

const int height = 1280;
const int width = 1080;


int main(int argc, char** argv)
{
	TGAImage img(width, height, TGAImage::RGB);

	// load
	Model* model = new Model("../resource/african_head/african_head.obj");

	// iterate model triangles
	int nfaces = model->nfaces();
	for (int i = 0; i < nfaces; i++)
	{
		int* face = model->face(i);

		for (int face_idx = 0; face_idx < 3; face_idx++)
		{
			vec3 v0 = model->vert(face[face_idx]);
			vec3 v1 = model->vert(face[(face_idx + 1) % 3]);

			int x0 = (v0.x + 1.f) * width / 2;
			int y0 = (v0.y + 1.f) * height / 2;
			int x1 = (v1.x + 1.f) * width / 2;
			int y1 = (v1.y + 1.f) * height / 2;
				
			line(x0, y0, x1, y1, img, red);
		}
	}

	img.flip_vertically();
	img.write_tga_file("../output/e1.tga", false);

	delete model;

	return 0;
}
