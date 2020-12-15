#ifndef ARRAY
#define ARRAY

#include <cstdlib>
#include <algorithm>



//integer array
class ArrayInt {
	ArrayInt() :ArrayInt(0) {};
	ArrayInt(int size) {
		capacity = size;
		this->size_ = size;
		data = allocatorInt.allocate(size);
	}

	ArrayInt(const ArrayInt &other) {
		this->capacity = other.size_;
		this->size_ = other.size_;
		data = allocatorInt.allocate(size_);
		for (size_t i = 0; i < size_; i++) {
			allocatorInt.construct(data + i, other.data[i]);
		}
	}

	ArrayInt(ArrayInt &&other) {
		this->capacity = other.size_;
		this->size_ = other.size_;
		data = other.data;
		other.data = nullptr;
	}
	~ArrayInt() {
		if (data) {
			allocatorInt.deallocate(data, size_);
		}
	}

	ArrayInt &operator = (const ArrayInt &other){
		this->capacity = other.capacity;
		this->size_ = other.size_;
		data = allocatorInt.allocate(size_);
		for (size_t i = 0; i < size_; i++) {
			allocatorInt.construct(data + i, other.data[i]);
		}
		return *this;
	}

	ArrayInt &operator = (ArrayInt &&other) {
		this->capacity = other.capacity;
		this->size_ = other.size_;
		data = other.data;
		other.data = nullptr;
		return *this;
	}


	void pushBack(const int &x) {
		if (size_ < capacity) {
			allocatorInt.construct(data + size_++, x);//мжно и глубже allocator_traits
			return;
		}
		auto temp = allocatorInt.allocate(capacity * 2);
		for (int i = 0; i < size_;i++) {
			allocatorInt.construct(temp + i, data[i]);
			allocatorInt.destroy(data + i);
		}
		allocatorInt.construct(temp + size_++, x);
		std::swap(temp, data);
		allocatorInt.deallocate(temp,capacity);
		capacity <<= 1;
	}
	void popBack() {
		if (size_) {
			allocatorInt.destroy(data + (--size_));
		}
	}

	int &operator[](int n) {
		return data[n];
	}

	int &at(int n) {
		if (n>=0 && n<size_) {
			return data[n];
		}
		else {
			throw std::exception("out of range");
		}
	}
	int size() {
		return size_;
	}
private:
	struct AllocatorInt {
		int * allocate(size_t n) const {
			return static_cast<int *>(malloc(sizeof(int) * n));
		}

		void deallocate(int *p,size_t n)const noexcept {
			if (p) free(static_cast<void *>(p));
		}
		
		template<class Type=int, typename... Args>
		void * construct(Type *p, const Args&... args)const noexcept {
			return new((void*)p) Type(args...);
		}
		template<typename Type=int>
		void destroy(Type *p)const noexcept {
			p->~Type();
		}
	}allocatorInt;
	int *data;
	size_t capacity;
	size_t size_;
};

#endif
