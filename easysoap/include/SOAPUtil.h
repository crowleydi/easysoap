

#ifndef __SOAPUTIL_H__
#define __SOAPUTIL_H__

#include <stdlib.h>

template<typename T>
inline T*
sp_alloc(size_t n)
{
	return (T*)malloc(n * sizeof(T));
}

template<typename T>
inline void
sp_free(T*& ptr)
{
	if (ptr)
		free((void *)ptr);
	ptr = 0;
}

inline char *
sp_strcpy(char *dest, const char *src)
{
	char *work = dest;
	if (src && dest)
	{
		while ((*work++ = *src++))
			;
	}
	return dest;
}

inline char *
sp_strncpy(char *dest, const char *src, size_t n)
{
	char *work = dest;
	if (src && dest)
	{
		while (n-- && (*work++ = *src++))
			;
	}
	return dest;
}

inline size_t
sp_strlen(const char *str)
{
	size_t ret = 0;
	if (str)
	{
		while (*str++)
			++ret;
	}
	return ret;
}

inline int
sp_toupper(int c)
{
	if (c >= 'a' && c <= 'z')
		c += 'A' - 'a';
	return c;
}

inline int
sp_strcmp(const char *a, const char *b)
{
	int ret = 0;
	if (a && b)
	{
		while (*a && *b && *a == *b)
			++a, ++b;
		ret = *a - *b;
	}
	else if (a)
		ret = 1;
	else if (b)
		ret = -1;
	return ret;
}

inline int
sp_strncmp(const char *a, const char *b, unsigned int n)
{
	int ret = 0;
	if (a && b)
	{
		while (n > 0 && *a && *b && *a == *b)
			++a, ++b, --n;
		if (n == 0)
			ret = 0;
		else
			ret = *a - *b;
	}
	else if (a)
		ret = 1;
	else if (b)
		ret = -1;
	return ret;
}

inline int
sp_strcasecmp(const char *a, const char *b)
{
	int ret = 0;
	if (a && b)
	{
		int ua = sp_toupper(*a++);
		int ub = sp_toupper(*b++);
		while (ua && ub && ua == ub)
		{
			ua = sp_toupper(*a++);
			ub = sp_toupper(*b++);
		}
		ret = ua - ub;
	}
	else if (a)
		ret = 1;
	else if (b)
		ret = -1;
	return ret;
}

inline char*
sp_strstr(const char *haystack, const char *needle)
{
	if (haystack && needle)
	{
		if (*needle == 0)
			return (char *)haystack;

		// step along the main string
		while (*haystack)
		{
			const char *nn = needle;
			const char *hh = haystack;
			// see if substring characters match
			while (*nn++ == *hh++)
			{
				if (*nn == 0)
					// we got all the way to the end of the substring
					// so we must've won
					return (char *)haystack;
			}
			++haystack;
		}
	}
	return 0;
}

inline bool
sp_isspace(char c)
{
	switch (c)
	{
	case '\r':
	case '\n':
	case '\t':
	case ' ':
		return true;
	}
	return false;
}

inline char*
sp_strchr(const char *s, char c)
{
	if (s)
	{
		do
		{
			if (*s == c)
				return (char*)s;
		}
		while (*s++);
	}
	return 0;
}

inline void *
sp_memset(void *s, int c, size_t n)
{
	unsigned char *ptr = (unsigned char *)s;
	if (ptr)
	{
		while (n--)
			*ptr++ = c;
	}
	return s;
}

inline unsigned int
sp_hashcode(const char *key)
{
	unsigned char* ptr = (unsigned char*)key;
	unsigned int h = 0;
	if (ptr)
	{
		unsigned int c;
		while ((c = *ptr++))
			h = 31 * h + c;
	}
	return h;
}

inline unsigned int
sp_hashcodecase(const char *key)
{
	unsigned char* ptr = (unsigned char*)key;
	unsigned int h = 0;
	if (ptr)
	{
		unsigned int c;
		while ((c = sp_toupper(*ptr++)))
			h = 31 * h + c;
	}
	return h;
}

template <class T>
inline T
sp_minimum(const T& a, const T& b)
{
	return (a < b) ? a : b;
}

template <class T>
inline T
sp_maximum(const T& a, const T& b)
{
	return (a < b) ? b : a;
}

#endif // __SOAPUTIL_H__

