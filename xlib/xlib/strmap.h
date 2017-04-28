#ifndef _STRING_MAP_H_
#define _STRING_MAP_H_

#include "xvector.h"
#include "range.h"
#include "result.h"
#include "loops.h"

namespace x
{
	/// <summary>
	/// Kind of map class holding values os specified type assigned to strings.
	/// Optimizes search by key string by mapping first character and reduces
	/// the amount of string comparisons distincly.
	/// </summary>
	/// <typeparam name="_Type"> - value type of stored elements.</typeparam>
	template<class _Type>
	class strmap
	{
		x::vector<x::two<char const*, _Type>> data_[256];
		size_t size_;
		size_t capacity_;

	public:
		/// <summary>
		/// Default constructor.
		/// It does not allocate memory for stored elements.
		/// </summary>
		strmap()
			:
			data_{(x::vector<_Type>::set_default_alloc(0),
				std::move(x::vector<_Type>{}))}
		{
			x::vector<_Type>::set_default_alloc();
		}


		/// <summary>
		/// Initializes map with list of elements, every of which is inserted into the map.
		/// </summary>
		/// <param name="pairs"> - elements to be inserted</param>
		/// <example><code>x::strmap<int> intMap{{"one",1}, {"two",2}, {"three",3}};</code></example>
		strmap(std::initializer_list<x::two<char const*, _Type>> pairs)
			:
			data_{(x::vector<_Type>::set_default_alloc(0),
				std::move(x::vector<_Type>{}))}
		{
			x::vector<_Type>::set_default_alloc();
			for (auto pair : pairs)
				insert(pair.a, pair.b);
		}


		/// <summary>
		/// Gives actual amount of stored pairs.
		/// </summary>
		/// <returns>number of pairs</returns>
		inline size_t size() const
		{
			return size_;
		}

		/*size_t memory() const
		{

		}

		size_t capacity() const
		{

		}*/


		/// <summary>
		/// Assignes element of given value to specified key string.
		/// Overrides data existing under that key.
		/// </summary>
		/// <param name="str"> - key to be assigned</param>
		/// <param name="elem"> - value to assign</param>
		/// <returns>Reference to assigned or newly inserted element.</returns>
		strmap<_Type>& insert(char const* str, _Type const& elem) 
		{
			if (str) {
				data_[str[0]].push_back({str,elem});
				++size_;
			}
			return *this;
		}


		/// <summary>
		/// Removes pair with specified key string from the map.
		/// </summary>
		/// <param name="key"> - key to look for</param>
		/// <returns>reference to the caller</returns>
		strmap<_Type>& remove(char const* key)
		{
			if (key) {
				data_[key[0]].erase(data_[key[0]].find_by(&x::two<char const*, _Type>::a, key));
				--size_;
			}
			return *this;
		}


		/// <summary>
		/// Accesses element of specified key string.
		/// </summary>
		/// <returns>Reference to value if key was found wrapped into x::result class, 
		/// otherwise invalid result value not containing desired information.</returns>
		/// <example><code>x::strmap<int> intMap{{"one",1}};
		/// if (auto val = intMap["one"]){
		///		++val;
		/// }</code></example>
		x::result<_Type&> operator[](char const* str)
		{
			if (str) {
				foreach (elem, data_[str[0]]) {
					if ( ! std::strcmp(elem->a, str))
						return x::result<_Type&>{elem->b};
				}
			}
			return x::result<_Type&>::INVALID;
		}
	};

}

#endif //_STRING_MAP_H_
