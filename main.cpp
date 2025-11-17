#include <algorithm>
#include <numeric>
#include <initializer_list>
#include <ranges>
#include <compare>
#include <concepts>

template<typename T>
class Vec{
public:	
	using value_type = T;
	using iterator = T*;
	using const_iterator = const T*;
	using size_type = std::size_t;
	using reference = T&;
	using const_reference = const T&;
	using pointer = T*;
	using const_pointer = const T*;

	Vec() noexcept : _size(0), _data(nullptr){}
	
	Vec(std::initializer_list<value_type> l){
		_data = new value_type[l.size()];
		_size = l.size();
		std::copy(l.begin(), l.end(), begin());
	}
	
	Vec(size_type count, value_type val = {}){
		_data = new value_type[count];
		_size = count;				
		std::fill(begin(), end(), val);
		//std::iota(_data, _data+_size, 0);
	}	
	Vec(const Vec& that){ //copy ctor
		if(that.empty()){ return; }
		_data = new value_type[that._size];
		std::copy(that.begin(), that.end(), begin());
		_size = that._size;
	}
	Vec(Vec&& that) noexcept {		
		_data = that._data;		
		_size = that._size;
		that._data = nullptr;
		that._size = 0;		
	}
	Vec& operator=(Vec&& that) noexcept{
		std::swap(_data, that._data);
		std::swap(_size, that._size);
		return *this;
	}

	Vec& operator=(const Vec& that){ 
		auto temp(that); //copy ctor
		swap(temp);
		return *this;
	}
	
	~Vec() noexcept{ delete[] _data; }
	
	bool operator==(const Vec& that) const noexcept{
		if(size() != that.size()){ return false; }		
		return std::equal(begin(), end(), that.begin());		
	}

	bool operator<(const Vec& that) const noexcept{
		return std::lexicographical_compare(
			begin(), end(),
			that.begin(), that.end()
		);
	}

	iterator begin() noexcept { return _data; };
	iterator end() noexcept { return _data + _size; }
	
	const_iterator begin() const noexcept { return _data; };
	const_iterator end() const noexcept { return _data + _size; }
	
	size_type size() const noexcept { return _size; }
	bool empty() const noexcept { return size() == 0; }

	void swap(Vec& that) noexcept{
		using std::swap;
		swap(_data, that._data);
		swap(_size, that._size);		
	}
	friend void swap(Vec& a, Vec& b) noexcept{
		a.swap(b); //delegate to the member version
	}
private:
	size_t _size = 0;
	pointer _data = nullptr;
};

int main(){
	//check that Vec<T> is a regular type, using the std::regular concept
	static_assert(std::regular<Vec<int>>, "Vec<T> should be regular");
	
	auto v = Vec<int>(10, 5); //fill ctor
	auto v2(v); //copy ctor

	auto v3 = Vec<int>(3, 7);
	v2 = v3; //copy assignment

	auto v4 = Vec<int>{5,4,3,2,1}; //initializer list ctor
	v4 = Vec<int>(4, 9); //move assignment
	
	v.swap(v4); //member swap
	swap(v, v4); //non-member swap

	return 0;
}