#pragma once
#include "vec.h"

template<int d>
struct determinant;

template<int row, int col>
struct matrix {
	vec<col> rows[row] = { {} };	// default ctor
	matrix() = default;

	// indexer
	vec<col>& operator[] (const int row) { return rows[row]; }
	const vec<col>& operator[] (const int row) const { return rows[row]; }

	//
	vec<row> col(const int idx) const {
		vec<row> ret;
		for (int i = 0; i < row; i++)
			ret[i] = rows[i][idx];
		return ret;
	}

	void set_col(const int idx, const vec<row>& v) {
		for (int i = 0; i < row; i++)
			rows[i][idx] = v[i];
	}

	static matrix identity() {
		matrix ret;
		for (int col_idx = 0; col_idx < length; col_idx++)
			for (int row_idx = 0; row_idx < length; row_idx++)
				rows[row_idx][col_idx] = row_idx == col_idx ? 1 : 0;

		return ret;
	}

	// 行列式
	// 返回当前矩阵的行列式值
	double det() const {
		return determinant<row>::calc(*this);
	}

	// 指定行列删除后，剩下的无符号矩阵
	matrix<row - 1, col - 1> minor(const int row_idx, const int col_idx) const {
		matrix<row - 1, col - 1> ret;
		for (int r = 0; r < row; r++)
			for (int c = 0; c < col; c++)
			{
				int ri = r < row_idx ? r : r + 1;
				int ci = c < col_idx ? c : c + 1;
				ret[r][c] = rows[ri][ci];
			}
		return ret;
	}


	// 共轭因数
	double cofactor(const int row, const int col) const {
		return minor(row, col).det() * ((row + col) % 2 == 0 ? 1 : -1);
	}

	matrix<row, col> adjugate() const {
		matrix<row, col> ret;
		for (int ri = 0; ri < row; ri++)
			for (int ci = 0; ci < col; ci++)
			{
				ret[ri][ci] = cofactor(ri, ci);
			}
		return ret;
	}

};
