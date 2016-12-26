#include "stdafx.h"
#include <stdio.h>

#include "DebugLogging.h"

#include <Windows.h>

FILE * f;

void Debug_Start() {
	f = fopen(".\\com.kenstudio.luacq.log","w+");
}

void Debug_Stop() {
	fclose(f);
}


void Debug_Write(char * s) {
	fwrite(s, 1, strlen(s), f);
	fflush(f);
}