#include <stdexcept>


template <typename T> class Sequence{
public:

	virtual T getFirst() const = 0;
	virtual T getLast() const = 0;
	virtual T get(int index) const = 0;
	virtual int getSize() const = 0;

	virtual Sequence<T>* clone() const {
		return this->getSubsequence(0, this->getSize());
	}

	virtual Sequence<T>* getSubsequence(int start, int end) const = 0; //end excluding

	virtual void set(const T &item, int index) = 0;
	virtual void append(const T &item) = 0;
	virtual void prepend(const T &item) = 0;
	virtual void insertAt(const T &item, int index) = 0;

	virtual Sequence<T>* concat(const Sequence<T>& seq) const = 0;

	virtual bool operator==(const Sequence<T> &seq) const {
		if(this->getSize() != seq.getSize()) return false;

		for(int i = 0; i < this->getSize(); i++){
			if(this->get(i) != seq.get(i)) return false;
		}

		return true;
	}

};


template <typename T> class ArraySequence : public Sequence<T>{
protected:
	DynamicArray<T> *array;

public:
	ArraySequence(){
		this->array = new DynamicArray<T>();
	}

	ArraySequence(const ArraySequence<T> &seq){
		this->array = new DynamicArray<T>(*seq.array);
	}

	ArraySequence(DynamicArray<T>* array){
		this->array = array;
	}


	ArraySequence(T *items, int size){
		this->array = new DynamicArray<T>(items, size);
	}

	ArraySequence(int size){
		this->array = new DynamicArray<T>(size);
	}

	virtual ~ArraySequence(){
		delete this->array;
	}

	virtual T getFirst() const override {
		return this->array->get(0);
	}

	virtual T getLast() const override {
		return this->array->get(this->getSize() - 1);
	}

	virtual T get(int index) const override {
		return this->array->get(index);
	}

	virtual int getSize() const override {
		return this->array->getSize();
	}

	virtual void set(const T &item, int index) override {
		if(index < 0 || index >= this->getSize()) throw std::out_of_range(INDEX_OUT_OF_RANGE_MESSAGE);

		this->array->set(item, index);
	}


	virtual ArraySequence<T>* getSubsequence(int start, int end) const override {
		if(!(0 <= start && start <= end && end <= this->getSize())){
			if(start < 0 || start >= this->getSize()) throw std::out_of_range(INDEX_OUT_OF_RANGE_MESSAGE);
			if(end < 0 || end > this->getSize()) throw std::out_of_range(INDEX_OUT_OF_RANGE_MESSAGE);
			if(start > end) throw std::logic_error("end must be not less than start");
		}

		T subArray[end - start];
		for(int i = 0; i < end - start; i++)
			subArray[i] = this->array->get(i + start);
		
		ArraySequence<T> *subSequence = new ArraySequence<T>(subArray, end - start);

		return subSequence;
	}

	virtual void append(const T &item) override {
		this->array->resize(this->getSize() + 1);
		this->array->set(item, this->getSize() - 1);
	}

	virtual void prepend(const T &item) override {
		this->array->resize(this->getSize() + 1);
		T t1 = this->array->get(0);
		T t2;
		for(int i = 0; i < this->getSize() - 1; i++){
			t2 = t1;
			t1 = this->array->get(i + 1);
			this->array->set(t2, i + 1);
		}
		this->array->set(item, 0);
	}

	virtual void insertAt(const T &item, int index) override {
		if(index < 0 || index > this->getSize()) throw std::out_of_range(INDEX_OUT_OF_RANGE_MESSAGE);

		this->array->resize(this->getSize() + 1);
		T t1 = this->array->get(index);
		T t2;
		for(int i = index; i < this->getSize() - 1; i++){
			t2 = t1;
			t1 = this->array->get(i + 1);
			this->array->set(t2, i + 1);
		}
		this->array->set(item, index);
	}

	virtual ArraySequence<T>* concat(const Sequence<T>& seq) const override {
		DynamicArray<T> *array = new DynamicArray<T>(this->getSize() + seq.getSize());
		ArraySequence<T> *newSequence = new ArraySequence<T>(array);
		for(int i = 0; i < this->getSize(); i++)
			newSequence->set(this->get(i), i);

		for(int i = 0; i < seq.getSize(); i++)
			newSequence->set(seq.get(i), i + this->getSize());

		return newSequence;
	}

};


template <typename T> class ListSequence : public Sequence<T>{
protected:
	LinkedList<T> *list;
public:
	ListSequence(){
		this->list = new LinkedList<T>();
	}

	ListSequence(const ListSequence<T> &seq){
		this->list = new LinkedList<T>(*seq.list);
	}

	ListSequence(LinkedList<T> *list){
		this->list = list;
	}

	ListSequence(T *items, int size){
		this->list = new LinkedList<T>(items, size);
	}

	ListSequence(int size){
		this->list = new LinkedList<T>(size);
	}

	virtual ~ListSequence(){
		delete this->list;
	}


	virtual T getFirst() const override {
		return this->list->getFirst();
	}
	
	virtual T getLast() const override {
		return this->list->getLast();
	}

	virtual T get(int index) const override {
		return this->list->get(index);
	}

	virtual int getSize() const override {
		return this->list->getSize();
	}

	virtual ListSequence<T>* getSubsequence(int start, int end) const override {
		LinkedList<T> *subList = this->list->getSublist(start, end);
		ListSequence<T> *seq = new ListSequence<T>(subList);
		return seq;
	}

	virtual void set(const T &item, int index) override {
		this->list->set(item, index);
	}

	virtual void append(const T &item) override {
		this->list->append(item);
	}

	virtual void prepend(const T &item) override {
		this->list->prepend(item);
	}

	virtual void insertAt(const T &item, int index) override {
		this->list->insertAt(item, index);
	}

	virtual ListSequence<T>* concat(const Sequence<T>& seq) const override {
		ListSequence<T> *newSequence = new ListSequence<T>();

		for(int i = 0; i < this->getSize(); i++)
			newSequence->append(this->get(i));

		for(int i = 0; i < seq.getSize(); i++)
			newSequence->append(seq.get(i));

		return newSequence;
	}


};