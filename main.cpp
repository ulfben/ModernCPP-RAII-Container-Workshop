#include <algorithm>
#include <numeric>
#include <initializer_list>
#include <ranges>
#include <compare>
#include <concepts>
#include <cassert>
#include <stdexcept>
#include <memory>
template<typename T>
class Vec{
	static_assert(std::regular<T>, "Vec<T> requires T to be regular");
public:
	using value_type = T;
	using iterator = T*;
	using const_iterator = const T*;
	using size_type = std::size_t;
	using reference = T&;
	using const_reference = const T&;
	using pointer = T*;
	using const_pointer = const T*;

	Vec() noexcept = default; 
	Vec(size_type count){
		_data = std::make_unique<value_type[]>(count);
		_size = count;
	}
	Vec(size_type count, value_type val) : Vec(count){
		std::fill(begin(), end(), val); //if this throws, our vector is invalid (_size is wrong!)
	}
	Vec(std::initializer_list<value_type> l) : Vec(l.size()){
		std::copy(l.begin(), l.end(), begin());
	}
	Vec(const Vec& that) : Vec(that.size()){ //copy ctor		
		std::copy(that.begin(), that.end(), begin());
	}
	Vec(Vec&& that) noexcept :
		_data(std::exchange(that._data, nullptr)),
		_size(std::exchange(that._size, 0)){}

	//assignment operator, handling both rvalue and lvalue via copy-and-swap idiom
	// 'that' is passed by value, so it will be constructed either via copy-ctor (if called with an lvalue) 
	// or via move-ctor (if called with an lvalue)
	//either way we have an independent copy of the data to swap with 'this'
	Vec& operator=(Vec that) noexcept{
		swap(that);//copy-and-swap idiom. 	
		return *this;
	}


	~Vec() noexcept = default;

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

	auto begin() noexcept -> iterator{ return data(); };
	auto begin() const noexcept -> const_iterator{ return data(); };
	auto end() noexcept -> iterator{ return data() + size(); }
	auto end() const noexcept -> const_iterator{ return data() + size(); }
	auto size() const noexcept -> size_type{ return _size; }
	auto empty() const noexcept -> bool{ return size() == 0; }
	auto data() noexcept -> pointer{ return _data.get(); }
	auto data() const noexcept -> const_pointer{ return _data.get(); }

	auto front() noexcept -> reference{
		assert(!empty() && "Calling front() on an empty vec is undefined behavior!");
		return _data[0];
	}
	auto front() const noexcept -> const_reference{
		assert(!empty() && "Calling front() on an empty vec is undefined behavior!");
		return _data[0];
	}
	auto back() noexcept -> reference{
		assert(!empty() && "Calling back() on an empty vec is undefined behavior!");
		return _data[size() - 1];
	}
	auto back() const noexcept -> const_reference{
		assert(!empty() && "Calling back() on an empty vec is undefined behavior!");
		return _data[size() - 1];
	}

	auto clear() noexcept -> void{ 
		*this = {}; 
	} //use assignment operator to clean up and assign a default constructed Vec

	reference operator[](size_type index) noexcept{
		assert(index < size() && "Vec<T>: Index out of bounds in operator[]");
		return _data[index];
	}
	reference operator[](size_type index) const noexcept{
		assert(index < size() && "Vec<T>: Index out of bounds in operator[]");
		return _data[index];
	}

	reference at(size_type index){
		if(index < size()){
			return _data[index];
		}
		throw std::out_of_range("Vec<T>: Index out of bounds in at()");
	}
	reference at(size_type index) const{
		if(index < size()){
			return _data[index];
		}
		throw std::out_of_range("Vec<T>: Index out of bounds in at()");
	}

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
	std::unique_ptr<value_type[]> _data = nullptr;
};

