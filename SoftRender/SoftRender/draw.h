#pragma once

#include "vec.h"
#include "tgaimage.h"
#include <float.h>

#define double_max DBL_MAX
#define double_min DBL_MIN

void line(int x1, int y1, int x2, int y2, TGAImage& image, TGAColor color) {
	// 处理斜率的绝对值大于 1 的直线
	bool steep = false;
	if (std::abs(x1 - x2) < std::abs(y1 - y2)) {
		std::swap(x1, y1);
		std::swap(x2, y2);
		steep = true;
	}
	// 交换起点终点的坐标
	if (x1 > x2) {
		std::swap(x1, x2);
		std::swap(y1, y2);
	}
	int y = y1;
	int eps = 0;
	int dx = x2 - x1;
	int dy = y2 - y1;
	int yi = 1;

	// 处理 [-1, 0] 范围内的斜率
	if (dy < 0) {
		yi = -1;
		dy = -dy;
	}

	for (int x = x1; x <= x2; x++) {
		if (steep) {
			image.set(y, x, color);
		}
		else {
			image.set(x, y, color);
		}

		eps += dy;
		// 这里用位运算 <<1 代替 *2
		if ((eps << 1) >= dx) {
			y = y + yi;
			eps -= dx;
		}
	}
}


void triangle(const vec2* points, TGAImage& img, const TGAColor& color)
{
	double w = img.get_width();
	double h = img.get_height();

	// calc bounding
	vec2 min(double_max, double_max);
	vec2 max(0, 0);
	for (int i = 0; i < 3; i++)
	{
		vec2 p = points[i];
		min.x = std::min(p.x, min.x);
		min.y = std::min(p.y, min.y);

		max.x = std::max(p.x, max.x);
		max.y = std::max(p.y, max.y);
	}


	vec2 v_ab = points[1] - points[0];
	vec2 v_bc = points[2] - points[1];
	vec2 v_ca = points[0] - points[2];

	// iterate and fill
	for (int y = min.y; y <= max.y; y++)
	{
		for (int x = min.x; x <= max.x; x++)
		{
			vec2 p = vec2(x, y);
			// determine if be within triangle
			vec2 v_ap = p - points[0];
			if (cross(v_ab, v_ap) < 0)
				continue;

			vec2 v_bp = p - points[1];
			if (cross(v_bc, v_bp) < 0)
				continue;

			vec2 v_cp = p - points[2];
			if (cross(v_ca, v_cp) < 0)
				continue;

			img.set(x, y, color);
		}
	}
}


// 计算有三个points 组成p时，对应的三个分量
vec3 barycenter(const vec2* points, const vec2 p)
{
	vec3 vecx(points[1].x - points[0].x, points[2].x - points[0].x, points[0].x - p.x);
	vec3 vecy(points[1].y - points[0].y, points[2].y - points[0].y, points[0].y - p.y);

	vec3 uvw = cross(vecx, vecy);
	if (abs(uvw.w) < 0.001f)
	{
		return vec3(1, 0, 0);	// 这个值 不知道怎么设置好
	}

	return uvw / uvw.w;
}
