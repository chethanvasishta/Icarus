int main(){
	int x;
	if(x)
		x=10;
	if(x){
		x = 15;
		x = x + 5;
		while(x){
			x = x - 2;
			if(x)
				x = 14;
		}
	}
	return x;	
}
