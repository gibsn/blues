#include <string.h>
#include <stdlib.h>

#include "VarTable.h"


VarTableRow::VarTableRow(const char *str, int i)
	: name(0),
	value(i),
	next(0)
{
	if (str)
		name = strdup(str);
#if 0
	var_addr = new int;
	*var_addr = i;
#endif
}


VarTableRow::~VarTableRow()
{
	if (name)
		free(name);
#if 0
	if (var_addr)
		delete var_addr;
#endif
}


VarTable::VarTable()
	: table_head(0)
{
}


void VarTable::AddNewVar(const char *new_name, int new_value)
{
	VarTableRow *new_var = new VarTableRow(new_name, new_value);

	new_var->SetNext(table_head);
	table_head = new_var;
}


void VarTable::Assign(const char *name, int value)
{
	VarTableRow *variable = GetVarByName(name);

	if (!variable)
		AddNewVar(name, value);
	else
		variable->SetVariable(value);
}


VarTableRow *VarTable::GetVarByName(const char *name)
{
	VarTableRow *iterator = table_head;

	while (iterator) {
		if (!strcmp(iterator->GetName(), name))
			return iterator;
		else
			iterator = iterator->GetNext();
	}

	return 0;
}


void VarTable::DeleteVarTable(VarTableRow *head)
{
	if (head) {
		DeleteVarTable(head->GetNext());
		delete head;
	}
}


VarTable::~VarTable()
{
	DeleteVarTable(table_head);
}

