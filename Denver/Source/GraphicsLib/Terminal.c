#include <GraphicsLib/Terminal.h>
#include <StringUtils.h>
#include <stdarg.h>

#define FORMAT_NEW '%'
#define FORMAT_NEWLINE 'n'
#define FORMAT_TABULATE 't'
#define FORMAT_STRING 's'
#define FORMAT_INT 'i'
#define FORMAT_BOOLEAN 'b'
#define FORMAT_HEX 'x'
#define FORMAT_PTR 'p'

typedef struct TagTerminal BootTerm;

static const char CONVERSION_TABLE[16] = "0123456789ABCDEF";
static Boolean gStreamOn = False;
static BootTerm* gTag = NULL;
static void(*gConOut)(char* text, SizeT sz);

void Console(BootloaderHeader* bs) {
#ifndef __PRODUCTION__
    if (gStreamOn) return;

	gTag = BootloaderTag(bs, EKBOOT_STRUCT_TAG_TERMINAL_ID);
	if (gTag == NULL || gTag->rows < 1) return;

	gConOut = (VoidPtr)gTag->term_write;
    gStreamOn = True;
#endif
}

void ConsoleEnableLogging(Boolean show) { gStreamOn = show; }

void Log(char* message) {
#ifndef __PRODUCTION__
    if (!gStreamOn) return;
	if (gConOut == NULL) return;

 	gConOut(message, StringLength(message));
#endif
}


void ConsoleLogChar(Char x) {
	Char chr[2];
	chr[0] = x;
	chr[1] = '\0';
	Log(chr);
}

void __ConsoleLogInteger(SizeT num) {
#ifndef __PRODUCTION__
	int i;
	char buf[21] = {0};

	if (!num) {
		Log("0");
		return;
	}

	for (i = 19; num; i--) {
		buf[i] = (num % 10) + 0x30;
		num = num / 10;
	}

	i++;
	Log(buf + i);
#endif
}

void ConsoleLogInteger(SizeT width) {
#ifndef __PRODUCTION__
	__ConsoleLogInteger(width);
#endif
}

void __ConsoleLogHex(SizeT num) {
#ifndef __PRODUCTION__
	int i;
	char buf[17];

	if (!num) {
		Log("0x0");
		return;
	}

	buf[16] = 0;

	for (i = 15; num; i--) {
		buf[i] = CONVERSION_TABLE[num % 16];
		num /= 16;
	}

	i++;
	Log("0x");
	Log(&buf[i]);
#endif
}

void ConsoleLogHex(SizeT width) {
#ifndef __PRODUCTION__
	__ConsoleLogHex(width);
#endif
}

void ConsoleLogEx(char* fmt, ...) {
#ifndef __PRODUCTION__
    if (!gStreamOn) return;

	Boolean fmtOn = False;
	SizeT index = 0;

	while (fmt[index] != '\0') {
		if (fmt[index] == FORMAT_NEW) {
			fmtOn = True;
			break;
		}

		++index;
	}

	if (fmtOn) {
		index = 0; // reset it again

		va_list args;
		va_start(args, fmt);

		Boolean fmtEnable = False;

		while (fmt[index] != '\0') {
			switch (fmt[index]) {
				case FORMAT_BOOLEAN: {
					if (!fmtEnable) break;

					Log(va_arg(args, int) ? "Yes" : "No");
					fmtEnable = False;
					break;
				}
				case FORMAT_HEX: {
					if (!fmtEnable) break;

					int arg = va_arg(args, int);
					__ConsoleLogHex(arg);

					fmtEnable = False;
					break;
				}
				case FORMAT_INT: {
					if (!fmtEnable) break;

					int arg = va_arg(args, int);
					__ConsoleLogInteger(arg);

					fmtEnable = False;
					break;
				}
				case FORMAT_TABULATE: {
					if (!fmtEnable) break;

					Log("\t");
					fmtEnable = False;
					break;
				}
				case FORMAT_NEWLINE: {
					if (!fmtEnable) break;

					Log("\n");
					fmtEnable = False;
					break;
				}
				case FORMAT_STRING: {
					if (!fmtEnable) break;

					Log(va_arg(args, char*));

					fmtEnable = False;
					break;
				}
				case FORMAT_NEW:
					fmtEnable = True;
					break;
			}

			++index;
		}

   	 	va_end(args);
		return;
	}

	Log(fmt);
#endif
}

void ConsoleClear(void) { Log("\e[2J\e[H"); }