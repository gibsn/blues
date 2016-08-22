#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "LabelTable.h"
#include "VarTable.h"
#include "Exception.h"
#include "Common.h"
#include "Interpretator.h"

#include "Poliz.h"


PolizList::~PolizList()
{
	delete element;
}

void PolizConst::Evaluate(Stack<PolizElem> &stack, PolizList *&cur_cmd) const
{
	stack.Push(Clone());
	cur_cmd = cur_cmd->next;
}


void PolizFunction::Evaluate(Stack<PolizElem> &stack, PolizList *&cur_cmd) const
{
	PolizElem *res = EvaluateFun(stack);

	if (res)
		stack.Push(res);

	cur_cmd = cur_cmd->next;
}


PolizInt::PolizInt(int a)
	: value(a)
{
}


PolizInt *PolizInt::Clone() const
{
	return new PolizInt(value);
}


PolizStr::PolizStr(char *string)
	: str(0)
{
	if (string) {
		if (string[0] == '"')
			str = strndup(string + 1, strlen(string) - 2);
		else
			str = strdup(string);
	}
}


PolizStr *PolizStr::Clone() const
{
	return new PolizStr(str);
}


PolizStr::~PolizStr()
{
	if (str)
		free(str);
}


PolizVar::PolizVar(char *str, int n)
	: name(0),
	line_number(n)
{
	if (str)
		name = strdup(str);
}


PolizVar *PolizVar::Clone() const
{
	return new PolizVar(name, line_number);
}


PolizVar::~PolizVar()
{
	if (name)
		free(name);
}


PolizLabel::PolizLabel(char *str, PolizList *addr1, int n)
	: name(0),
	addr(addr1),
	line_number(n)
{
	if (str)
		name = strdup(str);
}


PolizLabel *PolizLabel::Clone() const
{
	PolizList *label_addr;

	if (addr)
		label_addr = addr;
	else
		label_addr = Interpretator::label_table.GetLabelAddr(name);

	return new PolizLabel(name, label_addr, line_number);
}


PolizLabel::~PolizLabel()
{
	if (name)
		free(name);
}


void PolizOpGo::Evaluate(Stack<PolizElem> &stack, PolizList *&cur_cmd) const
{
	PolizElem *tmp;

	stack.Pop(tmp);
	PolizLabel *label = dynamic_cast<PolizLabel *>(tmp);
	cur_cmd = label->GetAddr();

	if (!cur_cmd)
		throw UndefinedLabel(label);
	if (label)
		delete label;
}


void PolizOpGoFalse::Evaluate(Stack<PolizElem> &stack,PolizList *&cur_cmd) const
{
	PolizElem *tmp;

	stack.Pop(tmp);
	PolizLabel *label = dynamic_cast<PolizLabel *>(tmp);

	stack.Pop(tmp);
	PolizInt *expression = dynamic_cast<PolizInt *>(tmp);

	if (expression->GetVal() == 0) {
		cur_cmd = label->GetAddr();
		if (!cur_cmd)
			throw UndefinedLabel(label);
	} else {
		cur_cmd = cur_cmd->next;
	}

	if (label)
		delete label;
	if (expression)
		delete expression;
}


PolizPrint::PolizPrint(int n)
	: argc(n)
{
}


void PolizPrint::PrintArgs(int n, Stack<PolizElem> &stack)
{
	PolizElem *tmp;

	if (n > 0) {
		stack.Pop(tmp);
		PrintArgs(n - 1, stack);
		PolizInt *op_int = dynamic_cast<PolizInt *>(tmp);
		if (op_int) {
			printf("%d", op_int->GetVal());

			if (op_int)
				delete op_int;
		} else {
			PolizStr *op_str = dynamic_cast<PolizStr *>(tmp);
			printf("%s", op_str->GetStr());

			if (op_str)
				delete op_str;
		}
	}
}


PolizElem* PolizPrint::EvaluateFun(Stack<PolizElem> &stack) const
{
	PrintArgs(argc, stack);
	putchar('\n');

	return 0;
}


PolizElem *PolizOpPlus::EvaluateFun(Stack<PolizElem> &stack) const
{
	PolizElem *op1, *op2;

	stack.Pop(op1);
	stack.Pop(op2);
	PolizInt *op11 = dynamic_cast<PolizInt *>(op1);
	PolizInt *op22 = dynamic_cast<PolizInt *>(op2);
	int res = op22->GetVal() + op11->GetVal();

	if (op11)
		delete op11;
	if (op22)
		delete op22;

	return new PolizInt(res);
}


PolizElem *PolizOpMinus::EvaluateFun(Stack<PolizElem> &stack) const
{
	PolizElem *op1, *op2;

	stack.Pop(op1);
	stack.Pop(op2);
	PolizInt *op11 = dynamic_cast<PolizInt *>(op1);
	PolizInt *op22 = dynamic_cast<PolizInt *>(op2);
	int res = op22->GetVal() - op11->GetVal();

	if (op11)
		delete op11;
	if (op22)
		delete op22;

	return new PolizInt(res);
}


PolizElem *PolizOpGreater::EvaluateFun(Stack<PolizElem> &stack) const
{
	PolizElem *op1, *op2;

	stack.Pop(op1);
	stack.Pop(op2);
	PolizInt *op11 = dynamic_cast<PolizInt *>(op1);
	PolizInt *op22 = dynamic_cast<PolizInt *>(op2);
	int res = op22->GetVal() > op11->GetVal();

	if (op11)
		delete op11;
	if (op22)
		delete op22;

	return new PolizInt(res);
}


