#ifndef STACK_H_SENTRY
#define STACK_H_SENTRY


struct Lexeme;
class PolizElem;


template <class T>
struct StackElement
{
	T *address;
	StackElement *next;
};


template <class T>
class Stack
{
	StackElement<T> *head;

	static void DeleteStack(StackElement<T> *head);

public:
	Stack();
	~Stack();

	void Pop(T *&dest);
	void Push(T *data);
};


#endif
