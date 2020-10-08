#pragma once


template <typename T>
DynamicArray<T>::DynamicArray()
	: size(0) {}

template <typename T>
DynamicArray<T>::DynamicArray(int size) : size(size) {
	if(size < 0) throw std::length_error(NEGATIVE_SIZE_MESSAGE);

	this->data = new T[size];
}

template <typename T>
DynamicArray<T>::DynamicArray(const T *data, int size) : DynamicArray(size) {
	for(int i = 0; i < size; i++)
		this->data[i] = data[i];
}

template <typename T>
DynamicArray<T>::DynamicArray(const DynamicArray<T> &array, int size) : DynamicArray(size){
	if(size > array.size) throw std::out_of_range(INDEX_OUT_OF_RANGE_MESSAGE);

	for(int i = 0; i < size; i++)
		this->data[i] = array.data[i];
}

template <typename T>
DynamicArray<T>::DynamicArray(const DynamicArray<T> &array) : 
	DynamicArray(array.data, array.size) {}

template <typename T>
DynamicArray<T>::~DynamicArray(){
	delete[] this->data;
	this->size = 0;
}

template <typename T>
T DynamicArray<T>::get(int index) const {
	if(index < 0 || index >= this->size) throw std::out_of_range(INDEX_OUT_OF_RANGE_MESSAGE);

	return this->data[index];
}

template <typename T>
int DynamicArray<T>::getSize() const { return this->size; }

template <typename T>
void DynamicArray<T>::set(const T &value, int index){
	if(index < 0 || index >= this->size) throw std::out_of_range(INDEX_OUT_OF_RANGE_MESSAGE);

	this->data[index] = value;
}

template <typename T>
void DynamicArray<T>::resize(int size){
	if(size < 0) throw std::length_error(NEGATIVE_SIZE_MESSAGE);

	T *newData = new T[size];
	for(int i = 0; i < (this->size > size ? size : this->size); i++)
		newData[i] = data[i];

	delete[] data;
	this->data = newData;

	this->size = size;
}

template <typename T>
bool DynamicArray<T>::operator==(const DynamicArray<T> &arr) const {
	if(this->size != arr.size) return false;

	for(int i = 0; i < this->size; i++)
		if(this->get(i) != arr.get(i)) return false;
	

	return true;
}

template <typename T>
DynamicArray<T>& DynamicArray<T>::operator=(const DynamicArray &array){
	delete[] this->data;

	this->size = array.size;
	this->data = new T[this->size];

	for(int i = 0; i < size; i++)
		this->data[i] = array.data[i];

	return *this;
}


template <typename T>
LinkedList<T>::LinkedList()
	: size(0){}

template <typename T>
LinkedList<T>::LinkedList(const T *items, int size) : LinkedList() {
	if(size < 0) throw std::length_error(NEGATIVE_SIZE_MESSAGE);

	Record **ptr = &(this->head);
	for(int i = 0; i < size; i++){
		*ptr = new Record;
		(*ptr)->item = items[i];
		ptr = &((*ptr)->next);
	}

	this->size = size;
}

template <typename T>
LinkedList<T>::LinkedList(int size) : LinkedList() {
	if(size < 0) throw std::length_error(NEGATIVE_SIZE_MESSAGE);

	Record **ptr = &(this->head);
	for(int i = 0; i < size; i++){
		*ptr = new Record;
		(*ptr)->item = T();
		ptr = &((*ptr)->next);
	}

	this->size = size;
}

template <typename T>
LinkedList<T>::LinkedList(const LinkedList<T> &list){
	Record *ptr = list.head;
	Record **newPtr = &(this->head);

	for(int i = 0; i < list.size; i++, ptr = ptr->next){
		*newPtr = new Record;
		(*newPtr)->item = ptr->item;
		newPtr = &((*newPtr)->next);

	}

	this->size = list.size;
}

template <typename T>
LinkedList<T>::~LinkedList(){
	Record *ptr = this->head;
	Record *next;
	while(ptr != nullptr){
		next = ptr->next;
		delete ptr;
		ptr = next;
	}
	this->size = 0;
}


template <typename T>
T LinkedList<T>::getFirst() const {
	if(this->size == 0) throw std::length_error(ZERO_SIZE_MESSAGE);

	return this->head->item;
}

