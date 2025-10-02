#include <stdio.h>

int main() {
	FILE* dataFile = fopen("./myFile", "r");
	if(dataFile == NULL)
		printf("Not opened \n");
	else {
		printf("Succesfully opened \n");
		char input = fscanf(dataFile, "%c", &input);
		printf("Value: %c \n");
	}
	fclose(dataFile);
}

