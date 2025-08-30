#ifndef MATRIX_H
#define MATRIX_H
#include <mdspan>
#include <iostream>
#include <iomanip>
#include <cstddef>
#include <stdexcept>
#include <vector>
#include <memory>
template<typename T>
class matrix 
{
	private:
		using Rep = std::shared_ptr<std::vector<T>>;
		Rep store;
		std::mdspan<T, std::dextents<size_t,2>> store_view;
	public:
		explicit matrix(const size_t row, const size_t column, const T& val = T()) noexcept
		{
			this->store = std::make_shared<std::vector<T>>(row * column, val);
			this->store_view = std::mdspan<T, std::dextents<size_t, 2>>(this->store->data(), row, column);
		}
		matrix(matrix&& other) noexcept : 
			store(std::move(other.store)), store_view(other.store_view)
		{
			if (store)
				store_view = std::mdspan<T, std::dextents<size_t,2>>(store->data(), store_view.extent(0), store_view.extent(1));
			other.store_view = std::mdspan<T, std::dextents<size_t,2>>();
		}
		matrix& operator=(matrix&& other) noexcept 
		{
			if (this != &other)
			{
				store = std::move(other.store);
				store_view = other.store_view;
				if (store)
					store_view = std::mdspan<T, std::dextents<size_t, 2>>(store->data(), store_view.extent(0), store_view.extent(1));
				other.store_view = std::mdspan<T, std::dextents<size_t,2>>();
			}
			return *this;
		}
		matrix(const matrix& other) noexcept : 
			store(std::make_shared<std::vector<T>>(*other.store)), store_view(store->data(), other.store_view.extent(0), other.store_view.extent(1)){}
		matrix& operator=(const matrix& other)
		{
			if (this != &other)
			{
				store = std::make_shared<std::vector<T>>(*other.store);
				store_view = std::mdspan<T, std::dextents<size_t, 2>>(store->data(), other.store_view.extent(0), other.store_view.extent(1));
			}
			return *this;
		}
		explicit matrix() = default;
		~matrix() = default;
		[[nodiscard]] auto make_row_acceptor(size_t row) const noexcept
		{
			return [this, row](size_t col)->decltype(auto)
			{
				return this->store_view[row, col];
			};
		}
		auto make_column_acceptor(size_t col) const noexcept
		{
			return [this, col](size_t row)->decltype(auto)
			{
				return this->store_view[row,col];
			};
		}
		friend std::ostream& operator << (std::ostream& os, const matrix& val)
		{
			for (size_t i = 0; i < val.store->size(); i++)
			{
				if (!(i % val.store_view.extent(1)) && i != 0) os << '\n';
				os << std::fixed << std::setprecision(9) << std::setw(2) << std::left << (*val.store)[i] << " ";
			}
			return os;
		}
		[[nodiscard]] size_t size_row() const noexcept { return this->store_view.extent(0); }
		[[nodiscard]] size_t size_col() const noexcept { return this->store_view.extent(1); }
		[[nodiscard]] matrix operator + (const matrix& m) 
		{
			if (this->store_view.extent(0) == m.store_view.extent(0) && this->store_view.extent(1) == m.store_view.extent(1))
			{
				matrix res(this->store_view.extent(0), this->store_view.extent(1));
				std::transform(this->store_view.data_handle(), this->store_view.data_handle() + this->store_view.size(), m.store_view.data_handle(),	res.store_view.data_handle(), [](T x, T y){return x + y;});
				return res;
			}
			else throw std::invalid_argument("Matrix dimensions don't match for addition!");
		}
		[[nodiscard]] matrix operator + (const T& val) noexcept
		{
			matrix res(this->store_view.extent(0), this->store_view.extent(1));;
			std::transform(res.store_view.data_handle(), res.store_view.data_handle() + res.store_view.size(), res.store_view.data_handle(), [val](T x) { return x + val; });
			return res;
		}
		void operator += (const matrix& m) 
		{
			if (this->store_view.extent(0) == m.store_view.extent(0) && m.store_view.extent(1) == this->store_view.extent(1))
				std::transform(this->store_view.data_handle(), this->store_view.data_handle() + this->store_view.size(), m.store_view.data_handle(), this->store_view.data_handle(), [](T x, T y){return x + y;});
			else throw std::invalid_argument("Matrix dimensions don't match for addition!");
		}
		void operator += (const T& val) noexcept
		{
			std::transform(this->store_view.data_handle(), this->store_view.data_handle() + this->store_view.size(), this->store_view.data_handle(), [val](T x){return x + val;});
		}
		[[nodiscard]] matrix operator * (const matrix& m)
		{
			matrix res(this->store_view.extent(0), m.store_view.extent(1));
			if (this->store_view.extent(1) == m.store_view.extent(0))
			{
				for (size_t i = 0; i < this->store_view.extent(0); i++)
				{
					for (size_t j = 0; j < m.store_view.extent(1); j++)
					{
						T sum = T();
						for (size_t k = 0; k < this->store_view.extent(1); k++)
							sum += this->store_view[i,k] * m.store_view[k,j];
						res.store_view[i,j] = sum;
					}
				}
			}
			else throw std::invalid_argument("Matrix dimension don't match for multiplication!");
			return res;
		}
		[[nodiscard]] matrix operator * (const T& val) noexcept
		{
			matrix res(this->store_view.extent(0), this->store_view.extent(1));
			std::transform(this->store_view.data_handle(), this->store_view.data_handle() + this->store_view.size(), res.store_view.data_handle(), [val](T x){return x *val;});
			return res;
		}
		void operator *= (const matrix& m) 
		{
			if (this->store_view.extent(1) == m.store_view.extent(0))
			{
				std::vector<T> buffer(this->store_view.extent(0) * m.store_view.extent(1));
				std::mdspan<T, std::dextents<size_t,2>>buffer_view(buffer.data(), this->store_view.extent(0), m.store_view.extent(1));
				matrix cur(this->store_view.extent(0), this->store_view.extent(1));
				*cur.store = *this->store;
				for (size_t i = 0; i < this->store_view.extent(0); i++)
				{
					for (size_t j = 0;j < m.store_view.extent(1); j++)
					{
						T sum = T();
						for (size_t k = 0; k < this->store_view.extent(1); k++)
							sum += cur.store_view[i,k] * m.store_view[k,j];
						buffer_view[i,j] = sum;
					}
				}
				std::copy(buffer.begin(), buffer.end(), this->store_view.data_handle());
			}
			else throw std::invalid_argument("Matrix dimension don't match for multiplication!");
		}
		void operator *= (const T& val) noexcept 
		{
			std::transform(this->store_view.data_handle(), this->store_view.data_handle() + this->store_view.size(), this->store_view.data_handle(), [val](T x){return x * val;});
		}
		[[nodiscard]] decltype(auto) operator ()(const size_t row, const size_t col) const noexcept
		{
			return this->store_view[row, col];
		}
		void set_column(size_t idx, std::vector<T> val) noexcept 
		{
			if (val.size() == this->store_view.extent(0))
			{
				for (size_t i = 0; i < this->store_view.extent(0);i++)
					this->store_view[i, idx] = val[i];
			}
		}
		void set_row(size_t idx, std::vector<T> val) noexcept 
		{
			if (val.size() == this->store_view.extent(1))
			{
				for (size_t i = 0; i < this->store_view.extent(1); i++)
					this->store_view[idx, i] = val[i];
			}
		}
		[[nodiscard]] matrix operator - (const matrix& m) 
		{
			if (this->store_view.extent(0) == m.store_view.extent(0) && this->store_view.extent(1) == m.store_view.extent(1))
			{
				matrix res(this->store_view.extent(0), this->store_view.extent(1));
				std::transform(this->store_view.data_handle(), this->store_view.data_handle() + this->store_view.size(), m.store_view.data_handle(), res.store_view.data_handle(), [](T x, T y){return x - y;});
				return res;
			}
			else throw std::invalid_argument("Matrix dimension don't match for subtraction!");
		}
		[[nodiscard]] matrix operator - (const T& val) noexcept 
		{
			matrix res(this->store_view.extent(0), this->store_view.extent(1));
			std::transform(this->store_view.data_handle(), this->store_view.data_handle() + this->store_view.size(),res.store_view.data_handle(), [val](T x){return x - val;});
			return res;
		}
		void operator -= (const matrix& m)
		{
			if (this->store_view.extent(0) == m.store_view.extent(0) && this->store_view.extent(1) == m.store_view.extent(1))
				std::transform(this->store_view.data_handle(), this->store_view.data_handle() + this->store_view.size(), m.store_view.data_handle(), this->store_view.data_handle(), [](T x, T y){return x - y;});
			else throw std::invalid_argument("Matrix dimension don't match for subtraction!");
		}
		void operator -= (const T& val) noexcept
		{
			std::transform(this->store_view.data_handle(), this->store_view.data_handle() + this->store_view.size(), this->store_view.data_handle(), [val](T x){return x - val;});
		}
		void set_row(size_t idx, auto acceptor)
		{
			for (size_t i = 0; i < this->store_view.extent(1); i++)
				this->store_view[idx,i] = acceptor(i);
		}
		void set_column(size_t idx, auto acceptor)
		{
			for (size_t i = 0; i < this->store_view.extent(0);i++)
				this->store_view[i,idx] = acceptor(i);
		}
		T sum_row(size_t idx) const noexcept
		{
			T res = T();
			for (size_t i = 0;i < this->store_view.extent(1);i++)
				res += this->store_view[idx,i];
			return res;
		}
		T sum_column(size_t idx) const noexcept 
		{
			T res = T();
			for (size_t i = 0; i < this->store_view.extent(0);i++)
				res += this->store_view[i, idx];
			return res;
		}
		T max_row(size_t idx) const noexcept 
		{
			T res = this->store_view[idx,0];
			for (size_t i = 1; i < this->store_view.extent(1);i++)
			{
				if (res < this->store_view[idx, i])
					res = this->store_view[idx,i];
			}
			return res;
		}
		T max_column(size_t idx) const noexcept 
		{
			T res = this->store_view[0, idx];
			for (size_t i = 1; i < this->store_view.extent(0); i++)
			{
				if (res < this->store_view[i,idx])
					res = this->store_view[i, idx];
			}
			return res;
		}
		T min_row(size_t idx) const noexcept 
		{
			T res = this->store_view[idx, 0];
			for (size_t i = 1; i < this->store_view.extent(1); i++)
			{
				if (res > this->store_view[idx,i])
					res = this->store_view[idx,i];
			}
			return res;
		}
		T min_column(size_t idx) const noexcept 
		{
			T res = this->store_view[0,idx];
			for (size_t i = 0; i < this->store_view.extent(0); i++)
			{
				if (res > this->store_view[i,idx])
					res = this->store_view[i,idx];
			}
			return res;
		}
		std::shared_ptr<std::vector<T>> store_handle() { return this->store; }
};
#endif
