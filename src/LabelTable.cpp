#include <string.h>
#include <stdlib.h>

#include "Poliz.h"
#include "Exception.h"

#include "LabelTable.h"


LabelTableRow::LabelTableRow(char *str, PolizList *p)
{
	if (str)
		name = strndup(str, strlen(str));
	else
		name = 0;
	addr = p;
	next = 0;
}


char *LabelTableRow::GetName() const
{
	return name;
}


PolizList *LabelTableRow::GetAddr() const
{
	return addr;
}


LabelTableRow *LabelTableRow::GetNext() const
{
	return next;
}


void LabelTableRow::SetAddr(PolizList *p)
{
	addr = p;
}


void LabelTableRow::SetNext(LabelTableRow *new_row)
{
	next = new_row;
}


LabelTableRow::~LabelTableRow()
{
	if (name)
		free(name);
}


LabelTable::LabelTable()
{
	table_head = 0;
}


void LabelTable::AddNewLabel(char *str, PolizList* addr1, int n)
{
	LabelTableRow *tmp = table_head;

	while(tmp)
		if (!strcmp(tmp->GetName(), str))
		{
			if (tmp->GetAddr())
				throw DuplicateLabel(str, n);
			tmp->SetAddr(addr1);
			return;
		}
		else
			tmp = tmp->GetNext();
	LabelTableRow *new_label = new LabelTableRow(str, addr1);
	new_label->SetNext(table_head);
	table_head = new_label;
}


PolizList *LabelTable::GetLabelAddr(const char *name) const
{
	LabelTableRow *tmp = table_head;

	while(tmp)
		if (!strcmp(tmp->GetName(), name))
			return tmp->GetAddr();
		else
			tmp = tmp->GetNext();
	return 0;
}


void LabelTable::DeleteLabelTable(LabelTableRow *head)
{
	if (head)
	{
		DeleteLabelTable(head->GetNext());
		delete head;
	}
}


LabelTable::~LabelTable()
{
	DeleteLabelTable(table_head);
}










