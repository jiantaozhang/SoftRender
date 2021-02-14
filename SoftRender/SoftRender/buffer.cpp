#include "buffer.h"

template<typename T>
buffer<T>::buffer(int width, int height)
{
	this->_buffer = new T[width * height];
	this->_width = width;
	this->_height = height;
}

template<typename T>
buffer<T>::~buffer()
{
	delete[] this->_buffer;
	this->_width = 0;
	this->_height = 0;	
}

template<typename T>
void buffer<T>::Set(int x, int y, T data)
{
	this._buffer[y * this->_width+ x] = data;
}

template<typename T>
T buffer<T>::Get(int x, int y)
{
	return this->_buffer[y * this->_width+ x];
}

template<typename T>
void buffer<T>::Clear(T data)
{
	int cnt = this->_width* this->_height;
	for (int i = 0; i < cnt; i++)
	{
		this->_buffer[i] = data;
	}
}