int main(){
	//check that Vec<T> is a regular type, using the std::regular concept
	static_assert(std::regular<Vec<int>>, "Vec<T> should be regular");

  // 1) Default construction
	{
		Vec<int> v;
		assert(v.empty());
		assert(v.size() == 0);
		assert(v.data() == nullptr);

		// range-for over empty
		for(int x : v){
			(void) x;
			assert(false && "Range-for over empty Vec should not execute body");
		}
	}

	// 2) Initializer-list construction, range-for, front/back
	{
		Vec<int> v{1, 2, 3, 4};
		assert(!v.empty());
		assert(v.size() == 4);
		assert(v.front() == 1);
		assert(v.back() == 4);

		int sum = 0;
		for(int x : v){
			sum += x;
		}
		assert(sum == 1 + 2 + 3 + 4);
	}

	// 3) (count, value) constructor + std::all_of
	{
		Vec<int> v(5, 42);
		assert(v.size() == 5);
		bool all_42 = std::all_of(v.begin(), v.end(),
			[](int x){ return x == 42; });
		assert(all_42);
	}

	// 4) Copy constructor
	{
		Vec<int> a{1, 2, 3};
		Vec<int> b = a;  // copy-ctor
		assert(b == a);
		assert(b.size() == a.size());
		// make sure it's a deep copy (different buffers)
		assert(b.data() != a.data());
	}

	// 5) Copy assignment
	{
		Vec<int> a{5, 6, 7};
		Vec<int> b;
		b = a;  // copy-assign
		assert(b == a);
		assert(b.size() == a.size());
		assert(b.data() != a.data());
	}

	// 6) Move constructor
	{
		Vec<int> src{10, 11, 12};
		auto* old_data = src.data();
		Vec<int> dst = std::move(src);
		assert(dst.size() == 3);
		assert(dst.front() == 10);
		assert(dst.back() == 12);
		assert(dst.data() == old_data);
	}

	// 7) Move assignment
	{
		Vec<int> src{20, 21};
		Vec<int> dst{1, 2, 3, 4};
		auto* src_data = src.data();
		dst = std::move(src);
		assert(dst.size() == 2);
		assert(dst[0] == 20);
		assert(dst[1] == 21);
		assert(dst.data() == src_data);
		assert(src.size() == 0);
		assert(src.data() == nullptr);
	}

	// 8) Comparison operators (== and <)
	{
		Vec<int> a{1, 2, 3};
		Vec<int> b{1, 2, 3};
		Vec<int> c{1, 2, 4};
		Vec<int> d{1, 2};

		assert(a == b);
		assert(!(a == c));
		assert(a < c);   // 3 < 4
		assert(d < a);   // prefix is smaller (2 elements vs 3)
		assert(!(c < a));
	}

	// 9) at() with valid and invalid index
	{
		Vec<int> v{10, 20, 30};
		assert(v.at(0) == 10);
		assert(v.at(1) == 20);
		assert(v.at(2) == 30);

		bool threw = false;
		try{
			v.at(3); // out of range
		} catch(const std::out_of_range&){
			threw = true;
		}
		assert(threw && "v.at(3) should throw std::out_of_range");
	}

	// 10) clear(), std::sort, and swap()
	{
		Vec<int> v{3, 1, 4, 1, 5};
		std::sort(v.begin(), v.end());
		// sorted: 1 1 3 4 5
		assert(v.size() == 5);
		assert(v[0] == 1);
		assert(v[1] == 1);
		assert(v[2] == 3);
		assert(v[3] == 4);
		assert(v[4] == 5);

		// friend swap + member swap
		Vec<int> a{1, 2};
		Vec<int> b{9, 8, 7};
		swap(a, b); // ADL to friend

		assert(a.size() == 3);
		assert(b.size() == 2);
		assert(a[0] == 9 && a[1] == 8 && a[2] == 7);
		assert(b[0] == 1 && b[1] == 2);

		v.clear();
		assert(v.empty());
		assert(v.size() == 0);
		// with your current clear() implementation,
		// this will be nullptr again:
		assert(v.data() == nullptr);
	}

   //"All Vec<T> tests passed.\n";

	return 0;
}