#pragma once
#include <functional>

template<class T>
struct LinkedListItem 
{
	LinkedListItem();
	~LinkedListItem();

	LinkedListItem<T>* Previous;
	LinkedListItem<T>* Next;
	T* Item;
};

template<class T>
inline LinkedListItem<T>::LinkedListItem() :
	Next{ nullptr },
	Previous{ nullptr },
	Item{ nullptr }
{
}

template<class T>
inline LinkedListItem<T>::~LinkedListItem()
{
	Previous = nullptr;
	Next = nullptr;
	Item = nullptr;
}


template<class T>
class LinkedList
{
public:
	LinkedList();
	~LinkedList();

	T* operator[](int index);

	void Add(T* value);
	void Delete(T* value);

	void Loop(std::function<void(T* item)> callback);
	void ReverseLoop(std::function<void(T* item)> callback);
	void RangeLoop(int range,std::function<void(int,T*)> callback);

	int GetCount();
	

private:
	LinkedListItem<T>* First;
	LinkedListItem<T>* Last;
	int Count;
};

template<class T>
inline LinkedList<T>::LinkedList():
	First{nullptr},
	Last{nullptr},
	Count{0}
{
}

template<class T>
inline LinkedList<T>::~LinkedList()
{
	delete First;
	delete Last;
}

template<class T>
inline T* LinkedList<T>::operator[](int index)
{
	if (index >= Count) {
		return nullptr;
	}

	LinkedListItem<T>* current{ First };
	int currentIndex{ 0 };
	while (current->Next != nullptr && currentIndex != index) 
	{
		current = current->Next;
	}

	return current->Item;
}

template<class T>
inline void LinkedList<T>::Add(T* value)
{
	if (First == nullptr) {
		First = LinkedListItem<T>{
			nullptr,
			nullptr,
			value
		};
		Last = First;
	}
	else {
		LinkedListItem<T>* temp{ new LinkedListItem<T>
			{
				Last,
				nullptr,
				value
			}
		};
		Last->Next = temp;
		Last = temp;
	}
	++Count;
}

template<class T>
inline void LinkedList<T>::Delete(T* value)
{
	LinkedListItem<T>* current{ First };
	while (current->Item != value) {
		if (current->Next == nullptr) {
			return;
		}
		current = current->Next;
	}

	LinkedListItem<T>* previous{ current->Previous };
	LinkedListItem<T>* next{ current->Next };
	previous->Next = next;
	next->Previous = previous;
	--Count;
}

template<class T>
inline void LinkedList<T>::Loop(std::function<void(T* item)> callback)
{
	if (First == nullptr) {
		return;
	}

	LinkedListItem<T>* current{ First };
	while (current->Next != nullptr) {
		callback(current->Item);
		current = current->Next;
	}
}

template<class T>
inline void LinkedList<T>::ReverseLoop(std::function<void(T* item)> callback)
{
	if (Last == nullptr) {
		return;
	}

	LinkedListItem<T>* current{ Last };
	while (current->Previous != nullptr)
	{
		callback(current->Item);
		current = current->Previous;
	}
}


template<class T>
inline void LinkedList<T>::RangeLoop(int range, std::function<void(int,T*)> callback)
{
	if (First == nullptr) {
		return;
	}

	LinkedListItem<T>* current{ First };
	int index{ 0 };
	while (current->Next != nullptr && index < range)
	{
		callback(current);
		current = current->Next;
		++index;
	}
}

template<class T>
inline int LinkedList<T>::GetCount()
{
	return Count;
}