template <typename T>
T LinkedList<T>::getLast() const {
	if(this->size == 0) throw std::length_error(ZERO_SIZE_MESSAGE);

	Record *ptr = this->head;
	while(ptr->next != nullptr) ptr = ptr->next;
	return ptr->item;
}

template <typename T>
T LinkedList<T>::get(int index) const {
	if(index < 0 || index >= this->size) throw std::out_of_range(INDEX_OUT_OF_RANGE_MESSAGE);


	Record *ptr;
	{
	int i = 0;
	for(i = 0, ptr = this->head; i < index; i++, ptr = ptr->next);
	}

	return ptr->item;
}

template <typename T>
void LinkedList<T>::set(const T &item, int index){
	if(index < 0 || index >= this->size) throw std::out_of_range(INDEX_OUT_OF_RANGE_MESSAGE);


	Record *ptr;
	{
	int i = 0;
	for(i = 0, ptr = this->head; i < index; i++, ptr = ptr->next);
	}

	ptr->item = item;
}

template <typename T>
LinkedList<T>* LinkedList<T>::getSublist(int start, int end) const { //end excluding
	if(!(0 <= start && start <= end && end <= this->size)){
		if(start < 0 || start >= this->size) throw std::out_of_range(INDEX_OUT_OF_RANGE_MESSAGE);
		if(end < 0 || end > this->size) throw std::out_of_range(INDEX_OUT_OF_RANGE_MESSAGE);
		if(start > end) throw std::logic_error("end must be not less than start");
	}

	LinkedList<T> *newList = new LinkedList<T>();


	Record *ptr = this->head;
	Record **newPtr = &newList->head;

	for(int i = 0; i < end; i++, ptr = ptr->next){
		if(i >= start){
			*newPtr = new Record;
			(*newPtr)->item = ptr->item;
			newPtr = &((*newPtr)->next);
		}

	}

	newList->size = end - start;

	return newList;
}

template <typename T>
int LinkedList<T>::getSize() const { return this->size; }

template <typename T>
void LinkedList<T>::append(const T &item){
	Record **ptr = &(this->head);
	while(*ptr != nullptr) ptr = &((*ptr)->next);

	(*ptr) = new Record;
	(*ptr)->item = item;

	this->size++;
}

template <typename T>
void LinkedList<T>::prepend(const T &item){
	Record *ptr = new Record{item, this->head};
	this->head = ptr;

	this->size++;		
}

template <typename T>
void LinkedList<T>::removeAt(int index){
	if(index < 0 || index >= this->size) throw std::out_of_range(INDEX_OUT_OF_RANGE_MESSAGE);


	Record preHead = {.next = this->head};
	Record *ptr;

	{
	int i = 0;
	for(i = 0, ptr = &preHead; i < index; i++, ptr = ptr->next);
	}

	Record *tmp = ptr->next;
	ptr->next = ptr->next->next;
	delete tmp;

	this->head = preHead.next;

	this->size--;
}

template <typename T>
void LinkedList<T>::insertAt(const T &item, int index){
	if(index < 0 || index > this->size) throw std::out_of_range(INDEX_OUT_OF_RANGE_MESSAGE);


	Record preHead = {.next = this->head};
	Record *ptr;

	{
	int i = 0;
	for(i = 0, ptr = &preHead; i < index; i++, ptr = ptr->next);
	}

	ptr->next = new Record{item, ptr->next};
	this->head = preHead.next;

	this->size++;
}

template <typename T>
LinkedList<T>* LinkedList<T>::concat(const LinkedList<T> &list) const {
	Record *ptr1 = this->head;
	Record *ptr2 = list.head;

	LinkedList<T> *newList = new LinkedList<T>();
	Record **ptr = &(newList->head);
	while(ptr1 != nullptr){
		*ptr = new Record{
			ptr1->item,
			ptr1->next
		};

		ptr1 = ptr1->next;
		ptr = &((*ptr)->next);
	}
	while(ptr2 != nullptr){
		*ptr = new Record{
			ptr2->item,
			ptr2->next
		};

		ptr2 = ptr2->next;
		ptr = &((*ptr)->next);
	}

	newList->size = this->size + list.size;
	return newList;
}

template <typename T>
bool LinkedList<T>::operator==(const LinkedList<T> &list) const {
	if(this->size != list.size) return false;

	for(int i = 0; i < this->size; i++){
		if(this->get(i) != list.get(i)) return false;
	}

	return true;
}
