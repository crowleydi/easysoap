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


#ifndef __SOAPHASHMAP_H__
#define __SOAPHASHMAP_H__

#ifdef _MSC_VER
#pragma warning(disable: 4284)
#endif // _MSC_VER

// pre-decalure some functors
template<typename T>
struct SOAPHashCodeFunctor;
template<typename T>
struct SOAPHashCodeFunctorNoCase;
template<typename T>
struct SOAPEqualsFunctorNoCase;

template<typename T>
struct SOAPEqualsFunctor
{
	bool operator()(const T& a, const T& b) const
	{
		return a == b;
	}
};


#include <SOAPUtil.h>
#include <SOAPArray.h>


template <typename K, typename I,
	typename H = SOAPHashCodeFunctor<K>,
	typename E = SOAPEqualsFunctor<K> >
class EASYSOAP_EXPORT SOAPHashMap
{
private:
	// structure for keeping a linked-list of elements
	struct HashElement {
		HashElement(size_t hash, const K& key, const I& item)
			: m_hash(hash), m_key(key), m_item(item), m_next(0)
		{}
		HashElement(size_t hash, const K& key)
			: m_hash(hash), m_key(key), m_next(0)
		{}

		HashElement	*m_next;
		size_t		m_hash;
		K			m_key;
		I			m_item;
	};

	H hashcode;
	E equals;

	typedef SOAPArray<HashElement*>	Elements;

	Elements	m_elements;
	Elements	m_pool;
	size_t		m_numElements;
	float		m_fillfactor;
	size_t		m_resizeThreshold;

	// our Iterator class
	class ForwardHashMapIterator
	{
	private:
		const SOAPHashMap		*m_map;
		Elements::Iterator		m_index;
		HashElement				*m_he;
		
		friend class SOAPHashMap<K,I,H,E>;

		// private constuctor that can only be called by SOAPHashMap
		ForwardHashMapIterator(const SOAPHashMap *map, Elements::Iterator index)
			: m_map(map), m_index(index), m_he(0)
		{
			if (m_map)
			{
				// Find first bucket with an element
				while (m_index != m_map->m_elements.End() && !(m_he = *m_index))
						++m_index;
			}
		}

		ForwardHashMapIterator(const SOAPHashMap *map, Elements::Iterator index, HashElement *he)
			: m_map(map), m_index(index), m_he(he)
		{
		}

	public:
		// default constructor
		ForwardHashMapIterator()
			: m_map(0), m_index(0)
		{
		}

		//copy constructor
		ForwardHashMapIterator(const ForwardHashMapIterator& r)
			: m_map(r.m_map), m_index(r.m_index), m_he(r.m_he)
		{
		}

		// assignment operator
		ForwardHashMapIterator& operator=(const ForwardHashMapIterator& r)
		{
			m_map = r.m_map;
			m_index = r.m_index;
			m_he = r.m_he;
			return *this;
		}

		// equals operator
		bool operator==(const ForwardHashMapIterator& r) const
		{
			// make sure we're pointing to the exact same element
			return m_he == r.m_he;
		}

		// not equals operator
		bool operator!=(const ForwardHashMapIterator& r) const
		{
			// can't be pointing to the same element...
			return m_he != r.m_he;
		}

		// Move to next element
		ForwardHashMapIterator& Next()
		{
			if (m_index != m_map->m_elements.End() && !(m_he = m_he->m_next))
				while (++m_index != m_map->m_elements.End() && !(m_he = *m_index))
					;
			return *this;
		}

		// prefix-increment
		ForwardHashMapIterator& operator++()
		{
			return Next();
		}

		// postfix-increment
		ForwardHashMapIterator operator++(int)
		{
			// copy our current position
			ForwardHashMapIterator ret(*this);
			// move to the next item
			Next();
			// return the old position
			return ret;
		}

		// some boolean operators
		operator bool()
		{
			return m_index != m_map->m_elements.End();
		}

		bool operator!()
		{
			return m_index == m_map->m_elements.End();
		}

		// access the hash key.  can't modify it!
		const K& Key() const
		{
			return m_he->m_key;
		}

		// access the data item.
		I& operator*()
		{
			return m_he->m_item;
		}

		const I& operator*() const
		{
			return m_he->m_item;
		}

		I* operator->()
		{
			return &m_he->m_item;
		}

