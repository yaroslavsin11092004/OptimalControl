#ifndef MATRIX_H
#define MATRIX_H
#include "iterator.h"
#include <iostream>
#include <iomanip>
#include <cstddef>
#include <utility>
#include <valarray>
#pragma warning(disable : 4996)
using namespace std;
template<typename T>
class matrix 
{
	private:
		using PTR = typename stdext::checked_array_iterator<T*>;
	protected:
		using Rep = valarray<T>;
		size_t x,y;
		Rep v;
	public:
		using value_type = T;
		explicit matrix(const size_t row, const size_t column, const T& val = T())noexcept : 
			x(row == 0 ? 1 : row), y(column == 0 ? 1 : column), v(Rep(val, x * y)){}
		[[nodiscard]] PTR operator[](const size_t index)
		{
			return PTR{ &v[index * y], y};
		}
		gslice_array<value_type> operator[](const gslice& g){ return v[g]; }
		slice_array<value_type> operator[](const slice& s){ return v[s]; }
		slice_array<value_type> row(size_t index) noexcept
		{
			if (index >= x)
				index = x-1;
			return operator[](slice(index *y, y,1));
		}
		slice_array<value_type>column(size_t index) noexcept
		{
			if (index >= y)
				index = y - 1;
			return operator[](slice(index, x,y));
		}
		[[nodiscard]] matrix transpose() noexcept
		{
			matrix tm(y,x);
			tm.v[gslice(0, {x,y}, {1,x})] = v;
			return tm;
		}
		[[nodiscard]] Rep get_row(const size_t index) const noexcept
		{
			return v[slice((index % x)*y, y, 1)];
		}
		[[nodiscard]] Rep get_column(const size_t index) const noexcept
		{
			return v[slice(index % y,x,y)];
		}
		[[nodiscard]] pair<size_t, size_t> size() const noexcept
		{
			return make_pair(x,y);
		}
		[[nodiscard]] matrix operator* (const matrix& m) const
		{
			matrix tm(x,m.y);
			if (y == m.x)
			{
				for (size_t i = 0;i < x; i++)
				{
					for (size_t j = 0;j < m.y;j++)
					{
						Rep t = get_row(i) * m.get_column(j);
						tm.v[i * m.y + j] = t.sum();
					}
				}
			}
			return tm;
		}
		void operator += (const matrix& m)
		{
			if (m.x == x && m.y == y)
				v += m.v;
		}
		operator Rep() { return v; }
		friend ostream& operator << (ostream& os, const matrix& m)
		{
			for (size_t i = 0;i < m.v.size();i++)
			{
				if (!(i % m.y)) os << '\n';
				os << fixed << setprecision(9) << setw(2) << left << m.v[i] << " ";
			}
			return os;
		}
		[[nodiscard]]matrix operator+(const matrix& m) const 
		{
			matrix tm = *this;
			if (x == m.x && y == m.y)
				tm.v += m.v;
			return tm;
		}
};
#endif
