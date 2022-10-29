#include <lib.h>
#include <syscalls.h>
#include <stdarg.h>

/**
 * @brief
 *  Compares two strings.
 * @param str1
 * @param str2
 * @return int
 */
int strcmp(const char *str1, const char *str2)
{
	for (; *str1 == *str2 && *str1 != '\0'; str1++, str2++)
		;
	return *str1 - *str2;
}
/**
 * @brief
 *  Compares n characters between two strings.
 * @param str1
 * @param str2
 * @param n
 * @return int
 */
int strncmp(const char *str1, const char *str2, char n)
{
	for (; *str1 == *str2 && *str1 != '\0' && n != 0; str1++, str2++, n--)
		;
	return *str1 - *str2;
}
/**
 * @brief
 *  Length of the string.
 * @param str1
 * @return int
 */
int strlen(const char *str1)
{
	int i = 0;
	for (; str1[i] != '\0' && *str1 != '\0'; i++)
		;
	return i;
}

/*
 * @brief
 *  Concat two strings.
 * @param dest
 * @param src
 * @return char*
 */
char *strcat(char *dest, const char *src)
{
	char *aux;
	for (aux = dest; *aux != '\0'; aux++)
		;
	for (; *src != '\0';)
	{
		*(aux++) = *(src++);
	}
	*aux = '\0';
	return aux;
}
/**
 * @brief
 *  Concat first n characters from src to dest.
 * @param dest
 * @param src
 * @param n
 * @return char*
 */
char *strncat(char *dest, const char *src, int n)
{
	char *aux;
	for (aux = dest; *aux != '\0'; aux++)
		;
	for (; *src != '\0' && n != 0; n--)
	{
		*(aux++) = *(src++);
	}
	*aux = '\0';
	return aux;
}
/**
 * @brief
 *  Copy the content of src into dest.
 * @param dest
 * @param src
 * @return char*
 */
char *strcpy(char *dest, const char *src)
{
	char *aux;
	for (aux = dest; *src != '\0';)
	{
		*(aux++) = *(src++);
	}
	*aux = '\0';
	return aux;
}
/**
 * @brief
 *  Copy first n characters from src into dest.
 * @param dest
 * @param src
 * @param n
 * @return char*
 */
char *strncpy(char *dest, const char *src, int n)
{
	char *aux;
	for (aux = dest; *src != '\0' && n != 0; n--)
	{
		*(aux++) = *(src++);
	}
	*aux = '\0';
	return aux;
}
/**
 * @brief
 *  Transform integer into string.
 * @param number
 * @param str
 * @return int
 */
long itoa(long number, char *str)
{
	int digits = 1;
	for (long n = number / 10; n != 0; digits++, n /= 10)
		;

	if (digits == 1)
	{
		str[0] = '0';
		str[1] = number + '0';
		str[2] = 0;
		return digits;
	}

	str[digits] = 0;
	for (int i = digits - 1; i >= 0; i--)
	{
		str[i] = (number % 10) + '0';
		number /= 10;
	}

	return digits;
}

/**
 * @brief
 *  Get the Char
 * @return int
 */
int getChar()
{
	char c[2] = {1, 0};
	sys_read(0, (char *)&c, 2);
	return c[0];
}

static int numToBase(long value, char *buffer, int base)
{
	char *p = buffer;
	char *p1, *p2;
	int digits = 0;

	do
	{
		int remainder = value % base;
		*p++ = (remainder < 10) ? remainder + '0' : remainder + 'A' - 10;
		digits++;
	} while (value /= base);

	*p = 0;

	p1 = buffer;
	p2 = p - 1;
	while (p1 < p2)
	{
		char tmp = *p1;
		*p1 = *p2;
		*p2 = tmp;
		p1++;
		p2--;
	}

	return digits;
}
static void print(const char *fmt, va_list args)
{
	int state = 0;
	while (*fmt)
	{
		if (state == 0)
		{
			if (*fmt == '%')
			{
				state = 1;
			}
			else
			{
				putChar(*fmt);
			}
		}
		else if (state == 1)
		{
			switch (*fmt)
			{
			case 'c':
			{
				char ch = va_arg(args, int);
				putChar(ch);
				break;
			}
			case 's':
			{
				char *s = va_arg(args, char *);
				sys_write(s, white);
				break;
			}
			case 'i':
			case 'd':
			{
				char buffer[27];
				int num = va_arg(args, int);
				numToBase(num, buffer, 10);
				sys_write(buffer, white);
				break;
			}
			case 'o':
			{
				char buffer[27];
				int num = va_arg(args, int);
				numToBase(num, buffer, 8);
				sys_write(buffer, white);
				break;
			}
			case 'x':
			case 'X':
			case 'p':
			{
				char buffer[27];
				int num = va_arg(args, int);
				putChar('0');
				putChar('x');
				numToBase(num, buffer, 8);
				sys_write(buffer, white);
				break;
			}
			case '%':
			{
				putChar('%');
				break;
			}
			case 'l':
			{
				char buffer[27];
				long num = va_arg(args, long);
				numToBase(num, buffer, 8);
				sys_write(buffer, white);
				break;
			}
			}
			state = 0;
		}
		fmt++;
	}
}

void printf(const char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	print(fmt, args);
	va_end(args);
}

/**
 * @brief
 *  Print char.
 * @param c
 * @return int
 */
int putChar(int c)
{
	return putColorChar(c, white);
}

int putColorChar(int car, Color c)
{
	char str[2];
	str[0] = car;
	str[1] = '\0';
	sys_write((char *)str, c);
	return str[0];
}

void putStringColor(char *str, Color c)
{
	while (*str)
	{
		putColorChar(*str, c);
		str++;
	}
}

void puts(char *str)
{
	while (*str)
	{
		putChar(*str);
		str++;
	}
	putChar('\n');
}

void printStringAt(int x, int y, char *str, Color c)
{
	sys_writeAt((short)x, (short)y, str, c);
}

//length problem
static void scan(const char *fmt, va_list args, int length)
{
	int state = 0;
	while (*fmt)
	{
		if (state == 0)
		{
			if (*fmt == '%')
			{
				state = 1;
			}
			else
			{
				putChar(*fmt);
			}
		}
		else if (state == 1)
		{
			switch (*fmt)
			{
			case 'c':
			{
				*(va_arg(args, char *)) = getChar();
				break;
			}
			case 's':
			{

				sys_read(0, va_arg(args, char *), length);
				break;
			}
			case 'i':
			case 'd':
			case 'o':
			case 'x':
			case 'X':
			case 'p':
			{
				//warning type
				sys_read(0, va_arg(args, int *), length); 
				break;
			}
			}
			state = 0;
		}
		fmt++;
	}
}

/**
 * @brief
 *
 * @param fmt
 * @param ...
 * @return int
 */
void scanf(int lenght, const char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	scan(fmt, args, lenght);
	va_end(args);
}

void clear() {
	sys_clearScreen();
}
