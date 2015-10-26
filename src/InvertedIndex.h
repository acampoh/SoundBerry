/*
 * InvertedIndex.h
 *
 *  Created on: 23/10/2015
 *      Author: acampoh
 */

#ifndef INVERTEDINDEX_H_
#define INVERTEDINDEX_H_

#include<string>

template<class T>
static std::map<std::string, T> CreateInvertedIndex(const std::string* index, int count)
{
	std::map<std::string, T> m;
	for (int i = 0; i < count; ++i)
	{
		m[index[i]] = (T) i;
	}
	return m;
}

template<class T>
static std::map<int, T> CreateInvertedIndex(const int* index, int count)
{
	std::map<int, T> m;
	for (int i = 0; i < count; ++i)
	{
		m[index[i]] = (T) i;
	}
	return m;
}

#endif /* INVERTEDINDEX_H_ */
