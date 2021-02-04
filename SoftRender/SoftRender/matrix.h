#pragma once

#include <iostream>
#include "vec.h"

template <int d>
struct determinant;

template <int nrow, int ncol>
struct matrix
{
	vec<ncol> rows[nrow] = { {} }; // default ctor
	matrix() = default;

	// indexer
	vec<ncol>& operator[](const int row) { return rows[row]; }
	const vec<ncol>& operator[](const int row) const { return rows[row]; }

	// 获得指定列
	vec<nrow> col(const int idx) const
	{
		vec<nrow> ret;
		for (int i = 0; i < nrow; i++)
			ret[i] = rows[i][idx];
		return ret;
	}

	// 设置指定列
	void set_col(const int idx, const vec<nrow>& v)
	{
		for (int i = 0; i < nrow; i++)
			rows[i][idx] = v[i];
	}

	// 单位矩阵
	static matrix identity()
	{
		matrix ret;
		// 把 row == col 的地方全部填充1，剩下的全部都是0
		for (int r = 0; r < nrow; r++)
			for (int c = 0; c < ncol; c++)
				rows[r][c] = r == c ? 1 : 0;

		return ret;
	}

	// 行列式
	// 返回当前矩阵的行列式值
	double det() const
	{
		return determinant<nrow>::calc(*this);
	}

	// 余子式
	matrix<nrow - 1, ncol - 1> cominor(const int row_idx, const int col_idx) const
	{
		matrix<nrow - 1, ncol - 1> ret;
		for (int r = 0; r < nrow; r++)
			for (int c = 0; c < ncol; c++)
			{
				int ri = r < row_idx ? r : r + 1;
				int ci = c < col_idx ? c : c + 1;
				ret[r][c] = rows[ri][ci];
			}
		return ret;
	}

	// 代数余子式
	double cofactor(const int nrow, const int ncol) const
	{
		return cominor(nrow, ncol).det() * ((nrow + ncol) % 2 == 0 ? 1 : -1);
	}

	// 伴随矩阵 （方阵A中各元素的代数余子式构成的矩阵，通常记作 A*）
	matrix<nrow, ncol> adjugate() const
	{
		matrix<nrow, ncol> ret;
		for (int ri = 0; ri < nrow; ri++)
			for (int ci = 0; ci < col; ci++)
			{
				ret[ri][ci] = cofactor(ri, ci);
			}
		return ret;
	}

	// 逆转矩阵
	matrix<nrow, ncol> invert_transpose() const
	{
		matrix<nrow, ncol> ret = adjugate();
		double det = ret[0] * rows[0]; // 这个式子相当于 用第一行的代数余子式来计算 this行列式的值，  理解起来比较隐晦
		return ret / det;
	}

	// 逆矩阵
	matrix<nrow, ncol> invert() const
	{
		// 把 逆转矩阵 转置一下即可
		matrix<nrow, ncol> ret = invert_transpose();
		return ret.transpose();
	}

	// 转置矩阵
	matrix<ncol, nrow> transpose() const
	{
		// 讲道理 通常来说 ncol == nrow， 如果不是的话 其实很多使用处也会有一些隐患
		matrix<ncol, nrow> ret;
		for (int c = 0; c < ncol; c++)
			ret[c] = this->col(c);
		return ret;
	}
};


// operator
// matrix x vec
template<int nrow, int ncol>
vec<nrow> operator*(const matrix<nrow, ncol>& m, const vec<nrow>& v)
{
	vec<nrow> ret;
	for (int r = 0; r < nrow; r++)
		for (int c = 0; c < ncol; c++)
			ret[r] = m[r] * v;

	return ret;
}
// matrix x matrix
template<int nrowl, int ncoll, int ncolr>
matrix<nrowl, ncolr> operator*(const matrix<nrowl, ncoll>& left, const matrix<nrowl, ncolr>& right)
{
	matrix<nrowl, ncolr> ret;
	for (int r = 0; r < nrowl; r++)
	{
		for (int c = 0; c < ncolr; c++)
		{
			ret[r][c] = left[r] * right.col(c);
		}
	}
	return ret;
}

// matrix x double
template<int nrows, int ncols>matrix<nrows, ncols> operator*(const matrix<nrows, ncols>& lhs, const double& val) {
	matrix<nrows, ncols> result;
	for (int i = nrows; i--; result[i] = lhs[i] * val);
	return result;
}

// matrix / double
template<int nrows, int ncols>matrix<nrows, ncols> operator/(const matrix<nrows, ncols>& lhs, const double& val) {
	matrix<nrows, ncols> result;
	for (int i = nrows; i--; result[i] = lhs[i] / val);
	return result;
}

// matrix + matrix
template<int nrows, int ncols>matrix<nrows, ncols> operator+(const matrix<nrows, ncols>& lhs, const matrix<nrows, ncols>& rhs) {
	matrix<nrows, ncols> result;
	for (int i = nrows; i--; )
		for (int j = ncols; j--; result[i][j] = lhs[i][j] + rhs[i][j]);
	return result;
}

// matrix - matrix
template<int nrows, int ncols>matrix<nrows, ncols> operator-(const matrix<nrows, ncols>& lhs, const matrix<nrows, ncols>& rhs) {
	matrix<nrows, ncols> result;
	for (int i = nrows; i--; )
		for (int j = ncols; j--; result[i][j] = lhs[i][j] - rhs[i][j]);
	return result;
}

// ostream << matrix
template<int nrows, int ncols> std::ostream& operator<<(std::ostream& out, const matrix<nrows, ncols>& m) {
	for (int i = 0; i < nrows; i++) out << m[i] << std::endl;
	return out;
}


// determinant
// 这里的写法其实就是 泛型编程的递归写法
template <int d>
struct determinant {
	static double calc(const matrix<d, d>& m)
	{
		double ret = 0;
		for (int c = 0; c < d; c++)
			ret += m[0][c] * m.cofactor(0, c);
		return ret;
	}
};

// 递归终止条件 
template <>
struct determinant<1> {
	static double calc(const matrix<1, 1>& m)
	{
		return m[0][0];
	}
};
