

#include <iostream>
#include <fstream>

#include "tgaimage.h"
#include "model.h"
#include "draw.h"


const TGAColor red = TGAColor(255, 0, 0, 255);
const TGAColor white(255, 255, 255, 255);

const int height = 1280;
const int width = 1080;

vec3 light_dir(0, 0, -1);


TGAColor _rand_color()
{
	TGAColor color(rand() % 255, rand() % 255, rand() % 255);
	return color;
}

void african_head() {
	TGAImage img(width, height, TGAImage::RGB);

	// load
	Model* model = new Model("../resource/african_head/african_head.obj");

	// iterate model triangles
	int nfaces = model->nfaces();
	for (int i = 0; i < nfaces; i++)
	{
		int* face = model->face(i);

		vec3 v3_pts[3];
		model->face_verts(i, v3_pts);
		vec2 v2_pts[3];
		for (int idx = 0; idx < 3; idx++)
		{
			vec3 v0 = v3_pts[idx];

			int x0 = (v0.x + 1.f) * width / 2;
			int y0 = (v0.y + 1.f) * height / 2;

			v2_pts[idx] = vec2(x0, y0);
		}


		// normalized normal
		vec3 normal = cross(v3_pts[2] - v3_pts[1], v3_pts[1] - v3_pts[0]);
		normal = normal / normal.norm();
		//std::cout << "normal: " << normal << std::endl;

		// light_dir dot normal
		vec3 inv_light_dir = -1 * light_dir;
		double light_fac = std::max((double)0, normal * light_dir);

		// 这里模型的方向不太对 !

		int pixels = triangle(v2_pts, img, white * light_fac);
		//std::cout << "pixels: " << pixels << std::endl;
	}

	img.flip_vertically();
	img.write_tga_file("../output/e1.tga", false);

	delete model;
}

void draw_triangle()
{
	TGAImage img(width, height, TGAImage::RGB);

	vec2 points[3];
	points[0] = vec2(100, 100);
	points[1] = vec2(300, 700);
	points[2] = vec2(200, 900);

	triangle(points, img, red);
	img.write_tga_file("../output/triangle.tga");
}

int main(int argc, char** argv)
{
	african_head();
	draw_triangle();

	return 0;
}
