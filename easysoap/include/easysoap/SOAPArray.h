/* 
 * EasySoap++ - A C++ library for SOAP (Simple Object Access Protocol)
 * Copyright (C) 2001 David Crowley; SciTegic, Inc.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 * $Id$
 */


#ifndef __SOAPARRAY_H__
#define __SOAPARRAY_H__

#include <new>

#include <easysoap/SOAPUtil.h>
#include <easysoap/SOAPException.h>

BEGIN_EASYSOAP_NAMESPACE

template <typename T>
class SOAPArray
{
private:
	T*		m_array;
	size_t	m_allocated;
	size_t	m_size;

	void _realloc(size_t s)
	{
		if (s > m_allocated)
		{
			const size_t minalloc = sp_maximum<size_t>(8, 128/sizeof(T));
			size_t toalloc = m_allocated;
			if (toalloc < minalloc)
				toalloc = minalloc;

			while (toalloc < s)
				toalloc *= 2;

			T* newarray = sp_alloc<T>(toalloc);
			if (!newarray)
				throw SOAPMemoryException(); 

			// Copy/construct new array from old array
			size_t i;
			for (i = 0; i < m_size; ++i)
				new (newarray + i) T(m_array[i]);

			// Initialize rest of new array
			for (i = m_size; i < toalloc; ++i)
				new (newarray + i) T();

			s = m_size;
			Empty();
			m_size = s;
			m_array = newarray;
			m_allocated = toalloc;
		}
	}

public:
	typedef T* Iterator;
	typedef const T* ConstIterator;

	SOAPArray(size_t s = 0)
		: m_array(0)
		, m_allocated(0)
		, m_size(0)
	{
		Resize(s);
	}

	SOAPArray(const SOAPArray& x)
		: m_array(0)
		, m_allocated(0)
		, m_size(0)
	{
		Add(x.m_array, x.Size());
	}

	SOAPArray<T>& operator=(const SOAPArray<T>& x)
	{
		Resize(x.Size());
		for (size_t i = 0; i < x.Size(); ++i)
			m_array[i] = x[i];

		return *this;
	}

	~SOAPArray()
	{
		Empty();
	}

	void AttachTo(SOAPArray<T>& other)
	{
		Empty();

		m_allocated = other.m_allocated;
		m_array = other.m_array;
		m_size = other.m_size;

		other.m_allocated = 0;
		other.m_array = 0;
		other.m_size = 0;
	}

	T& Add()
	{
		size_t len = Size();
		Resize(len + 1);
		return m_array[len];
	}

	template <typename X>
	T& Add(const X& val)
	{
		size_t len = Size();
		Resize(len + 1);
		return m_array[len] = val;
	}

	template <typename X>
	void AddArray(const SOAPArray<X>& a)
	{
		Add((const X*)a, a.Size());
	}

	template <typename X>
	void Add(const X* vals, size_t numVals)
	{

		size_t len = Size();
		size_t newlen = Size() + numVals;
		Resize(newlen);
		T *work = m_array + len;
		for (size_t i = 0; i < numVals; ++i)
			*work++ = vals[i];
	}

	template <typename X>
	T& AddAt(size_t index, const X &val)
	{

		Resize(Size() + 1);
		for (size_t i = Size() - 1; i > index; i--)
			m_array[i] = m_array[i - 1];

		return m_array[index] = val;
	}

	ConstIterator Begin() const
	{
		return m_array;
	}

	ConstIterator End() const
	{
		return m_array + Size();
	}

	Iterator Begin()
	{
		return m_array;
	}

	Iterator End()
	{
		return m_array + Size();
	}

	void Empty()
	{
		for (size_t i = 0; i < m_allocated; ++i)
			m_array[i].~T();

		sp_free(m_array);
		m_allocated = 0;
		m_size = 0;
	}
	
	bool IsEmpty() const
	{
		return (Size() == 0);
	}

	T* Ptr()
	{
		return m_array;
	}

	const T* Ptr() const
	{
		return m_array;
	}

	operator T* ()
	{
		return Ptr();
	}

	operator const T* () const
	{
		return Ptr();
	}

	template <typename X>
	void Assign(size_t numItems, const X &value)
	{
		Resize(numItems); 
		for(size_t i = 0; i < Size(); i++)
			m_array[i] = value;
	}

	void RemoveAt(size_t index)
	{
		for(size_t i = index; i < Size() - 1; i++)
			m_array[i] = m_array[i + 1]; 

		Resize(Size() - 1); 
	}

	size_t Size() const
	{
		return m_size;
	}

	void Resize(size_t s)
	{

		if (s > Size())
			_realloc(s);
		m_size = s;
	}

	T& GetAt(size_t index)
	{
		return m_array[index];
	}

	const T& GetAt(size_t index) const
	{
		return m_array[index];
	}

	T& operator[](int index)
	{
		return m_array[index];
	}

	const T& operator[](int index) const
	{
		return m_array[index];
	}

	T& operator[](size_t index)
	{
		return m_array[index];
	}

	const T& operator[](size_t index) const
	{
		return m_array[index];
	}

	template<typename X>
	bool operator==(const SOAPArray<X>& x) const
	{
		if (m_size != x.m_size)
			return false;
		for (size_t i = 0; i < m_size; ++i)
			if (m_array[i] != x[i])
				return false;
		return true;
	}

	template<typename X>
	bool operator!=(const SOAPArray<X>& x) const
	{
		return !(*this == x);
	}

	//
	// For some STL compatibility
	typedef T* iterator;
	typedef const T* const_iterator;
	typedef T value_type;

	const_iterator begin() const	{return Begin();}
	const_iterator end() const		{return End();}
	iterator begin()				{return Begin();}
	iterator end()					{return End();}
	size_t size() const				{return Size();}
	void resize(size_t s)			{Resize(s);}
};

template <typename T>
class SOAP2DArray
{
private:
	SOAPArray<T>	m_array;
	size_t			m_rows;
	size_t			m_cols;

	//
	// It might be nice to
	// implement these...
	SOAP2DArray(const SOAP2DArray&);
	SOAP2DArray& operator=(const SOAP2DArray&);

public:

	SOAP2DArray()
	{
	}

	SOAP2DArray(size_t rows, size_t cols)
	{
		Resize(rows, cols);
	}

	//
	// This does a resize, but contents
	// will get all scrambled around.
	// use at your own peril...
	void Resize(size_t rows, size_t cols)
	{
		m_rows = rows;
		m_cols = cols;
		m_array.Resize(m_rows * m_cols);
	}

	size_t GetNumRows() const {return m_rows;}
	size_t GetNumCols() const {return m_cols;}

	T* operator[](size_t row)
	{
		return m_array.Ptr() + (row * m_cols);
	}

	const T* operator[](size_t row) const
	{
		return m_array.Ptr() + (row * m_cols);
	}

	bool operator==(const SOAP2DArray& other)
	{
		return GetNumRows() == other.GetNumRows() &&
			GetNumCols() == other.GetNumCols() &&
			m_array == other.m_array;
	}

	bool operator!=(const SOAP2DArray& other)
	{
		return !(*this == other);
	}


	//
	// For some STL compatibilty
	typedef T value_type;
	void resize(size_t rows, size_t cols)	{Resize(rows, cols);}
};

END_EASYSOAP_NAMESPACE

#endif // __SOAPARRAY_H__
