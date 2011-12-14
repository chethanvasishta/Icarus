int fib(int x){
	if(x)
		return fib(x-1) + fib(x-2);
	return 0;
}

int main(){
	int x;
	x = 3;
	x = fib(x);
	return x;
}
