#include "Stack.h"


template <class T>
void Stack<T>::DeleteStack(StackElement<T> *head)
{
	if (head) {
		DeleteStack(head->next);
		delete head;
	}
}

template <class T>
void Stack<T>::Pop(T *&dest)
{
	T *what_to_return = 0;
	StackElement<T> *tmp;

	if (head) {
		what_to_return = head->address;
		tmp = head;
		head = head->next;
		delete tmp;
	}

	dest = what_to_return;
}

template <class T>
void Stack<T>::Push(T *data)
{
	StackElement<T> *tmp;

	tmp = new StackElement<T>;
	tmp->address = data;
	tmp->next = head;
	head = tmp;
}

template <class T>
Stack<T>::Stack()
{
	head = 0;
}

template <class T>
Stack<T>::~Stack()
{
	DeleteStack(head);
}


template class Stack<Lexeme>;
template class Stack<PolizElem>;
template class Stack<int>;


