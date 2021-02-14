#pragma once

template<typename T>
class buffer
{
private:
	T* _buffer;
	int _width, _height;

public:
	buffer(int width, int height)
	{
		_buffer = new T[width * height];
		_width = width;
		_height = height;
	}

	~buffer()
	{
		delete[]  _buffer;
		_width = 0;
		_height = 0;
	}


	void Set(int x, int y, T data)
	{
		_buffer[y * _width + x] = data;
	}

	// 这里就不返回指针的   因为主要用途是封装如 depth buffer\ frame buffer 这种逐像素数据   通常不会很大
	T Get(int x, int y) const
	{
		return  _buffer[y * _width + x];
	}

	// 使用data 将所有像素数据覆盖填充
	void Fill(T data)
	{
		int cnt = _width * _height;
		for (int i = 0; i < cnt; i++)
		{
			_buffer[i] = data;
		}
	}
};

