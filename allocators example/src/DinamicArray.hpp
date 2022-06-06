#ifndef DINAMIC_ARRAY
#define DINAMIC_ARRAY

#include <cstdlib>
#include <algorithm>
#include <new>
#include <initializer_list>



//abstract dinamic array
template<class Type>
class DinamicArray {
public:
	DinamicArray() :DinamicArray(0) {};
	DinamicArray(int size) {
		capacity_ = size;
		size_ = size;
		data_ = allocatorTraits_.allocate(size);
	}

	DinamicArray(const DinamicArray &other) {
		capacity_ = other.size_;
		size_ = other.size_;
		data_ = allocatorTraits_.allocate(size_);
		for (size_t i = 0; i < size_; i++) {
			allocatorTraits_.construct(data_ + i, other.data_[i]);
		}
	}

	DinamicArray(DinamicArray &&other)noexcept {
		capacity_ = other.size_;
		size_ = other.size_;
		data_ = other.data_;
		other.data_ = nullptr;
	}
	~DinamicArray() {
		if (data_) {
			allocatorTraits_.deallocate(data_, size_);
		}
	}

	DinamicArray &operator = (const DinamicArray &other){
		capacity_ = other.capacity_;
		size_ = other.size_;
		data_ = allocatorTraits_.allocate(size_);
		for (size_t i = 0; i < size_; i++) {
			allocatorTraits_.construct(data_ + i, other.data_[i]);
		}
		return *this;
	}

	DinamicArray &operator = (DinamicArray &&other)noexcept {
		capacity_ = other.capacity_;
		size_ = other.size_;
		data_ = other.data_;
		other.data_ = nullptr;
		return *this;
	}


	void push_back(const Type &&x) {
		if (size_ < capacity_) {
			allocatorTraits_.construct(data_ + size_++, x);//мжно и глубже allocator_traits
			return;
		}
		auto temp = allocatorTraits_.allocate(capacity_ * 2);
		for (int i = 0; i < size_;i++) {
			allocatorTraits_.construct(temp + i, data_[i]);
			allocatorTraits_.destroy(data_ + i);
		}
		allocatorTraits_.construct(temp + size_++, x);
		std::swap(temp, data_);
		allocatorTraits_.deallocate(temp,capacity_);
		capacity_ <<= 1;
	}

	void pop_back() {
		if (size_) {
			allocatorTraits_.destroy(data_ + (--size_));
		}
	}

	Type &operator[](int n) noexcept {
		return data_[n];
	}

	const Type &operator[](int n) const noexcept {
		return data_[n];
	}

	Type &at(int n) {
		if (n>=0 && n<size_) {
			return data_[n];
		}
		else {
			throw std::exception("out of range");
		}
	}

	const Type &at(int n) const {
		if (n >= 0 && n < size_) {
			return data_[n];
		}
		else {
			throw std::exception("out of range");
		}
	}

	Type &back() {
		return data_[n - 1];
	}

	const Type &back()const {
		return data_[n - 1];
	}

	Type &front() {
		return data_[0];
	}

	const Type &front()const {
		return data_[0];
	}

	class iterator {

		Type &operator->()const {
			return *place_;
		}

		Type &operator*()const {
			return *place_;
		};

		operator Type *()const {
			return place_;
		};

		
		iterator &operator+(int i) {
			place_ += i;
			position_ += i;
			return *this;
		}

		iterator &operator-(int i) {
			place_ -= i;
			position_ -= i;
			return *this;
		}

		iterator &operator--() {
			place_ -= 1;
			position_ -= 1;
			return *this;
		}

		iterator &operator--(int) {
			iterator tmp{*this};
			place_ -= 1;
			position_ -= 1;
			return tmp;
		}

		iterator &operator++() {
			place_ += 1;
			position_ += 1;
			return *this;
		}

		iterator &operator++(int) {
			iterator tmp{*this};
			place_ += 1;
			position_ += 1;
			return tmp;
		}

		operator bool()const {

		}

	private:
		iterator() = delete;
		iterator(int position,Type place):position_(position),place_(place){}

		int position_;
		Type *place_;
	};


	int size() const {
		return size_;
	}

	int capacity() const {
		return capacity_;
	}

private:
	static struct {
		Type * allocate(size_t n) const {
			return static_cast<Type *>(::malloc(sizeof(Type) * n));
		}

		void deallocate(Type *p,size_t n)const noexcept {
			if (p) ::free(static_cast<void *>(p));
		}
		
		template<typename... Args>
		void * construct(Type *p, const Args&... args)const noexcept {
			return new((void*)p) Type(args...);
		}
		
		void destroy(Type *p)const noexcept {
			p->~Type();
		}
	}allocatorTraits_;
	Type *data_;
	int capacity_;
	int size_;
};

#endif // DINAMIC_ARRAY
