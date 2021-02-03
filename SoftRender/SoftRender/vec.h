#ifndef __VEC_H__
#define __VEC_H__
#endif // !__VEC_H__

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


// partial specialization template
template<>
struct vec<2>
{
	double x{}, y{};	// {} means default-initialization

	vec() = default;
	vec(double x, double y) : x(x), y(y) {}

	// indexer
	double& operator[](const int i) { return 0 == i ? x : y; }
	double operator[] (const int i) const { return 0 == i ? x : y; }


	// norm
	double norm2() const { return (*this) * (*this); }
	double norm() const { return std::sqrt(norm2()); }
}


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
vec<n> operator*(const vec<n>& left, const vec<n>& right) {
	vec<n> v = left;
	for (int i = n; i--; v[i] *= right[i]);
	return v;
}


template<int n>
vec<n> operator*(double fac, const vec<n>& right) {
	vec<n> v = right;
	for (int i = n; i--; v[i] *= fac);
	return v;
}

template<int n>
vec<n> operator/(const vec<n>& left, double fac) {
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

