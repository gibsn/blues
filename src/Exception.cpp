#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "Poliz.h"

#include "Exception.h"


void EmptyInput::Print() const
{
	fprintf(stderr, SYNT_ERR "Empty input\n");
}


void UnexpectedEOF::Print() const
{
	fprintf(stderr, SYNT_ERR "Unexpected EOF\n");
}


NotWhatExpected::NotWhatExpected(const char *str1, char *str2, int n)
	: expected(0),
	got(0),
	line_number(n)
{
	if (str1)
		expected = strdup(str1);

	if (str2)
		got = strdup(str2);
}


NotWhatExpected::NotWhatExpected(const NotWhatExpected &other)
	: expected(0),
	got(0),
	line_number(other.line_number)
{
	if (other.GetWhatExpected())
		expected = strdup(other.GetWhatExpected());

	if (other.GetWhatExpected())
		got = strdup(other.GetWhatGot());
}


void NotWhatExpected::Print() const
{
	fprintf(stderr, SYNT_ERR "expected %s, but %s found ", expected, got);
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
	: lexeme(0),
	line_number(n)
{
	if (lexeme)
		lexeme = strdup(str);
}


UnknownLexeme::UnknownLexeme(const UnknownLexeme &other)
	: lexeme(0),
	line_number(other.line_number)
{
	if (other.GetLexeme())
		lexeme = strdup(other.GetLexeme());
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
	fprintf(stderr, SYNT_ERR "expected label name ");
	fprintf(stderr, "(line %d)\n", line_number);
}


void EmptyVarName::Print() const
{
	fprintf(stderr, SYNT_ERR "expected variable name ");
	fprintf(stderr, "(line %d)\n", line_number);
}


void EmptyFunctionName::Print() const
{
	fprintf(stderr, SYNT_ERR "expected function name ");
	fprintf(stderr, "(line %d)\n", line_number);
}


void IllegalSymbol::Print() const
{
	fprintf(stderr, SYNT_ERR "illegal symbol '%c' ", symbol);
	fprintf(stderr, "(line %d)\n", line_number);
}


PolizEx::PolizEx(char *str, int n)
	: name(0),
	line_number(n)
{
	if (str)
		name = strdup(str);
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
	fprintf(stderr, INT_ERR "label '%s' is not defined ", GetName());
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
	fprintf(stderr, INT_ERR "variable '%s' is not defined ", GetName());
	fprintf(stderr, "(line %d)\n",  GetLineNumber());
}


void DuplicateLabel::Print() const
{
	fprintf(stderr, INT_ERR "duplicate label '%s' ", GetName());
	fprintf(stderr, "(line %d)\n",  GetLineNumber());
}

