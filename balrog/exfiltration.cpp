#include "pch.h"
#include "exfiltration.h"
#include <iostream>

using namespace std;

void startExfiltration() {
	char PATH_VARIABLE[1000];
	GetEnvironmentVariableA("PATH", PATH_VARIABLE, 1000);
	printf("PATH Variable : %s", PATH_VARIABLE);
	printf("Started startExfiltration\n");
}