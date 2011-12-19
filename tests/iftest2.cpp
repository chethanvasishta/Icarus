int main(){
	int x;
	x = 1;
	if(x)
		x=10;
	if(x){
		x = 15;
		x = x + 5;
		while(x){
			x = x - 2;
			if(x)
				x = 2;
		}
	}
	return x;	
}
