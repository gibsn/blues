#ifndef VARTABLE_H_SENTRY
#define VARTABLE_H_SENTRY


class VarTableRow
{
	char *name;
	int *var_addr;
	VarTableRow *next;

public:
	VarTableRow(const char *str, int i);
	char *GetName() const;
	int *GetVarAddr() const;
	VarTableRow *GetNext() const;
	void SetNext(VarTableRow *new_row);
	~VarTableRow();
};


class VarTable
{
	VarTableRow *table_head;

	static void DeleteVarTable(VarTableRow *head);

public:
	VarTable();
	void AddNewVar(const char *new_name, int new_value);
	int *GetVarAddr(const char *name) const;
	~VarTable();
};




#endif