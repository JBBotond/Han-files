#include <stdio.h>

int main() {
	FILE* myFile = fopen("created", "a");
	fprintf(myFile, "this is some text from C lang \n");
	fclose(myFile);
}
