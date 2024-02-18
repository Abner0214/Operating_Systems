#include"mm.h"

int main()
{
					/*myalloc*/
	int *array1;
	int size1=10;

	printf("\t[ mymalloc ]\n");
	printf("+++++++++++++++++++++++++++++++\n\n");

	array1 = (int *)mymalloc(sizeof(int)*size1);
	if (array1 == NULL) {
		printf("error\n");
		exit(1);
	}
	printf("Size = %d\n", size1);
	printf("Address: %p\n", &array1[0]);
	printf("Print array1:\n");
	for (int i = 0; i < size1; i++) {
		array1[i] = i+1;
		printf("%d ", array1[i]);
	}
	printf("\n");

	myfree(array1);
	
					/*myrealloc*/
	int *array2;
	int size2 = 10;

	printf("\n\t[ myrealloc ]\n");
	printf("+++++++++++++++++++++++++++++++\n\n");
	
	array2 = (int *)mymalloc(sizeof(int)*size2);
	if (array2 == NULL) {
		printf("error\n");
		exit(1);
	}
	printf("Size = %d\n", size2);
	printf("Address: %p\n", &array2[0]);
	printf("Print array2:\n");
	for (int i = 0; i < size2; i++) {
		array2[i]=i+1;
		printf("%d ",array2[i]);
	}
	printf("\n");
	printf("-\n");

	
	size2 = 20;
	array2 = myrealloc(array2, sizeof(int)*size2);

	if (array2==NULL) {
		printf("error\n");
		exit(1);
	}
	printf("Size = %d\n", size2);
	printf("Address: %p\n", &array2[0]);
	printf("Print array2:\n");
	for (int i = 0; i < size2; i++) {
		array2[i] = i+1;
		printf("%d ", array2[i]);
	}
	printf("\n");
	printf("-\n");

	size2=5;
	array2 = myrealloc(array2,sizeof(int)*size2);

	if (array2 == NULL) {
		printf("error\n");
		exit(1);
	}	

	printf("Size = %d\n", size2);
	printf("Address: %p\n", &array2[0]);
	printf("print array2:\n");
	for (int i = 0; i < size2; i++) {
		array2[i]=i+1;
		printf("%d ", array2[i]);
	}
	printf("\n");

	myfree(array2);
	
					/*calloc*/
	int *array3;
	int size3 = 10;
	printf("\n\t[ mycalloc ]\n");
	printf("+++++++++++++++++++++++++++++++\n\n");
	
	array3 = (int *)mycalloc(size3, sizeof(int)*size3);
	if (array3 == NULL) {
		printf("error\n");
		exit(1);
	}
	
	printf("Size = %d\n", size3);
	printf("Address:%p\n", &array3[0]);
	printf("Print array3:\n");
	for (int i = 0; i < size3; i++) {
		printf("%d ",array3[i]);
	}
	printf("\n");
	printf("-\n");
	printf("Size = %d\n", size3);
	printf("Address: %p\n", &array3[0]);
	printf("Print array3:\n");
	for (int i=0; i < size3; i++) {
		array3[i] = i+1;
		printf("%d ", array3[i]);
	}
	printf("\n");
	
	myfree(array3);

	return 0;
}

