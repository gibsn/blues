#ifndef LABELTABLE_H_SENTRY
#define LABELTABLE_H_SENTRY


struct PolizList;


class LabelTableRow
{
	char *name;
	PolizList *addr;
	LabelTableRow *next;

public:
	LabelTableRow(char *str, PolizList *p);
	char *GetName() const;
	PolizList *GetAddr() const;
	LabelTableRow *GetNext() const;
	void SetAddr(PolizList *p);
	void SetNext(LabelTableRow *new_row);
	~LabelTableRow();
};


class LabelTable
{
	LabelTableRow *table_head;
	
	static void DeleteLabelTable(LabelTableRow *head);

public:
	LabelTable();
	void AddNewLabel(char *str, PolizList *addr1, int n);
	PolizList *GetLabelAddr(const char *name) const;
	~LabelTable();
};


#endif