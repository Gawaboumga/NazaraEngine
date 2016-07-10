#include <algorithm>
#include <cassert>

template <class T, std::size_t N>
typename Matrix<T, N>::iterator Matrix<T, N>::begin()
{
	return m_datas.get();
}

template <class T, std::size_t N>
typename Matrix<T, N>::const_iterator Matrix<T, N>::begin() const
{
	return m_datas.get();
}

template <class T, std::size_t N>
typename Matrix<T, N>::const_iterator Matrix<T, N>::cbegin() const
{
	return begin();
}

template <class T, std::size_t N>
typename Matrix<T, N>::const_iterator Matrix<T, N>::cend() const
{
	return end();
}

template <class T, std::size_t N>
typename Matrix<T, N>::iterator Matrix<T, N>::end()
{
	return &m_datas[multiply_dimensions()];
}

template <class T, std::size_t N>
typename Matrix<T, N>::const_iterator Matrix<T, N>::end() const
{
	return &m_datas[multiply_dimensions()];
}

template <class T, std::size_t N>
Matrix<T, N>::Matrix(std::array<difference_type, N> dimensions) :
	m_datas{},
	m_dimensions{}
{
	m_dimensions = std::move(dimensions);

	auto numberToAllocate = multiply_dimensions();
	if (numberToAllocate == 0)
		throw std::logic_error("One dimension at least equals zero");

	m_datas = std::move(std::unique_ptr<T[]>(new T[numberToAllocate]));
}

template <class T, std::size_t N>
template <class InputIt>
void Matrix<T, N>::assign(InputIt first, InputIt last)
{
	assert(std::distance(first, last) == multiply_dimensions());

	std::uninitialized_copy(first, last, m_datas.get());
}

template <class T, std::size_t N>
void Matrix<T, N>::assign(const std::initializer_list<T>& datas)
{
	assert(std::distance(datas.begin(), datas.end()) == static_cast<difference_type>(multiply_dimensions()));

	std::uninitialized_copy(datas.begin(), datas.end(), m_datas.get());
}

template <class T, std::size_t N>
void Matrix<T, N>::fill(value_type value)
{
	std::uninitialized_fill(begin(), end(), value);
}

template <class T, std::size_t N>
typename Matrix<T, N>::difference_type Matrix<T, N>::ith_dimension(size_type i) const
{
	assert(i < m_dimensions.size());
	return m_dimensions[i];
}

template <class T, std::size_t N>
typename Matrix<T, N>::reference Matrix<T, N>::operator()(std::array<difference_type, N> indices)
{
	for (auto i = 0u; i < indices.size(); ++i)
		assert(indices[i] < m_dimensions[i]);

	return m_datas[compute_index(indices)];
}

template <class T, std::size_t N>
typename Matrix<T, N>::const_reference Matrix<T, N>::operator()(std::array<difference_type, N> indices) const
{
	for (auto i = 0u; i < indices.size(); ++i)
		assert(indices[i] < m_dimensions[i]);

	return m_datas[compute_index(indices)];
}

template <class T, std::size_t N>
void Matrix<T, N>::swap(Matrix& other)
{
	for (auto i = 0u; i < m_dimensions.size(); ++i)
		assert(m_dimensions[i] == other.m_dimensions[i]);

	std::swap_ranges(begin(), end(), other.begin());
}

template <class T, std::size_t N>
typename Matrix<T, N>::size_type Matrix<T, N>::compute_index(std::array<difference_type, N> indices) const
{
	size_type index = 0;
	size_type total = multiply_dimensions();
	for (auto i = 0u; i < indices.size(); ++i)
	{
		total /= m_dimensions[i];
		index += total * indices[i];
	}

	return index;
}

template <class T, std::size_t N>
typename Matrix<T, N>::size_type Matrix<T, N>::multiply_dimensions() const
{
	return std::accumulate(m_dimensions.begin(), m_dimensions.end(), 1, std::multiplies<size_type>());
}

namespace std
{
	template <class T, std::size_t N>
	void swap(Matrix<T, N>& lhs, Matrix<T, N>& rhs)
	{
		lhs.swap(rhs);
	}
}
