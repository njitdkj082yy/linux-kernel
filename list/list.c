#include"list.h"
struct student{
	int NumId;
	struct list_head list;
	const char *name;
};
struct student *pos = NULL;
void main()
{
	struct student stu1 = {
			.name = "sunny",
	};
	struct student stu2 = {
			.name = "bob",
	};
	stu1.NumId = 1;
	stu2.NumId = 2;
	struct list_head HEAD;
	INIT_LIST_HEAD(&HEAD);
	list_add(&stu1.list,&HEAD);
	list_add(&stu2.list,&HEAD);
	pos = list_first_entry(HEAD.next,struct student,list);
	printf("pos->NumId=%d,pos->name=%s\n",pos->NumId,pos->name);
}
