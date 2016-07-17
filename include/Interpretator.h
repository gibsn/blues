#ifndef INTERPRETATOR_H_SENTRY
#define INTERPRETATOR_H_SENTRY

#include "Stack.h"


class LabelTable;
class VarTable;
class PolizElem;
struct PolizList;


class Interpretator
{
	Stack<PolizElem> poliz_stack;
	PolizList *cur_cmd;
	PolizList *poliz_head;

public:
	static VarTable var_table;
	static LabelTable label_table;
	void Run();
	void SetPolizHead(PolizList *p) { poliz_head = p; }
};


#endif
