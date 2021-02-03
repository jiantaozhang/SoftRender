#ifndef __VEC_H__
#define __VEC_H__


#include <cmath>
#include <string>

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
	for (int i = n; i--; src[i] *= fac);
	return src;
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

	// indexer
	double& operator[](const int i) { return data[i]; }
	double operator[] (const int i) const { return data[i]; }
	// norm
	double norm2() const { return (*this) * (*this); }
	double norm() const { return std::sqrt(norm2()); }

	// str
	operator std::string() {
		std::string s;
		s += "(";
		s += x;
		s += ",";
		s += y;
		s += ")";
		return s;
	}
};
typedef vec<2> vec2;

template<>
struct vec<3>
{
	union {
		struct { double x, y, z; };
		double data[3];
	};

	vec() = default;
	vec(double x, double y, double z) : x(x), y(y), z(z) {}

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


vec3 cross(const vec3& v1, const vec3& v2) {
	return vec<3>{v1.y* v2.z - v1.z * v2.y, v1.z* v2.x - v1.x * v2.z, v1.x* v2.y - v1.y * v2.x};
}

#endif // !__VEC_H__
