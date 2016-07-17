 #ifndef EXCEPTION_H_SENTRY
#define EXCEPTION_H_SENTRY

#define SYNT_ERR "\n\x1b[31mSyntax error:\x1b[0m "
#define INT_ERR "\n\x1b[31mInterpretation error:\x1b[0m "


class PolizLabel;
class PolizVar;
class PolizOpDefFunction;


class Exception
{

public:
	virtual void Print() const = 0;
	virtual ~Exception() {};
};


class SyntaxEx : public Exception
{

public:
	~SyntaxEx() {};
};


class PolizEx : public Exception
{
	char *name;
	int line_number;

public:
	PolizEx(char *str, int n);
	const char *GetName() const { return name; }
	int GetLineNumber() const { return line_number; }
	~PolizEx();
};


class EmptyInput : public SyntaxEx
{

public:
	void Print() const;
	~EmptyInput() {};
};


class UnexpectedEOF : public SyntaxEx
{

public:
	void Print() const;
	~UnexpectedEOF() {};
};


class NotWhatExpected : public SyntaxEx
{
	char *expected;
	char *got;
	int line_number;

public:
	NotWhatExpected(const char *str1, char *str2, int n);
	NotWhatExpected(const NotWhatExpected &other);
	const char *GetWhatExpected() const { return expected; }
	const char *GetWhatGot() const { return got; }
	int GetLineNumber() const { return line_number; }
	void Print() const;
	~NotWhatExpected();
};


class UnknownLexeme : public SyntaxEx
{
	char *lexeme;
	int line_number;

public:
	UnknownLexeme(char *str, int n);
	UnknownLexeme(const UnknownLexeme &other);
	const char *GetLexeme() const { return lexeme; }
	int GetLineNumber() const { return line_number; }
	void Print() const;
	~UnknownLexeme();
};


class EmptyLabelName : public SyntaxEx
{
	int line_number;

public:
	EmptyLabelName(int n) : line_number(n) {}
	void Print() const;
	~EmptyLabelName() {};
};


class EmptyFunctionName : public SyntaxEx
{
	int line_number;

public:
	EmptyFunctionName(int n) : line_number(n) {}
	void Print() const;
	~EmptyFunctionName() {};
};


class EmptyVarName : public SyntaxEx
{
	int line_number;

public:
	EmptyVarName(int n) : line_number(n) {}
	void Print() const;
	~EmptyVarName() {};
};


class IllegalSymbol : public SyntaxEx
{
	char symbol;
	int line_number;

public:
	IllegalSymbol(char c, int n) : symbol(c), line_number(n) {}
	void Print() const;
	~IllegalSymbol() {};
};


class UndefinedLabel : public PolizEx
{

public:
	UndefinedLabel(PolizLabel *label);
	void Print() const;
	~UndefinedLabel() {};
};



class UndefinedVar : public PolizEx
{

public:
	UndefinedVar(PolizVar *var);
	void Print() const;
	~UndefinedVar() {};
};


class UndefinedFunction : public PolizEx
{

public:
	UndefinedFunction(const PolizOpDefFunction *func);
	void Print() const;
	~UndefinedFunction() {};
};


class DuplicateLabel : public PolizEx
{

public:
	DuplicateLabel(char *str, int n) : PolizEx(str, n) {};
	void Print() const;
	~DuplicateLabel() {};
};


class TooFewArguments : public PolizEx
{

public:
	TooFewArguments(PolizOpDefFunction *func);
	void Print() const;
	~TooFewArguments() {};
};


class TooManyArguments : public PolizEx
{

public:
	TooManyArguments(PolizOpDefFunction *func);
	void Print() const;
	~TooManyArguments() {};
};


#endif
