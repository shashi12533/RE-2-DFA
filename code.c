/*
ASHWANI KUMAR
RE 2 DFA CONVERSION
TE-IT ASSIGNMENT
*/
#include<stdio.h>
#include<bits/stdc++.h>
#include<string.h>
#include<malloc.h>
struct tnode
{
	int val;
	int nul;
	char key;
	char first[20];
	char last[20];
	struct tnode *left;
	struct tnode *right;
};
typedef struct tnode tnode;
tnode *nodeInit(char k, int v)
{
	tnode *t;
	t = (tnode*) malloc(sizeof(tnode));
	t->val = v;
	t->key = k;
	t->left =  NULL;
	t->right = NULL;
	if(v)
	{
		t->first[0] = v + 48;
		t->last[0] = v + 48;
		t->first[1] = '\0';
		t->last[1] = '\0';
		t->nul = 0;
	}
	if(k == '*')
		t->nul = 1;
	return t;
}
char *rmv(char *str)
{
	int i,j,f[20]={0};
	for(i = 0 ; i<strlen(str) ; i++)
		f[str[i] - 48] = 1;
	j = 0;
	for(i ='0' ; i<='9' ; i++)
		if(f[i - '0'] == 1)
			str[j++] = i;
	str[j] = '\0';
	return str;
}
char follow[20][20];
tnode *stack[50];
int top = -1;
void push(tnode *temp)
{
	stack[++top] = temp;
}
tnode* pop()
{
	return stack[top--];
}
tnode* createTree(char exp[])
{
	tnode *op,*l,*r;
	int i=0,j=1,k;
	while(i<strlen(exp))
	{
		if(isalpha(exp[i]) || exp[i] == '#')
		{
			push(nodeInit(exp[i++],j++));
			continue ;
		}
		op = nodeInit(exp[i++],0);
		if(op->key == '*')
		{
			l = pop();
			op->left = l;
			strcpy(op->first,l->first);
			strcpy(op->last,l->last);
			for(k=0;k<strlen(op->first);k++)
			{
				strcat(follow[op->first[k] - 48] ,op->last);
				strcpy(follow[op->first[k] - 48],rmv(follow[op->first[k] - 48]));
			}
		}
		else
		{
			r = pop();
			l = pop();
			op->left = l;
			op->right = r;
			if( op->key == '/')
			{
				strcpy(op->first,l->first);
				strcat(op->first,r->first);
				strcpy(op->last,l->last);
				strcat(op->last,r->last);
				if(l->nul == 1 || r->nul == 1)
					op->nul = 1;
				else
					op->nul = 0;
			}
			else
			{
				strcpy(op->first,l->first);
				if(l->nul == 1)
					strcat(op->first,r->first);
				strcpy(op->last,r->last);
				if(r->nul == 1)
					strcat(op->last,l->last);
				if(l->nul == 1 && r->nul == 1)
					op->nul = 1;
				else
					op->nul = 0;
				for(k=0;k<strlen(l->last);k++)
				{
					strcat(follow[l->last[k] - 48] ,r->first);
					strcpy(follow[l->last[k] - 48],rmv(follow[l->last[k] - 48]));
				}
			}
		}
		push(op);
	}
	return pop();
}
int map[30];
void listInPostOrder(tnode* hd)
{
    if(hd != NULL) 
    {
        listInPostOrder(hd->left);
        listInPostOrder(hd->right);
        printf("\n %3d %2c %4d %5s %4s %6s ",hd->val,hd->key,hd->nul,hd->first,hd->last,follow[hd->val]);
        map[hd->val] = hd->key - 'a';
    }
}
char state[30][20];
char left[30][20];
char right[30][20];
void dfa(tnode *head)
{
	int i = 0,k,j=0;
	strcpy(state[0],head->first);
	while(1)
	{
		for(k=0 ; k<strlen(state[i]) ; k++)
			if(map[state[i][k] - 48] == 0)
			{
				strcat(left[i],follow[state[i][k] - 48]);
				strcpy(left[i],rmv(left[i]));
			}
			else if(map[state[i][k] - 48] == 1)
			{
				strcat(right[i],follow[state[i][k] - 48]);
				strcpy(right[i],rmv(right[i]));
			}
		int l=1,r=1;
		for(k=0 ; k<=i ;k++)
		{
			if(!strcmp(state[k],left[i]))
				l = 0;
			if(!strcmp(state[k],right[i]))
				r = 0;
		}
		if(l)
			strcpy(state[++j],left[i]);
		if(r)
			strcpy(state[++j],right[i]);
		if(i == j)
			break ;
		i++ ;
	}
	printf("\n\n Index   State      a       b  ");
	for(k=0 ; k<=i ;k++)
		printf("\n%6d %7s %7s %7s ",k+1,state[k],left[k],right[k]);
}
int main()
{
	char re[20];
	tnode *head;
	printf("\n enter a RE expression : ");
	scanf("%s",re);
	head = createTree(re);
	printf("\n\n Ind Char NUL First Last Follow ");
	listInPostOrder(head);
	printf("\n\n");
	dfa(head);
	return 0;
}
