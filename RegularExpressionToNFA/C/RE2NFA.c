#define CVECTOR_LOGARITHMIC_GROWTH

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "cvector.h"

typedef struct Node {
    char input;
    int to;
    struct Node *next;
}Node;

typedef struct {
    int top;
    unsigned capacity;
    int* array;
}Stack;

Stack* createStack(unsigned capacity) {
	Stack* stack = (Stack*)malloc(sizeof(Stack));
    stack->capacity = capacity;
    stack->top = -1;
    stack->array = (int*)malloc(stack->capacity * sizeof(int));
    return stack;
}

int isFull(Stack* stack) {
	return stack->top == stack->capacity - 1;
}

int isEmpty(Stack* stack) {
	return stack->top == -1;
}

void push(Stack* stack, int item) {
    if (isFull(stack))
        return;
    stack->array[++stack->top] = item;
}

int pop(Stack* stack) {
    if (isEmpty(stack))
        return INT_MIN;
    return stack->array[stack->top--];
}

int peek(Stack* stack) {
    if (isEmpty(stack))
        return INT_MIN;
    return stack->array[stack->top];
}

int prec(char c){
	if(c=='*'){
		return 3;
	}else if(c=='.'){
		return 2;
	}else if(c=='+'){
		return 1;
	}else{
		return -1;
	}
}

void post(char *in, char *out) {
	int i, k=0;
	int l = strlen(in);
	Stack* st = createStack(l);
	push(st, 'N');
	
	for(i = 0; i < l; i++) {
		if((in[i] >= 'a' && in[i] <= 'z')||(in[i] >= 'A' && in[i] <= 'Z')) {
			out[k++] = in[i];
		} else if(in[i] == '(') {
			push(st, '(');
		} else if(in[i] == ')') {
			while(peek(st) != 'N' && peek(st) != '(') {
				out[k++] = pop(st);
			}
			if(peek(st) == '(') {
				pop(st);
			}
		} else {
			while(peek(st) != 'N' && prec(in[i]) <= prec((char)peek(st))) {
				out[k++] = pop(st);
			}
			push(st,in[i]);
		}
	}
	while(peek(st) != 'N') {
		out[k++] = pop(st);
	}
	out[k++] = '\0';
}

void printnode(cvector_vector_type(Node*) v) {
	int i,f;
	
	printf("___________________________________________\n");
	printf("| from state\t| input\t| tostates\n");
	for(i=0;i<cvector_size(v);i++) {
		printf("| %d          \t|",i);
		Node* head = v[i];
		printf("%c",head->input);
		f = 1;
		while(head!=NULL) {
			if (f) {
				printf("     \t|");
				f = 0;
			} else {
				printf("     \t");
			}
			printf("%d",head->to);
			head = head->next;
		}
		printf("\n");
	}
	printf("___________________________________________\n");
}

Node* makenode(char in) {
	Node *a = (Node*)malloc(sizeof(Node));
	a->input = in;
	a->to = -1;
	a->next = NULL;
	return a;
}

Node* copynode(Node* a){
	Node *b = (Node*)malloc(sizeof(Node));
	b->input = -1;
	b->to = -1;
	b->next =NULL;
	return b;
}

void andd(cvector_vector_type(Node*) *v, cvector_vector_type(cvector_vector_type(int)) *st){
	int x,y;
	int first,last1;
	y = (*st)[cvector_size((*st))-1][0];
	x = (*st)[cvector_size((*st))-2][1];
	first = (*st)[cvector_size((*st))-2][0];
	last1 = (*st)[cvector_size((*st))-1][1];

	cvector_pop_back((*st));
	cvector_pop_back((*st));

	cvector_vector_type(int) ptemp = NULL;
	cvector_push_back(ptemp, first);
	cvector_push_back(ptemp, last1);
	cvector_push_back((*st), ptemp);

	Node* last = (*v)[y];
	Node * lnode= (*v)[x];
	Node* temp = copynode(last);
	while(lnode->next!=NULL){
		lnode = lnode->next;
	}
	lnode->next = temp;
	lnode->to = y;

}

