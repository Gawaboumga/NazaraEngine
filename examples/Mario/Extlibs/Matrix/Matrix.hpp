#ifndef __MATRIX_HPP__
#define __MATRIX_HPP__

#include <array>
#include <memory>

template <class T, std::size_t N>
class Matrix
{
	template <class U>
	using ptr = std::unique_ptr<U>;
	using dimensions = std::array<std::ptrdiff_t, N>;

	public:

		using value_type = typename ptr<T[]>::element_type;
		using size_type = std::size_t;
		using difference_type = std::ptrdiff_t;
		using reference = value_type&;
		using const_reference = const typename ptr<T[]>::element_type&;
		using pointer = typename ptr<T[]>::pointer;
		using const_pointer = const typename ptr<T[]>::pointer;
		using iterator = pointer;
		using const_iterator = const_pointer;

	public:

		iterator begin();
		const_iterator begin() const;

		const_iterator cbegin() const;
		const_iterator cend() const;

		iterator end();
		const_iterator end() const;

	public:

		Matrix() = default;
		Matrix(Matrix&& other) = default;
		Matrix(std::array<difference_type, N> dimensions);

		template <class InputIt>
		void assign(InputIt first, InputIt last);
		void assign(const std::initializer_list<T>& datas);

		void fill(value_type value);

		difference_type ith_dimension(size_type i) const;

		reference operator()(std::array<difference_type, N> indices);
		const_reference operator()(std::array<difference_type, N> indices) const;
		Matrix& operator=(Matrix&& other) = default;

		void swap(Matrix& other);

	private:

		size_type compute_index(std::array<difference_type, N> indices) const;

		size_type multiply_dimensions() const;

		ptr<T[]> m_datas;
		dimensions m_dimensions;
};

namespace std
{
	template <class T, std::size_t N>
	void swap(Matrix<T, N>& lhs, Matrix<T, N>& rhs);
}

#include "Matrix.inl"

#endif // __MATRIX_HPP__
