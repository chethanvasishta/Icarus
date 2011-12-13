int main(){
	int x;
	if(x)
		x=10;
	else
		x = 5;
	if(x){
		x = 15;
		x = x + 5;
	}else{
		x = x - 10;
	}		
	return x;
}
