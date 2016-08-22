#include <string.h>
#include <stdlib.h>

#include "Poliz.h"
#include "Exception.h"

#include "LabelTable.h"


LabelTableRow::LabelTableRow(char *str, PolizList *p)
	: name(0),
	addr(p),
	next(0)
{
	if (str)
		name = strdup(str);
}


LabelTableRow::~LabelTableRow()
{
	if (name)
		free(name);
}


LabelTable::LabelTable()
	: table_head(0)
{
}


void LabelTable::AddNewLabel(char *str, PolizList* addr1, int n)
{
	LabelTableRow *iterator = table_head;

	while(iterator) {
		if (!strcmp(iterator->GetName(), str)) {
			if (iterator->GetAddr())
				throw DuplicateLabel(str, n);

			iterator->SetAddr(addr1);
			return;
		} else {
			iterator = iterator->GetNext();
		}
	}

	LabelTableRow *new_label = new LabelTableRow(str, addr1);
	new_label->SetNext(table_head);
	table_head = new_label;
}


PolizList *LabelTable::GetLabelAddr(const char *name) const
{
	LabelTableRow *iterator = table_head;

	while(iterator) {
		if (!strcmp(iterator->GetName(), name))
			return iterator->GetAddr();
		else
			iterator = iterator->GetNext();
	}

	return 0;
}


void LabelTable::DeleteLabelTable(LabelTableRow *head)
{
	if (head) {
		DeleteLabelTable(head->GetNext());
		delete head;
	}
}


LabelTable::~LabelTable()
{
	DeleteLabelTable(table_head);
}










