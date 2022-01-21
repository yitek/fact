#include <stdio.h>

int calc(int i) {
	int k = 3;
	k +=i;
	i=k*i;
	return i;
}

int main(int argc, char** argv){
	printf("input a number:");
	fflush(stdout); 
	int c=3;
	// 必须用attach方式才能做控制台输入
	// rewind(stdin);
	// scanf("%d",&c);
	int result = calc(c);
	printf("result: %i", result);
}