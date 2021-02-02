

#include <iostream>
#include <fstream>

#include "tgaimage.h"
#include "model.h"
#include "common.h"


const TGAColor red = TGAColor(255, 0, 0, 255);
//const TGAColor rr(255, 0, 0, 255);


int main(int argc, char** argv)
{
	// load
	Model* m = new Model("obj/african_head.obj");

	// iterate model triangles
	int nfaces = m->nfaces();
	for (int i = 0; i < nfaces; i++)
	{

		std::vector<int> face = m->m

	}



	TGAImage img(100, 100, TGAImage::RGB);

	line(25, 25, 75, 75, img, red);

	img.flip_vertically();
	img.write_tga_file("../output/e1.tga");

	return 0;
}
