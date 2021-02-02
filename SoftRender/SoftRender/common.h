
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