		const I* operator->() const
		{
			return &m_he->m_item;
		}

		// maybe get rid of these since we can use *
		const I& Item() const
		{
			return m_he->m_item;
		}

		I& Item()
		{
			return m_he->m_item;
		}
	};

public:
	typedef ForwardHashMapIterator Iterator;

	~SOAPHashMap()
	{
		Empty();
	}
	
	SOAPHashMap(size_t size = 31, float fillfactor = 0.75) :
		m_numElements(0), m_fillfactor(fillfactor), m_resizeThreshold(0)
	{
		Resize(size); // this sets m_resizeThreshold
	}

	template<typename A, typename B, typename C, typename D>
	SOAPHashMap(const SOAPHashMap<A,B,C,D>& r) :
		m_numElements(0), m_fillfactor(r.GetFillFactor()), m_resizeThreshold(0)
	{
		Resize(r.GetNumBuckets()); // this sets m_resizeThreshold
		*this = r;
	}

	SOAPHashMap(const SOAPHashMap& r) :
		m_numElements(0), m_fillfactor(r.GetFillFactor()), m_resizeThreshold(0)
	{
		Resize(r.GetNumBuckets()); // this sets m_resizeThreshold
		*this = r;
	}

	template<typename A, typename B, typename C, typename D>
	SOAPHashMap& operator=(const SOAPHashMap<A,B,C,D>& r)
	{
		if ((void *)this != (void *)&r)
		{
			Clear();
			Resize(r.GetNumBuckets());
			SOAPHashMap<A,B,C,D>::Iterator end = r.End();
			for (SOAPHashMap<A,B,C,D>::Iterator it = r.Begin(); it != end; ++it)
				Add(it.Key(), it.Item());
		}
		return *this;
	}

	SOAPHashMap& operator=(const SOAPHashMap& r)
	{
		if (this != &r)
		{
			Clear();
			Resize(r.GetNumBuckets());
			Iterator end = r.End();
			for (Iterator it = r.Begin(); it != end; ++it)
				Add(it.Key(), it.Item());
		}
		return *this;
	}

	Iterator Begin() const
	{
		return Iterator(this, (Elements::Iterator)m_elements.Begin());
	}

	Iterator End() const
	{
		return Iterator(this, (Elements::Iterator)m_elements.End());
	}


	// find & set or add
	I& Add(const K& key, const I& item)
	{
		// see if we can find it
		size_t hash = hashcode(key);
		Iterator found = Find(key, hash);
		if (found)
			return *found = item;
		return Put(hash, key, item);
	}

	// find or add
	I& operator[](const K& key)
	{
		// see if we can find it
		size_t hash = hashcode(key);
		Iterator found = Find(key, hash);
		if (found)
			return *found;
		return Put(hash, key);
	}


	// returns true if we found the key and removed it.
	bool Remove(const K& key)
	{
		if (m_elements.Size() > 0)
		{
			size_t hash = hashcode(key);
			// we use ** here so we can treat the first element the
			// same was as the other elements in the linked list
			HashElement **he = &m_elements[hash % m_elements.Size()];
			while (*he)
			{
				if ((*he)->m_hash == hash && equals((*he)->m_key, key))
				{
					HashElement *temp = (*he)->m_next;
					PutBackHashElement(*he);
					*he = temp;
					return true;
				}
				he = &((*he)->m_next);
			}
		}
		return false;
	}

	// clear all elements.
	void Clear()
	{
		for (Elements::Iterator i = m_elements.Begin(); i != m_elements.End(); ++i)
		{
			HashElement *he = *i;
			while (he)
			{
				HashElement *next = he->m_next;
				PutBackHashElement(he);
				he = next;
			}
			*i = 0;
		}
	}

	void Empty()
	{
		Elements::Iterator i;
		for (i = m_elements.Begin(); i != m_elements.End(); ++i)
		{
			HashElement *he = *i;
			while (he)
			{
				HashElement *next = he->m_next;
				delete he;
				he = next;
			}
		}
		for (i = m_pool.Begin(); i != m_pool.End(); ++i)
			delete (*i);
	}

	size_t Size() const
	{
		return m_numElements;
	}

	// find the item associated with the given key.
	// returns null if we can't find the key.
	Iterator Find(const K& key) const
	{
		size_t hash = hashcode(key);
		return Find(key, hash);
	}

