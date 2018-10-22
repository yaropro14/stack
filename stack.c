#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


typedef int Data_type;


#define canary 0xBEDABEDA


#define UNIT_TEST(WHAT, OP, EXPECTED)											\
	do {																		\
		Data_type result = (WHAT);												\
		if(result OP EXPECTED) printf(#WHAT"[passed]\n");						\
		else 																	\
		{																		\
			printf(#WHAT"is %d, but %s be %d\n", result,  #OP, EXPECTED);		\
			return -1;															\
		}																		\
	}while(0)																	\


#define UNIT_TEST_POINTER(WHAT, OP, EXPECTED)									\
	do {																		\
		struct Stack * result = (WHAT);											\
		if(result OP EXPECTED) printf(#WHAT"[passed]\n");						\
		else 																	\
		{																		\
			 printf(#WHAT"is NULL, but %s be NULL\n", #OP);						\
			return -1;															\
		}																		\
	}while(0)																	\


struct Stack * stack_create(int size);
int stack_destroy(struct Stack * stack);

int stack_push(struct Stack * stack, Data_type new_element);
Data_type stack_pop(struct Stack * stack);

Data_type stack_get(struct Stack * stack);
int stack_size(struct Stack * stack);
int stack_clear(struct Stack * stack);

int stack_OK(struct Stack * stack);
void print_error(int error);
int unit_test();

Data_type sum_of_elements(struct Stack * stack);


struct Stack
{
	int canary_begin;
	
    int n_data;
    int size;
    Data_type * data;
    Data_type control_sum;
     
    int canary_end;
};


int main()
{
	return unit_test();
}


struct Stack * stack_create(int size)
{
	if(size < 1)
		return NULL;
	
	struct Stack * new_stack = (struct Stack *) calloc(1, sizeof(struct Stack));
	
	new_stack->data = (Data_type *) calloc(size + 2, sizeof(Data_type));
	new_stack->n_data = 0; 
	new_stack->size = size + 2;
	
	new_stack->canary_begin = canary;
	new_stack->canary_end = canary;
	
	new_stack->data[0] = canary;
	new_stack->data[new_stack->size - 1] = canary;
	
	new_stack->control_sum = 0;
	
	return new_stack;
}


int stack_destroy(struct Stack * stack)
{
	assert(stack);
	assert(stack->data);
	
	free(stack->data);
	stack->data = NULL;
	
	free(stack);
	stack = NULL;
	
	return 0;
}


int stack_push(struct Stack * stack, Data_type new_element)
{
	int error = stack_OK(stack);
	
	if(error != 0)
		return error;
		
	if (stack->n_data >= stack->size - 2)
	{
		stack->size += 10;
		stack->data = realloc(stack->data, sizeof(Data_type) * stack->size);
		
		stack->data[stack->size - 11] = 0;
		stack->data[stack->size - 1] = canary;
	}
	
	stack->n_data ++;
	stack->data[stack->n_data] = new_element;
	stack->control_sum += new_element;
	
	error = stack_OK(stack);
	
	return error;
}


Data_type stack_pop(struct Stack * stack)
{
	int error = stack_OK(stack);
	
	if(error != 0)
		return error;
		
	Data_type res;
	
	res = stack->data[stack->n_data];
	stack->n_data --;
	stack->control_sum -= res;
	
	
	
	if((stack->size - 2 - stack->n_data) / 10)
	{
		stack->size -= 10;
		stack->data = realloc(stack->data, sizeof(Data_type) * stack->size);
		
		stack->data[stack->size - 1] = canary;
	}
	
	error = stack_OK(stack);
	if(error != 0)
		return error;
		
	return res;	
}


Data_type stack_get(struct Stack * stack)
{
	int error = stack_OK(stack);
	
	if(error != 0)
		return error;
		
	Data_type res;
	res = stack->data[stack->n_data];
	
	error = stack_OK(stack);
	if(error != 0)
		return error;
	
	return res;
}


int stack_print(struct Stack * stack)
{
	int error = stack_OK(stack);
	
	if(error != 0)
		return error;
		
	Data_type i;
	
	if (stack->n_data == 0)
	{
		print_error(-7);
		return 0;
	}
		
	else
	{
		for (i = 1; i <= stack->n_data; i ++)
		{
			printf("data[%d] = %d ", i, stack->data[i]);
		}
		printf ("\n");
	}
	
	return error = stack_OK(stack);
}


int stack_size(struct Stack * stack)
{
	int error = stack_OK(stack);
	
	if(error != 0)
		return error;
		
	return stack->n_data;
}


int stack_clear(struct Stack * stack)
{
	int error = stack_OK(stack);
	
	if(error != 0)
		return error;
		
	return stack->n_data;
	
	for(int i = 1; i <= stack->n_data; i++)
	{
		stack->data[i] = 0;
	}
	
	stack->n_data = 0;
	
	error = stack_OK(stack);
	return error;
}


int stack_OK(struct Stack * stack)
{
	int err = 0;
	if(stack == NULL)
	{
		err = -1;
		print_error(-1);
	}
		
	if(stack->canary_begin != canary || stack->canary_end != canary)
	{
		err = -1;
		print_error(-2);
	}
		
	if(stack->data[0] != canary || stack->data[stack->size - 1] != canary)
	{
		err = -1;
		print_error(-3);
	}
		
	if(sum_of_elements(stack) != stack->control_sum)
	{
		err = -1;
		print_error(-4);
	}
		
	if(stack->size <= 0)
	{
		err = -1;
		print_error(-5);
	}
		
	if(stack->n_data < 0)
	{
		err = -1;
		print_error(-6);
	}
		
	return err;
}


void print_error(int error)
{
	switch (error){
	
		case -1:
			printf("Stack = NULL\n");
			break;
	
		case -2:
			printf("Struct canaries were died :(\n");
			break;
	
		case -3:
			printf("\n");
			printf("Data canaries were died :(\n");
			break;
	
		case -4:
			printf("Checksum broken\n");
			break;
	
		case -5:
			printf("Stack size <= 0\n");
			break;
	
		case -6:
			printf("Stack data < 0\n");
			break;
	
		case -7:
			printf("Stack is empty\n");
			break;
	}
}


int unit_test()
{
	struct Stack * stack;
	
	UNIT_TEST_POINTER(stack = stack_create(1), !=, NULL);
	UNIT_TEST(stack_push(stack, 48), ==, 0);
	UNIT_TEST(stack_size(stack), ==, stack->n_data);
	UNIT_TEST(stack_get(stack), ==, 48);
	UNIT_TEST(stack_size(stack), ==, stack->n_data);
	UNIT_TEST(stack_pop(stack), ==, 48);
	UNIT_TEST(stack_clear(stack), ==, 0);
	UNIT_TEST(stack_size(stack), ==, stack->n_data);
	UNIT_TEST(stack_destroy(stack), ==, 0);
	
	printf("\nAll tests passed, program work correctly :)\n\n");
	
	return 0;
}


Data_type sum_of_elements(struct Stack * stack)
{
	Data_type sum = 0;
	
	for(int i = 1; i <= stack->n_data; i++)
	{
		sum = sum + stack->data[i];
	}
	
	return sum;
}
