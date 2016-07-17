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
	PolizInt *Clone() const;
	int GetVal() const;
	~PolizInt() {};
};


class PolizStr : public PolizConst
{
	char *str;

public:
	PolizStr(char *string);
	PolizStr *Clone() const;
	char *GetStr() const;
	~PolizStr();
};


class PolizVar : public PolizConst
{
	char *name;
	int *addr;
	int line_number;

public:
	PolizVar(char *str, int n);
	PolizVar *Clone() const;
	char *GetName() const;
	int GetLineNumber() const;
	int *GetAddr() const;
	~PolizVar();
};


class PolizLabel : public PolizConst
{
	char *name;
	PolizList *addr;
	int line_number;

public:
	PolizLabel(char *str, PolizList *addr1, int n);
	PolizLabel *Clone() const;
	PolizList *GetAddr() const;
	int GetLineNumber() const;
	char *GetName() const;
	void SetAddr(PolizList *addr1);
	~PolizLabel();
};


class PolizPrint : public PolizFunction
{
	int argc;

public:
	PolizPrint(int n);
	static void PrintArgs(int n, Stack<PolizElem> &stack);
	PolizElem *EvaluateFun(Stack<PolizElem> &stack) const;
	~PolizPrint() {};
};


class PolizTurn : public PolizFunction
{

public:
	PolizElem *EvaluateFun(Stack<PolizElem> &stack) const;
	~PolizTurn() {};
};


class PolizBuild : public PolizFunction
{

public:
	PolizElem *EvaluateFun(Stack<PolizElem> &stack) const;
	~PolizBuild() {};
};


class PolizProd : public PolizFunction
{

public:
	PolizElem *EvaluateFun(Stack<PolizElem> &stack) const;
	~PolizProd() {};
};


class PolizBuy : public PolizFunction
{

public:
	PolizElem *EvaluateFun(Stack<PolizElem> &stack) const;
	~PolizBuy() {};
};


class PolizSell : public PolizFunction
{

public:
	PolizElem *EvaluateFun(Stack<PolizElem> &stack) const;
	~PolizSell() {};
};


class PolizOpPlus : public PolizFunction
{

public:
	PolizElem *EvaluateFun(Stack<PolizElem> &stack) const;
	~PolizOpPlus() {};
};


class PolizOpMinus : public PolizFunction
{

public:
	PolizElem *EvaluateFun(Stack<PolizElem> &stack) const;
	~PolizOpMinus() {};
};


class PolizOpGreater : public PolizFunction
{

public:
	PolizElem *EvaluateFun(Stack<PolizElem> &stack) const;
	~PolizOpGreater() {};
};


class PolizOpEqual : public PolizFunction
{

public:
	PolizElem *EvaluateFun(Stack<PolizElem> &stack) const;
	~PolizOpEqual() {};
};


class PolizOpLess : public PolizFunction
{

public:
	PolizElem *EvaluateFun(Stack<PolizElem> &stack) const;
	~PolizOpLess() {};
};


class PolizOpDereference : public PolizFunction
{
	int line_number;

public:
	PolizOpDereference(int n);
	PolizElem *EvaluateFun(Stack<PolizElem> &stack) const;
	~PolizOpDereference() {};
};


class PolizOpAssign : public PolizFunction
{

public:
	PolizElem *EvaluateFun(Stack<PolizElem> &stack) const;
	~PolizOpAssign() {};
};


class PolizOpNot : public PolizFunction
{

public:
	PolizElem *EvaluateFun(Stack<PolizElem> &stack) const;
	~PolizOpNot() {};
};


class PolizOpMultiply : public PolizFunction
{

public:
	PolizElem *EvaluateFun(Stack<PolizElem> &stack) const;
	~PolizOpMultiply() {};
};


class PolizOpDivide : public PolizFunction
{

public:
	PolizElem *EvaluateFun(Stack<PolizElem> &stack) const;
	~PolizOpDivide() {};
};


class PolizOpUnMinus : public PolizFunction
{

public:
	PolizElem *EvaluateFun(Stack<PolizElem> &stack) const;
	~PolizOpUnMinus() {};
};



class PolizOpAnd : public PolizFunction
{

public:
	PolizElem *EvaluateFun(Stack<PolizElem> &stack) const;
	~PolizOpAnd() {};
};


class PolizOpOr : public PolizFunction
{

public:
	PolizElem *EvaluateFun(Stack<PolizElem> &stack) const;
	~PolizOpOr() {};
};


class PolizOpIndex : public PolizFunction
{

public:
	PolizElem *EvaluateFun(Stack<PolizElem> &stack) const;
	~PolizOpIndex() {};
};


class PolizOpNOP : public PolizFunction
{

public:
	PolizElem *EvaluateFun(Stack<PolizElem> &stack) const;
	~PolizOpNOP() {};
};


class PolizOpDefFunction : public PolizFunction
{
	char *name;
	int line_number;
	int argc;

public:
	PolizOpDefFunction(char *str, int n, int k);
	PolizElem *EvaluateFun(Stack<PolizElem> &stack) const;
	char *GetName() const;
	int GetLineNumber() const;
	int GetArgc() const;
	~PolizOpDefFunction();
};


#endif