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

	// ����Ͳ�����ָ���   ��Ϊ��Ҫ��;�Ƿ�װ�� depth buffer\ frame buffer ��������������   ͨ������ܴ�
	T Get(int x, int y) const
	{
		return  _buffer[y * _width + x];
	}

	// ʹ��data �������������ݸ������
	void Fill(T data)
	{
		int cnt = _width * _height;
		for (int i = 0; i < cnt; i++)
		{
			_buffer[i] = data;
		}
	}
};

