

#include <iostream>
#include <fstream>

#include "tgaimage.h"
#include "model.h"
#include "draw.h"
#include "main.h"
#include "vec.h"
#include "buffer.h"



// 这一坨全局变量 就当做渲染的上下文进行处理

const TGAColor red = TGAColor(255, 0, 0, 255);
const TGAColor green = TGAColor(0, 255, 0, 255);
const TGAColor white(255, 255, 255, 255);

const int height = 1000;
const int width = 1000;

vec3 world2Screen(vec3 w) { 
	int scale = std::min(height, width);
	return vec3((w.x + 1) / 2 * scale, (w.y + 1) / 2 * scale, w.z); 
}

vec3 light_dir(0, 0, -1);	// 平行光自身的方向
TGAColor _grayscale(double s) { return TGAColor(255 * s, 255 * s, 255 * s); }

template<typename T>
buffer<T>* _new_buffer() { return new buffer<T>(width, height); }




TGAColor _rand_color()
{
	TGAColor color(rand() % 255, rand() % 255, rand() % 255);
	return color;
}


int draw_triangle(
	const vec3* verts,
	const TGAColor& color,
	buffer<float>* zbuffer,
	buffer<TGAColor>* framebuffer
)
{

	vec2 vpixels[3];
	for (int i = 0; i < 3; i++)
	{
		vec3 screen_pos = world2Screen(verts[i]);
		vpixels[i] = screen_pos;
	}

	// 确定最小AABB， 减少后续遍历的次数
	// 这里的包围盒 只处理了 xy 平面  （z轴就由深度进行控制了，这里不处理)
	vec2 min(double_max, double_max);
	vec2 max(0, 0);
	for (int i = 0; i < 3; i++)
	{
		vec2 p = vpixels[i];
		min.x = std::min(p.x, min.x);
		min.y = std::min(p.y, min.y);

		max.x = std::max(p.x, max.x);
		max.y = std::max(p.y, max.y);
	}


	// 按照逆时针的顺序  连接三角形的三条边
	vec2 v_ab = vpixels[1] - vpixels[0];
	vec2 v_bc = vpixels[2] - vpixels[1];
	vec2 v_ca = vpixels[0] - vpixels[2];


	// 记录像素的填充个数
	int cnt = 0;

	// 遍历 AABB 中的所有像素
	for (int y = min.y; y <= max.y; y++)
	{
		for (int x = min.x; x <= max.x; x++)
		{
			// 当前的像素坐标
			vec2 pixel = vec2(x, y);

			// 精确判断是否在三角形内部
			vec2 v_ap = pixel - vpixels[0];
			if (cross(v_ab, v_ap) < 0)
				continue;

			vec2 v_bp = pixel - vpixels[1];
			if (cross(v_bc, v_bp) < 0)
				continue;

			vec2 v_cp = pixel - vpixels[2];
			if (cross(v_ca, v_cp) < 0)
				continue;

			// 通过三个顶点的重心算法  来计算一个 depth 的估值
			vec3 center = barycenter(vpixels, pixel);
			float z = 0;
			for (int i = 0; i < 3; i++)
			{
				z += verts[i].z * center.data[i];
			}

			// 只有 depth 比当前的大，才会进行覆盖
			if (z < zbuffer->Get(x, y))
				continue;

			zbuffer->Set(x, y, z);
			framebuffer->Set(x, y, color);
			++cnt;
		}
	}
	return cnt;
}


void _write_img(const buffer<TGAColor>* framebuffer, TGAImage* img)
{
	int width = img->get_width();
	int height = img->get_height();

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			TGAColor color = framebuffer->Get(x, y);
			img->set(x, y, color);
		}
	}
}

void _write_z_img(const buffer<float>* zbuffer, TGAImage* img)
{

	int width = img->get_width();
	int height = img->get_height();

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			double z = zbuffer->Get(x, y);
			/*if (z > 0)
				img->set(x, y, green);*/
			TGAColor color = _grayscale(z);
			img->set(x, y, color);
		}
	}
}


void african_head() {

	// load
	Model* model = new Model("../resource/african_head/african_head.obj");

	fstream fs("../output/log.txt", ios::trunc | ios::out);


	// buffer
	buffer<TGAColor>* framebuffer = new buffer<TGAColor>(width, height);
	buffer<float>* zbuffer = new buffer<float>(width, height);
	zbuffer->Fill(0);

	float albedo_r = 0;
	float albedo_g = 0;
	float albedo_a = 0;

	// iterate model triangles
	int nfaces = model->nfaces();
	for (int i = 0; i < nfaces; i++)	// todo  hh 先画一半 速度快一点
	{
		fs << "face: " << i << std::endl;
		int* face = model->face(i);

		vec3 v3_pts[3];
		model->face_verts(i, v3_pts);
		fs << "\tverts:" << v3_pts[0] << ", " << v3_pts[1] << ", " << v3_pts[2] << std::endl;

		// normalized normal
		vec3 normal = cross(v3_pts[2] - v3_pts[1], v3_pts[1] - v3_pts[0]);
		normal = normal / normal.norm();
		fs << "\t" << "normal: " << normal << std::endl;

		// light_dir dot normal
		vec3 inv_light_dir = -1 * light_dir;
		double light_fac = std::max((double)0, normal * light_dir);
		fs << "\t" << "light fac: " << light_fac << std::endl;

		TGAColor albedo = TGAColor(albedo_r, albedo_g, albedo_a);
		draw_triangle(v3_pts, white * light_fac, zbuffer, framebuffer);

		fs << "________" << std::endl;
		++albedo_r;
	}

	// 写入
	TGAImage img(width, height, TGAImage::RGB);
	_write_img(framebuffer, &img);
	img.write_tga_file("../output/african_head.tga");

	TGAImage img_z(width, height, TGAImage::RGB);
	_write_z_img(zbuffer, &img_z);
	img_z.write_tga_file("../output/african_head_z.tga");


	fs.flush();
	fs.close();

	delete model;
}

void triangle()
{

	buffer<TGAColor>* framebuffer = _new_buffer<TGAColor>();
	buffer<float>* zbuffer = _new_buffer<float>();
	zbuffer->Fill(0);

	vec3 points[3];
	points[0] = vec3(-1, -1, 0);
	points[1] = vec3(0.3, 0.7, 0.2);
	points[2] = vec3(0.2, 0.9, 0.7);
	
	draw_triangle(points, red, zbuffer, framebuffer);

	// write
	/*TGAImage img(width, height, TGAImage::RGB);
	_write_img(framebuffer, &img);
	img.write_tga_file("../output/triangle.tga");*/

	TGAImage img_z(width, height, TGAImage::RGB);
	_write_z_img(zbuffer, &img_z);
	img_z.write_tga_file("../output/triangle_z.tga");
}


int main(int argc, char** argv)
{
	african_head();
	//triangle();

	return 0;
}
