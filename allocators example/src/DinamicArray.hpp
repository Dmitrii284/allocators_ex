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
	public:
		Type *operator->()const {
			return place_;
		}

		Type &operator*()const {
			return *place_;
		};

		operator Type *()const {
			return place_;
		};

		
		iterator &operator+(int i)const {
			iterator result(*this);
			result.place_ += i;
			return result;
		}

		iterator &operator+=(int i) {
			place_ += i;
			return *this;
		}

		iterator &operator-(int i)const {
			iterator result(*this);
			result.place_ -= i;
			return result;
		}

		iterator &operator-=(int i) {
			place_ -= i;
			return *this;
		}

		iterator &operator--() {
			place_ -= 1;
			return *this;
		}

		iterator &operator--(int) {
			iterator tmp{*this};
			place_ -= 1;
			return tmp;
		}

		iterator &operator++() {
			place_ += 1;
			return *this;
		}

		iterator &operator++(int) {
			iterator tmp{*this};
			place_ += 1;
			return tmp;
		}

		operator bool()const {
			return static_cast<bool>(place_);
		}

		bool operator==(const iterator &&other)const {
			return place_ == other.place_;
		}

		bool operator!=(const iterator &&other)const {
			return !(*this == other);
		}

		bool operator<(const iterator &&other)const {
			return place_ < other.place_;
		}

		bool operator<=(const iterator &&other)const {
			return place_ <= other.place_;
		}

		bool operator>(const iterator &&other)const {
			return place_ > other.place_;
		}

		bool operator>=(const iterator &&other)const {
			return place_ >= other.place_;
		}

		Type& operator[](int i)const {
			return plce_[i];
		}
		const DinamicArray const *from()const {
			return &collection_;
		}
	private:
		iterator() = delete;

		iterator(const DinamicArray &&position,Type *place)
			:collection_(position),
			place_(place)
		{
		}

		friend class DinamicArray;
		const DinamicArray &collection_;
		Type *place_;
	};

	class const_iterator:public iterator {
	public:
		const Type *operator->()const override{
			return const_cast<const Type *>(place_);
		}

		const Type &operator*()const override {
			return const_cast<const Type &>(*place_);
		};

		operator Type *()const override {
			return const_cast<const Type *>(place_);
		};

		const Type &operator[](int i)const override {
			return const_cast<const Type &>(plce_[i]);
		}
	private:
		const_iterator() = delete;
		const_iterator(const DinamicArray &&position, Type *place)
			:iterator(position, place) {
		}
	};

	iterator begin() {
		return iterator(*this, data_);
	}

	const_iterator cbegin()const {
		return const_iterator(*this, data_);
	}

	iterator end() {
		return iterator(*this, data_ + size_);
	}

	const_iterator cend()const {
		return const_iterator(*this, data_ + size_);
	}

	int size() const {
		return size_;
	}

	int capacity() const {
		return capacity_;
	}

private:
	static struct {
		Type * allocate(size_t n) const {
			/*new char[sizeof(Type) * n]*/
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

//template<>
//class DinamicArray<bool> {
//
//};


#endif // DINAMIC_ARRAY