PolizElem *PolizOpEqual::EvaluateFun(Stack<PolizElem> &stack) const
{
	PolizElem *op1, *op2;

	stack.Pop(op1);
	stack.Pop(op2);
	PolizInt *op11 = dynamic_cast<PolizInt *>(op1);
	PolizInt *op22 = dynamic_cast<PolizInt *>(op2);
	int res = op22->GetVal() == op11->GetVal();

	if (op11)
		delete op11;
	if (op22)
		delete op22;

	return new PolizInt(res);
}


PolizElem *PolizOpLess::EvaluateFun(Stack<PolizElem> &stack) const
{
	PolizElem *op1, *op2;

	stack.Pop(op1);
	stack.Pop(op2);
	PolizInt *op11 = dynamic_cast<PolizInt *>(op1);
	PolizInt *op22 = dynamic_cast<PolizInt *>(op2);
	int res = op22->GetVal() < op11->GetVal();

	if (op11)
		delete op11;
	if (op22)
		delete op22;

	return new PolizInt(res);
}


PolizElem *PolizOpDereference::EvaluateFun(Stack<PolizElem> &stack) const
{
	PolizElem *op1;

	stack.Pop(op1);
	PolizVar *op11 = dynamic_cast<PolizVar *>(op1);
	VarTableRow *variable = Interpretator::var_table.GetVarByName(op11->GetName());

	if (variable) {
		if (op11)
			delete op11;

		return new PolizInt(variable->GetValue());
	} else {
		throw UndefinedVar(op11);
	}
}


PolizElem *PolizOpAssign::EvaluateFun(Stack<PolizElem> &stack) const
{
	PolizElem *op1, *op2;

	stack.Pop(op1);
	stack.Pop(op2);
	PolizInt *op11 = dynamic_cast<PolizInt *>(op1);
	PolizVar *op22 = dynamic_cast<PolizVar *>(op2);

	Interpretator::var_table.Assign(op22->GetName(), op11->GetVal());

	if (op11)
		delete op11;
	if (op22)
		delete op22;

	return 0;
}


PolizElem *PolizOpNot::EvaluateFun(Stack<PolizElem> &stack) const
{
	PolizElem *op1;

	stack.Pop(op1);
	PolizInt *op11 = dynamic_cast<PolizInt *>(op1);
	int res = !(op11->GetVal());

	if (op11)
		delete op11;

	return new PolizInt(res);
}


PolizElem *PolizOpMultiply::EvaluateFun(Stack<PolizElem> &stack) const
{
	PolizElem *op1, *op2;

	stack.Pop(op1);
	stack.Pop(op2);
	PolizInt *op11 = dynamic_cast<PolizInt *>(op1);
	PolizInt *op22 = dynamic_cast<PolizInt *>(op2);
	int res = op22->GetVal() * op11->GetVal();

	if (op11)
		delete op11;
	if (op22)
		delete op22;

	return new PolizInt(res);
}


PolizElem *PolizOpDivide::EvaluateFun(Stack<PolizElem> &stack) const
{
	PolizElem *op1, *op2;

	stack.Pop(op1);
	stack.Pop(op2);
	PolizInt *op11 = dynamic_cast<PolizInt *>(op1);
	PolizInt *op22 = dynamic_cast<PolizInt *>(op2);
	int	res = op22->GetVal() / op11->GetVal();

	if (op11)
		delete op11;
	if (op22)
		delete op22;

	return new PolizInt(res);
}


PolizElem *PolizOpUnMinus::EvaluateFun(Stack<PolizElem> &stack) const
{
	PolizElem *op1;

	stack.Pop(op1);
	PolizInt *op11 = dynamic_cast<PolizInt *>(op1);
	int res = -(op11->GetVal());

	if (op11)
		delete op11;

	return new PolizInt(res);
}


PolizElem *PolizOpAnd::EvaluateFun(Stack<PolizElem> &stack) const
{
	PolizElem *op1, *op2;

	stack.Pop(op1);
	stack.Pop(op2);
	PolizInt *op11 = dynamic_cast<PolizInt *>(op1);
	PolizInt *op22 = dynamic_cast<PolizInt *>(op2);
	int res = op22->GetVal() && op11->GetVal();

	if (op11)
		delete op11;
	if (op22)
		delete op22;

	return new PolizInt(res);
}


PolizElem *PolizOpOr::EvaluateFun(Stack<PolizElem> &stack) const
{
	PolizElem *op1, *op2;

	stack.Pop(op1);
	stack.Pop(op2);
	PolizInt *op11 = dynamic_cast<PolizInt *>(op1);
	PolizInt *op22 = dynamic_cast<PolizInt *>(op2);
	int res = op22->GetVal() || op11->GetVal();

	if (op11)
		delete op11;
	if (op22)
		delete op22;

	return new PolizInt(res);
}


PolizElem *PolizOpIndex::EvaluateFun(Stack<PolizElem> &stack) const
{
	PolizElem *op1, *op2;
	char new_name[buffer_size];

	stack.Pop(op1);
	stack.Pop(op2);
	PolizInt *op11 = dynamic_cast<PolizInt *>(op1);
	PolizVar *op22 = dynamic_cast<PolizVar *>(op2);
	sprintf(new_name, "%s[%d]", op22->GetName(), op11->GetVal());
	int	line_number = op22->GetLineNumber();

	if (op11)
		delete op11;
	if (op22)
		delete op22;

	return new PolizVar(new_name, line_number);
}


PolizElem *PolizOpNOP::EvaluateFun(Stack<PolizElem> &stack) const
{
	return 0;
}

