/*
	Aaron Frost 
	11/29/2020
	Dynamic Array
	Template Class Header & Implementation
*/
	

#pragma once


namespace twoloopstl {

	template<typename T>
	class DynamicArray {

	private:
		unsigned int size;
		unsigned int capacity;
		T* values;

		void Resize();

	public:
		DynamicArray();

		DynamicArray(const unsigned int start_capacity);

		~DynamicArray();

		const T& operator[](const unsigned int index) const;

		void Add(const T& value);

		// Sets value at specified index null, then moves all righthand values left
		void RemoveAt(unsigned int index);

		// Replaces value at index with specified value
		void SetValue(const unsigned int index, const T& value);
		
		// Inserts an element after the specified index, pushes all following elements to the right
		void InsertAfter(const unsigned int index, const T& value);

		const unsigned int Size() const;

		const unsigned int Capacity() const;
	};
}



template<typename T>
twoloopstl::DynamicArray<T>::DynamicArray() : size(0), capacity(2) {
	this->values = new T[this->capacity];
}


template<typename T>
twoloopstl::DynamicArray<T>::DynamicArray(const unsigned int start_capacity) : size(0), capacity(start_capacity) {
	this->values = new T[this->capacity];
}


template<typename T>
twoloopstl::DynamicArray<T>::~DynamicArray() {
	delete[] values;
}


template<typename T>
const T& twoloopstl::DynamicArray<T>::operator[](const unsigned int index) const {
	return values[index];
}


template<typename T>
void twoloopstl::DynamicArray<T>::Add(const T& value) {
	if (size == capacity)
		this->Resize();

	values[size++] = value;
}


// Sets value at specified index null, then moves all righthand values left
template<typename T>
void twoloopstl::DynamicArray<T>::RemoveAt(unsigned int index) {
	values[index] = nullptr;
	for (size_t i = index; i < size - 1; i++)
		values[i] = values[i + 1];
	size--;
}


// Replaces value at index with specified value
template<typename T>
void twoloopstl::DynamicArray<T>::SetValue(const unsigned int index, const T& value) {
	values[index] = value;
}


// Inserts an element after the specified index, pushes all following elements to the right
template<typename T>
void twoloopstl::DynamicArray<T>::InsertAfter(const unsigned int index, const T& value) {
	if (size == capacity)
		this->Resize();

	for (size_t i = size; i > index + 1; i--)
		values[i] = values[i - 1];

	values[index + 1] = value;
	size++;
}


template<typename T>
const unsigned int twoloopstl::DynamicArray<T>::Size() const {
	return size;
}


template<typename T>
const unsigned int twoloopstl::DynamicArray<T>::Capacity() const {
	return capacity;
}


template<typename T>
void twoloopstl::DynamicArray<T>::Resize() {
	capacity *= 2;
	T* resized = new T[capacity];
	for (size_t i = 0; i < size; i++)
		resized[i] = values[i];
	delete[] values;
	values = resized;
}