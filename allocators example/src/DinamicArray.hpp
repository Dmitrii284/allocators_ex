#ifndef DINAMIC_ARRAY
#define DINAMIC_ARRAY

#include <cstdlib>
#include <algorithm>
#include <new>
#include <initializer_list>

/*
Оператор [] 85 -110 это пример перегрузки оператора [] и метод at 
[] и метод at - Это синонимы  Доступ к объекту получаем через элементы массива
Почему возврат происходит по ссылке & -?   тка как у нас клас шаблонный то могут храниться Объекты класса . И по & мы можем получть доступ
к методам этого класса . И по & ссылке будем видеть изменения этого объекта. Так же получаем доступ работы на прямую с этим объектом.

const noexcept - строка 85 
const -  позволяет не менять значения 
noexcept - это своего рода спецификация исключения.  Если вдруг пользователь /0 то пользователь может что то поправить.
Они нужны для соблюдения нотации, 
noexcept - скажет что пометка до компилятора что у нас есть исключения  и там не пишем return at (n)  так как масло масленное получается.


284 __________________
void F01() const{}  вот тут.
int size() const {
		F01(); -  то работать не будет а если в верху добавим const
		return size_;
	} 

	метод size_ является константным И если мы в нутри константного метода решим использовать не константный 

	[] - отсутсвуют в List  Это раздельные элементы занющие друг о друге что то. И когда нам нужен конкретый элемент то операция доступа по индексу Это очень трудозатратная
	и по этому мы идем по коллекции элементов.  По их индексам просто...  
	Тоесть все коллекции у которых есть [] то вы имеете бысрый доступ к нужному элементу.

	ДЗ Теоритическое 
	Найти основные структуры данных и дать им краткое описание. Массивы + Списки +  Использует бинарное дерево. 
	Что это такое ? и как используется? 

	Реализовать класс Коллекцию  Продумать класс таким образом чтобы мог возвращать опеределенное количество элементов должно быть приплюсовано к общему.
	(Армия солдат. И есть транспорт. Надо знать сколько солдат Всего.  
	Надо посчитать и сколько в автобусах. 
	Учесть что в нутри могкт быть и другие коллекции.
	Без шаблонных классов. 
*/




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
