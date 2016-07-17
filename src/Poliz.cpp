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
	PolizElem *res;

	res = EvaluateFun(stack);
	if (res)
		stack.Push(res);
	cur_cmd = cur_cmd->next;
}


PolizInt::PolizInt(int a)
{
	value = a;
}


PolizInt *PolizInt::Clone() const
{
	return new PolizInt(value);
}


int PolizInt::GetVal() const
{
	return value;
}


PolizStr::PolizStr(char *string)
{
	if (string)
	{
		if (string[0] == '"')
			str = strndup(string+1, strlen(string)-2);
		else
			str = strndup(string, strlen(string));
	}
	else
		str = 0;
}


PolizStr *PolizStr::Clone() const
{
	return new PolizStr(str);
}


char *PolizStr::GetStr() const
{
	return str;
} 


PolizStr::~PolizStr()
{
	if (str)
		free(str);
}


PolizVar::PolizVar(char *str, int n)
{
	if (str)
		name = strndup(str, strlen(str));
	else
		str = 0;
	line_number = n;
}


char *PolizVar::GetName() const
{
	return name;
}


int PolizVar::GetLineNumber() const
{
	return line_number;
}


int *PolizVar::GetAddr() const
{
	return addr;
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
{
	if (str)
		name = strndup(str, strlen(str));
	else
		name = 0;
	addr = addr1;
	line_number = n;
}


PolizList *PolizLabel::GetAddr() const
{
	return addr;
}


int PolizLabel::GetLineNumber() const
{
	return line_number;
}


char *PolizLabel::GetName() const
{
	return name;
}


void PolizLabel::SetAddr(PolizList *addr1)
{
	addr = addr1;
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
	PolizLabel *label;

	stack.Pop(tmp);
	label = dynamic_cast<PolizLabel *>(tmp);
	cur_cmd = label->GetAddr();
	if (!cur_cmd)
		throw UndefinedLabel(label);
	if (label)
		delete label;
}


void PolizOpGoFalse::Evaluate(Stack<PolizElem> &stack,PolizList *&cur_cmd) const
{
	PolizElem *tmp;
	PolizLabel *label;
	PolizInt *expression;

	stack.Pop(tmp);
	label = dynamic_cast<PolizLabel *>(tmp);
	stack.Pop(tmp);
	expression = dynamic_cast<PolizInt *>(tmp);
	if (expression->GetVal() == 0)
	{
		cur_cmd = label->GetAddr();
		if (!cur_cmd)
			throw UndefinedLabel(label);
	}
	else
		cur_cmd = cur_cmd->next;
	if (label)
		delete label;
	if (expression)
		delete expression;
}


PolizPrint::PolizPrint(int n)
{
	argc = n;
}


void PolizPrint::PrintArgs(int n, Stack<PolizElem> &stack)
{
	PolizElem *tmp;
	PolizInt *op_int;
	PolizStr *op_str;

	if (n > 0)
	{
		stack.Pop(tmp);
		PrintArgs(n-1, stack);
		op_int = dynamic_cast<PolizInt *>(tmp);
		if (op_int)
		{
			printf("%d", op_int->GetVal());
			if (op_int)
				delete op_int;
		}
		else
		{
			op_str = dynamic_cast<PolizStr *>(tmp);
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


PolizElem *PolizTurn::EvaluateFun(Stack<PolizElem> &stack) const
{
	maintenance.SendMessage("turn");
	return 0;
}


PolizElem *PolizBuild::EvaluateFun(Stack<PolizElem> &stack) const
{
	maintenance.SendMessage("build");
	return 0;
}


PolizElem *PolizProd::EvaluateFun(Stack<PolizElem> &stack) const
{
	PolizElem *op1;
	PolizInt *op11;
	char tmp[buffer_size];

	stack.Pop(op1);
	op11 = dynamic_cast<PolizInt *>(op1);
	sprintf(tmp, "prod %d", op11->GetVal());
	maintenance.SendMessage(tmp);
	if (op11)
		delete op11;
	return 0;
}


PolizElem *PolizBuy::EvaluateFun(Stack<PolizElem> &stack) const
{
	PolizElem *op1, *op2;
	PolizInt *op11, *op22;
	char tmp[buffer_size];

	stack.Pop(op1);
	stack.Pop(op2);
	op11 = dynamic_cast<PolizInt *>(op1);
	op22 = dynamic_cast<PolizInt *>(op2);
	sprintf(tmp, "buy %d %d", op22->GetVal(), op11->GetVal());
	maintenance.SendMessage(tmp);
	if (op11)
		delete op11;
	if (op22)
		delete op22;
	return 0;
}


PolizElem *PolizSell::EvaluateFun(Stack<PolizElem> &stack) const
{
	PolizElem *op1, *op2;
	PolizInt *op11, *op22;
	char tmp[buffer_size];

	stack.Pop(op1);
	stack.Pop(op2);
	op11 = dynamic_cast<PolizInt *>(op1);
	op22 = dynamic_cast<PolizInt *>(op2);
	sprintf(tmp, "sell %d %d", op22->GetVal(), op11->GetVal());
	maintenance.SendMessage(tmp);
	if (op11)
		delete op11;
	if (op22)
		delete op22;
	return 0;
}


PolizElem *PolizOpPlus::EvaluateFun(Stack<PolizElem> &stack) const
{
	PolizElem *op1, *op2;
	PolizInt *op11, *op22;
	int res;

	stack.Pop(op1);
	stack.Pop(op2);
	op11 = dynamic_cast<PolizInt *>(op1);
	op22 = dynamic_cast<PolizInt *>(op2);
	res = op22->GetVal() + op11->GetVal();
	if (op11)
		delete op11;
	if (op22)
		delete op22;
	return new PolizInt(res);
}


PolizElem *PolizOpMinus::EvaluateFun(Stack<PolizElem> &stack) const
{
	PolizElem *op1, *op2;
	PolizInt *op11, *op22;
	int res;

	stack.Pop(op1);
	stack.Pop(op2);
	op11 = dynamic_cast<PolizInt *>(op1);
	op22 = dynamic_cast<PolizInt *>(op2);
	res = op22->GetVal() - op11->GetVal();
	if (op11)
		delete op11;
	if (op22)
		delete op22;
	return new PolizInt(res);
}


PolizElem *PolizOpGreater::EvaluateFun(Stack<PolizElem> &stack) const
{
	PolizElem *op1, *op2;
	PolizInt *op11, *op22;
	int res;

	stack.Pop(op1);
	stack.Pop(op2);
	op11 = dynamic_cast<PolizInt *>(op1);
	op22 = dynamic_cast<PolizInt *>(op2);
	res = op22->GetVal() > op11->GetVal();
	if (op11)
		delete op11;
	if (op22)
		delete op22;
	return new PolizInt(res);
}


PolizElem *PolizOpEqual::EvaluateFun(Stack<PolizElem> &stack) const
{
	PolizElem *op1, *op2;
	PolizInt *op11, *op22;
	int res;

	stack.Pop(op1);
	stack.Pop(op2);
	op11 = dynamic_cast<PolizInt *>(op1);
	op22 = dynamic_cast<PolizInt *>(op2);
	res = op22->GetVal() == op11->GetVal();
	if (op11)
		delete op11;
	if (op22)
		delete op22;
	return new PolizInt(res);
}


PolizElem *PolizOpLess::EvaluateFun(Stack<PolizElem> &stack) const
{
	PolizElem *op1, *op2;
	PolizInt *op11, *op22;
	int res;

	stack.Pop(op1);
	stack.Pop(op2);
	op11 = dynamic_cast<PolizInt *>(op1);
	op22 = dynamic_cast<PolizInt *>(op2);
	res = op22->GetVal() < op11->GetVal();
	if (op11)
		delete op11;
	if (op22)
		delete op22;
	return new PolizInt(res);
}


PolizOpDereference::PolizOpDereference(int n)
{
	line_number = n;
}


PolizElem *PolizOpDereference::EvaluateFun(Stack<PolizElem> &stack) const
{
	PolizElem *op1;
	int *var_addr;
	PolizVar *op11;

	stack.Pop(op1);
	op11 = dynamic_cast<PolizVar *>(op1);
	var_addr = Interpretator::var_table.GetVarAddr(op11->GetName());
	if (var_addr)
	{
		if (op11)
			delete op11;
		return new PolizInt(*var_addr);
	}
	else
		throw UndefinedVar(op11);
}


PolizElem *PolizOpAssign::EvaluateFun(Stack<PolizElem> &stack) const
{
	PolizElem *op1, *op2;
	int *var_addr;
	PolizInt *op11;
	PolizVar *op22;

	stack.Pop(op1);
	stack.Pop(op2);
	op11 = dynamic_cast<PolizInt *>(op1);
	op22 = dynamic_cast<PolizVar *>(op2);
	var_addr = Interpretator::var_table.GetVarAddr(op22->GetName());
	if (!var_addr)
		Interpretator::var_table.AddNewVar(op22->GetName(), op11->GetVal());
	else
		*var_addr = op11->GetVal();
	if (op11)
		delete op11;
	if (op22)
		delete op22;
	return 0;
}


PolizElem *PolizOpNot::EvaluateFun(Stack<PolizElem> &stack) const
{
	PolizElem *op1;
	PolizInt *op11;
	int res;

	stack.Pop(op1);
	op11 = dynamic_cast<PolizInt *>(op1);
	res = !(op11->GetVal());
	if (op11)
		delete op11;
	return new PolizInt(res);
}


PolizElem *PolizOpMultiply::EvaluateFun(Stack<PolizElem> &stack) const
{
	PolizElem *op1, *op2;
	PolizInt *op11, *op22;
	int res;

	stack.Pop(op1);
	stack.Pop(op2);
	op11 = dynamic_cast<PolizInt *>(op1);
	op22 = dynamic_cast<PolizInt *>(op2);
	res = op22->GetVal() * op11->GetVal();
	if (op11)
		delete op11;
	if (op22)
		delete op22;
	return new PolizInt(res);
}


PolizElem *PolizOpDivide::EvaluateFun(Stack<PolizElem> &stack) const
{
	PolizElem *op1, *op2;
	PolizInt *op11, *op22;
	int res;

	stack.Pop(op1);
	stack.Pop(op2);
	op11 = dynamic_cast<PolizInt *>(op1);
	op22 = dynamic_cast<PolizInt *>(op2);
	res = op22->GetVal() / op11->GetVal();
	if (op11)
		delete op11;
	if (op22)
		delete op22;
	return new PolizInt(res);
}


PolizElem *PolizOpUnMinus::EvaluateFun(Stack<PolizElem> &stack) const
{
	PolizElem *op1;
	int res;
	PolizInt *op11;

	stack.Pop(op1);
	op11 = dynamic_cast<PolizInt *>(op1);
	res = -(op11->GetVal());
	if (op11)
		delete op11;
	return new PolizInt(res);
}


PolizElem *PolizOpAnd::EvaluateFun(Stack<PolizElem> &stack) const
{
	PolizElem *op1, *op2;
	PolizInt *op11, *op22;
	int res;

	stack.Pop(op1);
	stack.Pop(op2);
	op11 = dynamic_cast<PolizInt *>(op1);
	op22 = dynamic_cast<PolizInt *>(op2);
	res = op22->GetVal() && op11->GetVal();
	if (op11)
		delete op11;
	if (op22)
		delete op22;
	return new PolizInt(res);
}


PolizElem *PolizOpOr::EvaluateFun(Stack<PolizElem> &stack) const
{
	PolizElem *op1, *op2;
	PolizInt *op11, *op22;
	int res;

	stack.Pop(op1);
	stack.Pop(op2);
	op11 = dynamic_cast<PolizInt *>(op1);
	op22 = dynamic_cast<PolizInt *>(op2);
	res = op22->GetVal() || op11->GetVal();
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
	int line_number;
	PolizInt *op11;
	PolizVar *op22;

	stack.Pop(op1);
	stack.Pop(op2);
	op11 = dynamic_cast<PolizInt *>(op1);
	op22 = dynamic_cast<PolizVar *>(op2);
	sprintf(new_name, "%s[%d]", op22->GetName(), op11->GetVal());
	line_number = op22->GetLineNumber();
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


PolizOpDefFunction::PolizOpDefFunction(char *str, int n, int k)
{
	if (str)
		name = strndup(str, strlen(str));
	else
		name = 0;
	line_number = n;
	argc = k;
}


PolizElem *PolizOpDefFunction::EvaluateFun(Stack<PolizElem> &stack) const
{
	if (!strcmp(name, "?supply"))
		return FuncSupply();
	else
	if (!strcmp(name, "?raw_price"))
		return FuncRawPrice();
	else
	if (!strcmp(name, "?demand"))
		return FuncDemand();
	else
	if (!strcmp(name, "?production_price"))
		return FuncProductionPrice();
	else
	if (!strcmp(name, "?my_id"))
		return FuncMyId();
	else
	if (!strcmp(name, "?active_players"))
		return FuncActivePlayers();
	else
	if (!strcmp(name, "?money"))
		return FuncMoney(stack);
	else
	if (!strcmp(name, "?raw"))
		return FuncRaw(stack);
	else
	if (!strcmp(name, "?production"))
		return FuncProduction(stack);
	else
	if (!strcmp(name, "?factories"))
		return FuncFactories(stack);
	else
	if (!strcmp(name, "?random"))
		return FuncRandom(stack);
	else
	if (!strcmp(name, "?result_raw_bought"))
		return FuncResultRawBought(stack);
	else
	if (!strcmp(name, "?result_raw_price"))
		return FuncResultRawPrice(stack);
	else
	if (!strcmp(name, "?result_prod_sold"))
		return FuncResultProdSold(stack);
	else
	if (!strcmp(name, "?result_prod_price"))
		return FuncResultProdPrice(stack);
	else
	if (!strcmp(name, "?factories_being_built"))
		return FuncFactoriesBeingBuilt(stack);
	else
		throw UndefinedFunction(this);
}


char *PolizOpDefFunction::GetName() const
{
	return name;
}


int PolizOpDefFunction::GetLineNumber() const
{
	return line_number;
}


int PolizOpDefFunction::GetArgc() const
{
	return argc;
}


PolizOpDefFunction::~PolizOpDefFunction()
{
	if (name)
		free(name);
}

