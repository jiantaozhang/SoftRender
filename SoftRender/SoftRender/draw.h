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

// 计算有三个points 组成p时，对应的三个分量
vec3 barycenter(const vec2* pixels, const vec2 p)
{
	vec3 vecx(pixels[1].x - pixels[0].x, pixels[2].x - pixels[0].x, pixels[0].x - p.x);
	vec3 vecy(pixels[1].y - pixels[0].y, pixels[2].y - pixels[0].y, pixels[0].y - p.y);

	vec3 bc = cross(vecx, vecy);

	// 退化成一条线，重心目前来说没有那么重要
	if (abs(bc.z) < 1)
	{
		// 姑且认定 u=1;v=1;
		return vec3(-1, 1, 1);	// 这个值 不知道怎么设置好
	}

	// (1-u-v, u, v)
	return vec3(1 - bc.x - bc.y, bc.x, bc.y) / bc.w;
}