	size_t GetNumBuckets() const {return m_elements.Size();}
	float GetFillFactor() const {return m_fillfactor;}


	//
	// For STL compatibility
	typedef K key_type;
	typedef Iterator iterator;
	typedef Iterator const_iterator;

	iterator begin() const {return Begin();}
	iterator end() const {return End();}

private:
	// Does the actual find.  We pass in the hashcode so we don't have
	// to recompute it since it is used in other places.
	Iterator Find(const K& key, size_t hash) const
	{
		if (m_elements.Size() > 0)
		{
			size_t index = hash % m_elements.Size();
			HashElement *he = m_elements[index];
			while (he)
			{
				if (he->m_hash == hash && equals(he->m_key, key))
					return Iterator(this, (Elements::Iterator)m_elements.Begin() + index, he);
				he = he->m_next;
			}
		}
		return End();
	}

	// resize only if it makes the table bigger
	void Resize(size_t newsize)
	{
		if (newsize <= m_elements.Size())
			return;

		Elements newelements;
		newelements.Resize(newsize);
		Elements::Iterator i;

		for (i = newelements.Begin(); i != newelements.End(); ++i)
			*i = 0;

		for (i = m_elements.Begin(); i != m_elements.End(); ++i)
		{
			HashElement *he = *i;
			while (he)
			{
				HashElement *next = he->m_next;
				size_t index = he->m_hash % newsize;
				he->m_next = newelements[index];
				newelements[index] = he;
				he = next;
			}
		}

		m_resizeThreshold = (size_t)(m_fillfactor * newsize);
		m_elements.AttachTo(newelements);
	}

	void PutBackHashElement(HashElement *he)
	{
		if (he)
		{
			he->m_next = 0;
			m_pool.Add(he);
			--m_numElements;
		}
	}

	HashElement *GetNextHashElement(
			size_t hash,
			const K& key,
			const I& item)
	{
		++m_numElements;
		if (m_pool.Size() > 0)
		{
			HashElement *he = m_pool[m_pool.Size() - 1];
			m_pool.Resize(m_pool.Size() - 1);

			he->m_hash = hash;
			he->m_key = key;
			he->m_item = item;
			return he;
		}
		return new HashElement(hash, key, item);
	}

	HashElement *GetNextHashElement(
			size_t hash,
			const K& key)
	{
		++m_numElements;
		if (m_pool.Size() > 0)
		{
			HashElement *he = m_pool[m_pool.Size() - 1];
			m_pool.Resize(m_pool.Size() - 1);

			he->m_hash = hash;
			he->m_key = key;
			return he;
		}
		return new HashElement(hash, key);
	}


	// This method actually puts an object into the hashtable.
	I& Put(size_t hash, const K& key, const I& item)
	{
		// check for resize
		if (m_numElements >= m_resizeThreshold)
			Resize(m_elements.Size() * 2 + 1);

		size_t index = hash % m_elements.Size();
		HashElement *he = GetNextHashElement(hash, key, item);
		he->m_next = m_elements[index];
		m_elements[index] = he;
		return he->m_item;
	}

	// This method actually puts an object into the hashtable.
	I& Put(size_t hash, const K& key)
	{
		// check for resize
		if (m_numElements >= m_resizeThreshold)
			Resize(m_elements.Size() * 2 + 1);

		size_t index = hash % m_elements.Size();
		HashElement *he = GetNextHashElement(hash, key);
		he->m_next = m_elements[index];
		m_elements[index] = he;
		return he->m_item;
	}

	friend class ForwardHashMapIterator;
};


////////////////////////////////
///  Make it easier to use no case
////////////////////////////////
template<typename K, typename I>
class SOAPHashMapNoCase : public SOAPHashMap<K, I,
				SOAPHashCodeFunctorNoCase<K>,
				SOAPEqualsFunctorNoCase<K> >
{
private:
	typedef SOAPHashMap<K, I,
				SOAPHashCodeFunctorNoCase<K>,
				SOAPEqualsFunctorNoCase<K> > super;

public:
	SOAPHashMapNoCase(size_t size = 31, float fillfactor = 0.75)
		: super(size, fillfactor)
	{
	}
};


#endif // __SOAPHASHMAP_H__

