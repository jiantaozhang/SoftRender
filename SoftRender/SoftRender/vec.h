﻿#pragma once

#include <cmath>
#include <string>
#include <iostream>

template<int n>
struct vec
{
public:
	vec() = default;

	// sample: vec[1] = 100
	double& operator[](const int i) { return data[i]; }

	// sample: double x = vec[0]
	double operator[] (const int i) const { return data[i]; }

	// vec norm
	double norm2() const { return (*this) * (*this); }
	double norm() const { return std::sqrt(norm2()); }

	double data[n] = { 0 };

	std::string str() {

		std::string ret = "(";
		for (int i = 0;; i++) {
			ret += data[i];
			if (i == n - 1)
			{
				ret += ")";
				break;
			}
		}
		return ret;
	}
};



// override operator
template<int n>
vec<n> operator+(const vec<n>& left, const vec<n>& right) {
	vec<n> v = left;
	for (int i = n; i--; v[i] += right[i]);
	return v;
}

template<int n>
vec<n> operator-(const vec<n>& left, const vec<n>& right) {
	vec<n> v = left;
	for (int i = n; i--; v[i] -= right[i]);
	return v;
}

template<int n>
double operator*(const vec<n>& left, const vec<n>& right) {
	double ret = 0;
	for (int i = 0; i < n; i++)
		ret += left[i] * right[i];
	return ret;
}

template<int n>
vec<n> operator*(const double& fac, const vec<n>& src) {
	vec<n> v = src;
	for (int i = n; i--; v[i] *= fac);
	return v;
}

template<int n>
vec<n> operator*(const vec<n>& src, const double& fac) {
	vec<n> v = src;
	for (int i = n; i--; v[i] *= fac);
	return v;
}

template<int n>
vec<n> operator/(const vec<n>& left, const double& fac) {
	vec<n> v = left;
	for (int i = n; i--; v[i] /= fac);
	return v;
}

template<int dst, int src>
vec<dst> proj(const vec<src>& src_vec, double fill = 0) {
	vec<dst> v;

	for (int i = dst; i--; v[i] = i < src ? src_vec[i] : fill);
	return v;
}

template<int n>
std::ostream& operator<<(std::ostream& s, const vec<n>& v)
{
	s << "(";
	for (int i = 0; i < n - 1; i++)
	{
		s << v[i] << ",";
	}

	s << v[n - 1] << ")";
	return s;
}



// partial specialization template
template<>
struct vec<2>
{
	union {
		struct { double x, y; };
		double data[2];
	};

	vec() = default;
	vec(double x, double y) : x(x), y(y) {}

	//operator vec<3>() const { return vec<3>(x, y, 0); }

	// indexer
	double& operator[](const int i) { return data[i]; }
	double operator[] (const int i) const { return data[i]; }
	// norm
	double norm2() const { return (*this) * (*this); }
	double norm() const { return std::sqrt(norm2()); }

	// str

};
typedef vec<2> vec2;


template<>
struct vec<3>
{
	union {
		struct { double x, y, z; };
		struct { double u, v, w; };
		double data[3];
	};

	vec() = default;
	vec(double x, double y, double z) : x(x), y(y), z(z) {}
	//vec(vec2 v2) : x(v2.x), y(v2.y), z(0) {}

	// 
	operator vec<2>() const { return vec<2>(x, y); }

	// indexer
	double& operator[](const int i) { return data[i]; }
	double operator[] (const int i) const { return data[i]; }

	// norm 
	double norm2() const { return (*this) * (*this); }
	double norm() { return std::sqrt(norm2()); }

	// str
	operator std::string() {
		std::string s;
		s += "(";
		s += x;
		s += ",";
		s += y;
		s += ",";
		s += z;
		s += ")";
		return s;
	}
};
typedef vec<3> vec3;

vec3 cross(const vec3& v1, const vec3& v2);

double cross(const vec2& v1, const vec2& v2);
