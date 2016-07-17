#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "Poliz.h"

#include "Exception.h"


void EmptyInput::Print() const
{
	fprintf(stderr, SYNT_ERR);
	fprintf(stderr, "Empty input\n");
}


void UnexpectedEOF::Print() const
{
	fprintf(stderr, SYNT_ERR);
	fprintf(stderr, "Unexpected EOF\n");
}


NotWhatExpected::NotWhatExpected(const char *str1, char *str2, int n)
{
	if (str1)
		expected = strndup(str1, strlen(str1));
	else
		str1 = 0;
	if (str2)
		got = strndup(str2, strlen(str2));
	else
		str2 = 0;

	line_number = n;
}


NotWhatExpected::NotWhatExpected(const NotWhatExpected &other)
{
	if (other.GetWhatExpected())
	{
		int len = strlen(other.GetWhatExpected());
		expected = strndup(other.GetWhatExpected(), len);
	}
	else
		expected = 0;
	if (other.GetWhatExpected())
	{
		int len = strlen(other.GetWhatGot());
		got = strndup(other.GetWhatGot(), len);
	}
	else
		got = 0;

	line_number = other.GetLineNumber();
}


void NotWhatExpected::Print() const
{
	fprintf(stderr, SYNT_ERR);
	fprintf(stderr, "expected %s, but %s found ", expected, got);
	fprintf(stderr, "(line %d)\n", line_number);
}


NotWhatExpected::~NotWhatExpected()
{
	if (expected)
		free(expected);
	if (got)
		free(got);
}


UnknownLexeme::UnknownLexeme(char *str, int n)
{
	lexeme = strdup(str);
	line_number = n;
}


UnknownLexeme::UnknownLexeme(const UnknownLexeme &other)
{
	if (other.GetLexeme())
	{
		int len = strlen(other.GetLexeme());
		lexeme = strndup(other.GetLexeme(), len);
	}
	else
		lexeme = 0;
	line_number = other.GetLineNumber();
}


void UnknownLexeme::Print() const
{
	fprintf(stderr, SYNT_ERR);
	fprintf(stderr, "unknown lexeme '%s' ", lexeme);
	fprintf(stderr, "(line %d)\n", line_number);
}


UnknownLexeme::~UnknownLexeme()
{
	if (lexeme)
		free(lexeme);
}


void EmptyLabelName::Print() const
{
	fprintf(stderr, SYNT_ERR);
	fprintf(stderr, "expected label name ");
	fprintf(stderr, "(line %d)\n", line_number);
}


void EmptyVarName::Print() const
{
	fprintf(stderr, SYNT_ERR);
	fprintf(stderr, "expected variable name ");
	fprintf(stderr, "(line %d)\n", line_number);
}


void EmptyFunctionName::Print() const
{
	fprintf(stderr, SYNT_ERR);
	fprintf(stderr, "expected function name ");
	fprintf(stderr, "(line %d)\n", line_number);
}


void IllegalSymbol::Print() const
{
	fprintf(stderr, SYNT_ERR);
	fprintf(stderr, "illegal symbol '%c' ", symbol);
	fprintf(stderr, "(line %d)\n", line_number);
}


PolizEx::PolizEx(char *str, int n)
{
	if (str)
		name = strndup(str, strlen(str));
	else
		name = 0;

	line_number = n;
}


PolizEx::~PolizEx()
{
	if (name)
		free(name);
}


UndefinedLabel::UndefinedLabel(PolizLabel *label)
	: PolizEx(label->GetName(), label->GetLineNumber())
{
	if (label)
		delete label;
}


void UndefinedLabel::Print() const
{
	fprintf(stderr, INT_ERR);
	fprintf(stderr, "label '%s' is not defined ", GetName());
	fprintf(stderr, "(line %d)\n", GetLineNumber());
}


UndefinedVar::UndefinedVar(PolizVar *var)
	: PolizEx(var->GetName(), var->GetLineNumber())

{
	if (var)
		delete var;
}


void UndefinedVar::Print() const
{
	fprintf(stderr, INT_ERR);
	fprintf(stderr, "variable '%s' is not defined ", GetName());
	fprintf(stderr, "(line %d)\n",  GetLineNumber());
}


UndefinedFunction::UndefinedFunction(const PolizOpDefFunction *func)
	: PolizEx(func->GetName(), func->GetLineNumber())
{
	if (func)
		delete func;
}


void UndefinedFunction::Print() const
{
	fprintf(stderr, INT_ERR);
	fprintf(stderr, "function '%s' is not defined ", GetName());
	fprintf(stderr, "(line %d)\n",  GetLineNumber());
}


void DuplicateLabel::Print() const
{
	fprintf(stderr, INT_ERR);
	fprintf(stderr, "duplicate label '%s' ", GetName());
	fprintf(stderr, "(line %d)\n",  GetLineNumber());
}


TooFewArguments::TooFewArguments(PolizOpDefFunction *func)
	: PolizEx(func->GetName(), func->GetLineNumber())
{
	if (func)
		delete func;
}


void TooFewArguments::Print() const
{
	fprintf(stderr, INT_ERR);
	fprintf(stderr, "Too few arguments for function '%s' ", GetName());
	fprintf(stderr, "(line %d)\n",  GetLineNumber());
}


TooManyArguments::TooManyArguments(PolizOpDefFunction *func)
	: PolizEx(func->GetName(), func->GetLineNumber())
{
	if (func)
		delete func;
}


void TooManyArguments::Print() const
{
	fprintf(stderr, INT_ERR);
	fprintf(stderr, "Too many arguments for function '%s' ", GetName());
	fprintf(stderr, "(line %d)\n",  GetLineNumber());
}

