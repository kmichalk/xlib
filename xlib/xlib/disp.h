#ifndef DISP_H
#define DISP_H

#include <iostream>

#define disp std::cout<<
#define endl '\n'

template<typename T>
inline std::ostream& operator , (std::ostream& os, T const& var)
{
	return os<<var;
}

//template<char delim = ' ', typename T, size_t size>
//inline std::ostream& operator<<(std::ostream& os, T(&arr)[size])
//{
//	for (int i=0; i<size; ++i) os<<arr[i]<<delim;
//	return os;
//}

#endif
