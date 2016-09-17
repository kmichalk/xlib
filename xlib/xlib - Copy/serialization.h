#ifndef SERIALIZATION_H
#define SERIALIZATION_H
#include <fstream>
#include <iostream>
#include <string>

#define byte char

//x::vec
//x::arr
//x::lst
//x::gph
//x::bnt
//x::bst
//x::rnd
//x::tim
//x::str
//x::srl
//x::clk
//x::fun
//x::sig
//x::map
//x::stk
//x::que
//x::slt
//x::exf
//x::dec
//x::obs

class xsrl
{
public:
	std::fstream& file;
	size_t size;

	xsrl(std::fstream& file):file(file){}
	//xsrl():size(0){}
	/*xsrl(const char* filename, bool clear=false) :
		file(filename, 	
			std::fstream::in | 
			std::fstream::out | 
			clear*std::fstream::trunc)
	{
	}*/

	template<typename T>
	typename std::enable_if_t<!std::is_class<T>::value, xsrl&>
		write(const T& obj)
	{
		file.write(static_cast<const byte*>(static_cast<const void*>(&obj)),
			sizeof(T));
		return *this;
	}

	template<typename T>
	xsrl& write(const T* ptr, size_t size)
	{
		file.write(static_cast<const byte*>(static_cast<const void*>(ptr)),
			sizeof(T) * size);
		return *this;
	}

	__forceinline void to_begin()
	{
		file.seekg(0);
	}

	template<typename T>
	typename std::enable_if_t<!std::is_class<T>::value, xsrl&>
		operator<<(const T& obj)
	{
		//file.seekg(0);
		file.write(static_cast<const byte*>(static_cast<const void*>(&obj)),
			sizeof(T));
		return *this;
	}

	template<typename T>
	T& operator>>(T& obj)
	{
		file.seekg(0);
		file.read(static_cast<byte*>(static_cast<void*>(&obj)), 
			sizeof(T));
		return obj;
	}

	friend std::ostream& operator<<(std::ostream& os, xsrl& ofs)
	{
		size_t size = ofs.file.seekg(0, std::fstream::end).tellg();
		ofs.to_begin();
		char* data = new char[size];
		ofs.file.read(data, size);
		os.write(data, size);
		delete[] data;
		return os;
	}

	~xsrl()
	{
		if(file.is_open()) file.close();
	}
};



template<typename T>
void save_object(const T& obj)
{
	xsrl objfs(typeid(T).name());
	objfs << obj;
}

#undef byte



#endif