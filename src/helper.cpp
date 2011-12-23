#include <stdio.h>
extern "C" {
	void print(int x){
		printf("%d\n",x);
	}
}
