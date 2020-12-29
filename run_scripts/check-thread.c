#include <stdio.h>
#include <pthread.h>

pthread_t p1, p2;





int main(){
	pthread(p1, NULL, callback1, NULL);
	pthread(p2, NULL, callback2, NULL);

	return 0;
}