void orr(cvector_vector_type(Node*) *v, cvector_vector_type(cvector_vector_type(int)) *st){
	int x,y,x1,y1,i;
	
	x = (*st)[cvector_size((*st))-2][0];
	y = (*st)[cvector_size((*st))-1][0];
	x1 = (*st)[cvector_size((*st))-2][1];
	y1 = (*st)[cvector_size((*st))-1][1];
	Node* start = makenode('e');
	Node* end = makenode('e');
	cvector_push_back((*v), start);
	int firstnode = cvector_size((*v)) -1;
	cvector_push_back((*v), end);
	int endnode = cvector_size((*v)) -1;

	cvector_pop_back((*st));
	cvector_pop_back((*st));

	cvector_vector_type(int) ptemp = NULL;
	cvector_push_back(ptemp, firstnode);
	cvector_push_back(ptemp, endnode);
	cvector_push_back((*st), ptemp);

	for(i=0;i<cvector_size((*v))-2;i++){
		Node* h=(*v)[i];
		while(h->next!=NULL){
			if(h->to==x || h->to == y){
				h->to = firstnode;
			}
			h = h->next;
		}
	}


	Node* temp = copynode((*v)[x]);
	Node* temp1 = copynode((*v)[y]);
	Node* t = (*v)[firstnode];
	while(t->next!=NULL){
		t = t->next;
	}
	t->to = x;
	t->next  = temp;
	t->next->to = y;
	t->next->next = temp1;

	Node* adlink = (*v)[x1];
	while(adlink->next!=NULL){
		adlink = adlink->next;
	}

	adlink->to= endnode;
	adlink->next = copynode(end);

	Node* adlink1 = (*v)[y1];
	while(adlink1->next!=NULL){
		adlink1 = adlink1->next;
	}
	adlink1->to = endnode;
	adlink1->next = copynode(end);
}

void closure(cvector_vector_type(Node*) *v, cvector_vector_type(cvector_vector_type(int)) *st){
	int x,x1, i;
	x = (*st)[cvector_size((*st))-1][0];
	x1 = (*st)[cvector_size((*st))-1][1];
	Node* s = makenode('e');
	cvector_push_back((*v), s);
	int firstnode = cvector_size((*v)) -1;
	cvector_pop_back((*st));
	cvector_vector_type(int) ptemp = NULL;
	cvector_push_back(ptemp, x);
	cvector_push_back(ptemp, firstnode);
	cvector_push_back((*st), ptemp);

	for(i=0;i<cvector_size((*v))-2;i++){
		Node* h=(*v)[i];
		while(h->next!=NULL){
			if(h->to==x){
				h->to = firstnode;
			}
			h = h->next;
		}
	}

	Node* t = (*v)[x1];
	while(t->next!=NULL){
		t = t->next;
	}
	t->to = x;
	t->next = copynode(t);
	t->next->to = firstnode;
	t->next->next = copynode(s);
}

int main(int argc, char *argv[]) {
	char re[100], repo[100];
	int l;
	
	printf("Enter a regular expression: ");
	scanf("%s",re);
	
	post(re, repo);
	
	printf("\nPostfix expression: %s\n",repo);
	
	cvector_vector_type(Node*) v = NULL;
	cvector_vector_type(cvector_vector_type(int)) st = NULL;
	int firstnode = 0;
	
	for(l =0 ;l<strlen(repo);l++){
		if(repo[l] !='+' && repo[l]!='*' && repo[l]!='.'){
			Node* temp = makenode(repo[l]);
			cvector_push_back(v, temp);
			cvector_vector_type(int) ptemp = NULL;
			cvector_push_back(ptemp, cvector_size(v)-1);
			cvector_push_back(ptemp, cvector_size(v)-1);
			cvector_push_back(st, ptemp);
		}
		else if(repo[l]=='.'){
			andd(&v,&st);
		}
		else if(repo[l]=='+'){
			orr(&v,&st);
		}
		else if(repo[l]=='*'){
			closure(&v,&st);
		}
	}
	
	printf("\nTrainsition table for given regular expression is - \n");
	printnode(v);
	printf("\nStarting node: %d\n", st[cvector_size(st)-1][0]);
	printf("\nEnding node: %d\n", st[cvector_size(st)-1][1]);
	
	return 0;
}
