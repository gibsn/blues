#ifndef POLIZ_H_SENTRY
#define POLIZ_H_SENTRY

#include "Stack.h"


class PolizElem;
class Maintenance;

extern Maintenance maintenance;


struct PolizList
{
	PolizElem *element;
	PolizList *next;

	~PolizList();
};


class PolizElem
{

public:
	virtual void Evaluate(Stack<PolizElem> &stack, PolizList *&cur_cmd) 
		const = 0;
	virtual ~PolizElem() {};
};


class PolizConst : public PolizElem
{

public:
	void Evaluate(Stack<PolizElem> &stack, PolizList *&cur_cmd) const;
	virtual PolizElem *Clone() const = 0;
	~PolizConst() {};
};


class PolizFunction : public PolizElem
{

public:
	virtual PolizElem *EvaluateFun(Stack<PolizElem> &stack) const = 0;
	void Evaluate(Stack<PolizElem> &stack, PolizList *&cur_cmd) const;
	~PolizFunction() {};
};


class PolizOpGo : public PolizElem
{

public:
	void Evaluate(Stack<PolizElem> &stack, PolizList *&cur_cmd) const;
	~PolizOpGo() {};
};


class PolizOpGoFalse : public PolizElem
{

public:
	void Evaluate(Stack<PolizElem> &stack, PolizList *&cur_cmd) const;
	~PolizOpGoFalse() {};
};


class PolizInt : public PolizConst
{
	int value;

public:
	PolizInt(int a);
	~PolizInt() {};

	int GetVal() const { return value; }

	PolizInt *Clone() const;
};


class PolizStr : public PolizConst
{
	char *str;

public:
	PolizStr(char *string);
	~PolizStr();

	char *GetStr() const { return str; }

	PolizStr *Clone() const;
};


class PolizVar : public PolizConst
{
	char *name;
	int *addr;
	int line_number;

public:
	PolizVar(char *str, int n);
	~PolizVar();

	char *GetName() const { return name; }
	int GetLineNumber() const { return line_number; }
	int *GetAddr() const { return addr; }

	PolizVar *Clone() const;
};


class PolizLabel : public PolizConst
{
	char *name;
	PolizList *addr;
	int line_number;

public:
	PolizLabel(char *str, PolizList *addr1, int n);
	~PolizLabel();

	PolizList *GetAddr() const { return addr; }
	int GetLineNumber() const { return line_number; }
	char *GetName() const { return name; }
	void SetAddr(PolizList *addr1) { addr = addr1; }

	PolizLabel *Clone() const;
};


class PolizPrint : public PolizFunction
{
	int argc;

	static void PrintArgs(int n, Stack<PolizElem> &stack);

public:
	PolizPrint(int n);
	~PolizPrint() {};

	PolizElem *EvaluateFun(Stack<PolizElem> &stack) const;
};


class PolizOpPlus : public PolizFunction
{

public:
	~PolizOpPlus() {};

	PolizElem *EvaluateFun(Stack<PolizElem> &stack) const;
};


class PolizOpMinus : public PolizFunction
{

public:
	~PolizOpMinus() {};

	PolizElem *EvaluateFun(Stack<PolizElem> &stack) const;
};


class PolizOpGreater : public PolizFunction
{

public:
	~PolizOpGreater() {};

	PolizElem *EvaluateFun(Stack<PolizElem> &stack) const;
};


class PolizOpEqual : public PolizFunction
{

public:
	~PolizOpEqual() {};

	PolizElem *EvaluateFun(Stack<PolizElem> &stack) const;
};


class PolizOpLess : public PolizFunction
{

public:
	~PolizOpLess() {};

	PolizElem *EvaluateFun(Stack<PolizElem> &stack) const;
};


class PolizOpDereference : public PolizFunction
{

public:
	~PolizOpDereference() {};

	PolizElem *EvaluateFun(Stack<PolizElem> &stack) const;
};


class PolizOpAssign : public PolizFunction
{

public:
	~PolizOpAssign() {};

	PolizElem *EvaluateFun(Stack<PolizElem> &stack) const;
};


class PolizOpNot : public PolizFunction
{

public:
	~PolizOpNot() {};

	PolizElem *EvaluateFun(Stack<PolizElem> &stack) const;
};


class PolizOpMultiply : public PolizFunction
{

public:
	~PolizOpMultiply() {};

	PolizElem *EvaluateFun(Stack<PolizElem> &stack) const;
};


class PolizOpDivide : public PolizFunction
{

public:
	~PolizOpDivide() {};

	PolizElem *EvaluateFun(Stack<PolizElem> &stack) const;
};


class PolizOpUnMinus : public PolizFunction
{

public:
	~PolizOpUnMinus() {};

	PolizElem *EvaluateFun(Stack<PolizElem> &stack) const;
};



class PolizOpAnd : public PolizFunction
{

public:
	~PolizOpAnd() {};

	PolizElem *EvaluateFun(Stack<PolizElem> &stack) const;
};


class PolizOpOr : public PolizFunction
{

public:
	~PolizOpOr() {};

	PolizElem *EvaluateFun(Stack<PolizElem> &stack) const;
};


class PolizOpIndex : public PolizFunction
{

public:
	~PolizOpIndex() {};

	PolizElem *EvaluateFun(Stack<PolizElem> &stack) const;
};


class PolizOpNOP : public PolizFunction
{

public:
	~PolizOpNOP() {};

	PolizElem *EvaluateFun(Stack<PolizElem> &stack) const;
};


#endif
