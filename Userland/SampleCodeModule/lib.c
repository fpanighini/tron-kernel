#include <lib.h>
#include <syscalls.h>
#include <color.h>

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

#define PRINTF_NORMAL_STATE 0
#define PRINTF_STATE_LENGHT 1
#define PRINTF_STATE_LENGHT_SHORT 2
#define PRINTF_STATE_LENGHT_LONG 3
#define PRINTF_STATE_SPEC 4

#define PRINTF_LENGHT_DEFAULT 0
#define PRINTF_LENGHT_SHORT_SHORT 1
#define PRINTF_LENGHT_SHORT 2
#define PRINTF_LENGHT_LONG 3
#define PRINTF_LENGHT_LONG_LONG 4

static void putc(const char *str)
{
	while (*str)
	{
		putChar(*str);
		str++;
	}
}

static const char g_hex_chars[] = "0123456789ABCDEF";

static int *print_num(int *argp, int lenght, bool sign, int radix)
{
	char buffer[32];
	unsigned long long number;
	int number_sign = 1;
	int pos = 0;

	switch (lenght)
	{
	case PRINTF_LENGHT_SHORT_SHORT:
	case PRINTF_LENGHT_SHORT:
	case PRINTF_LENGHT_DEFAULT:
		if (sign)
		{
			int n = *argp;
			if (n < 0)
			{
				number = -n;
				number_sign = -1;
			}
			number = (unsigned long long)n;
		}
		else
		{
			number = *(unsigned int *)argp;
		}
		argp++;
		break;
	case PRINTF_LENGHT_LONG:
		if (sign)
		{
			long int n = *(long int *)argp;
			if (n < 0)
			{
				number = -n;
				number_sign = 1;
			}
			number = (unsigned long long)n;
		}
		else
		{
			number = *(unsigned long int *)argp;
		}
		argp += 2;
		break;
	case PRINTF_LENGHT_LONG_LONG:
		if (sign)
		{
			long long int n = *(long long int *)argp;
			if (n < 0)
			{
				number = -n;
				number_sign = 1;
			}
			number = (unsigned long long)n;
		}
		else
		{
			number = *(unsigned long long *)argp;
		}
		argp += 4;
		break;
	}
	do
	{
		int rem = number % radix;
		buffer[pos++] = g_hex_chars[rem];
	} while (number > 0);
	if (sign && number_sign < 0)
	{
		buffer[pos++] = '-';
	}
	while (--pos >= 0)
	{
		putChar(buffer[pos]);
	}
	return argp;
}

void printf(const char *fmt, ...)
{
	int *argp = (int *)&fmt;
	int state = PRINTF_NORMAL_STATE;
	int lenght = PRINTF_LENGHT_DEFAULT;
	int radix = 10;
	bool sign = false;
	argp++;
	while (*fmt)
	{
		switch (state)
		{
		case PRINTF_NORMAL_STATE:
			switch (*fmt)
			{
			case '%':
				state = PRINTF_STATE_LENGHT;
				break;
			default:
				putChar(*fmt);
				break;
			}
			break;
		case PRINTF_STATE_LENGHT:
			switch (*fmt)
			{
			case 'h':
				lenght = PRINTF_LENGHT_SHORT;
				state = PRINTF_STATE_LENGHT_SHORT;
				break;
			case 'l':
				lenght = PRINTF_LENGHT_LONG;
				state = PRINTF_STATE_LENGHT_LONG;
				break;
			default:
				goto PRINTF_STATE_SPEC_;
			}
			break;
		case PRINTF_LENGHT_SHORT:
			if (*fmt == 'h')
			{
				lenght = PRINTF_LENGHT_SHORT_SHORT;
				state = PRINTF_STATE_SPEC;
			}
			else
			{
				goto PRINTF_STATE_SPEC_;
			}
			break;
		case PRINTF_LENGHT_LONG:
			if (*fmt == 'l')
			{
				lenght = PRINTF_LENGHT_LONG_LONG;
				state = PRINTF_STATE_SPEC;
			}
			else
			{
				goto PRINTF_STATE_SPEC_;
			}
			break;
		case PRINTF_STATE_SPEC:
		PRINTF_STATE_SPEC_:
			switch (*fmt)
			{
			case 'c':
				putChar(*argp);
				argp++;
				break;
			case 's':
				puts("aca estoy");
				putc(*(char **)argp);
				argp++;
				break;
			case '%':
				putChar('%');
				argp++;
				break;
			case 'd':
			case 'i':
				radix = 10;
				sign = true;
				argp = print_num(argp, lenght, sign, radix);
				break;
			case 'u':
				radix = 10;
				sign = false;
				argp = print_num(argp, lenght, sign, radix);
				break;
			case 'x':
			case 'X':
			case 'p':
				radix = 16;
				sign = false;
				argp = print_num(argp, lenght, sign, radix);
				break;
			case 'o':
				radix = 8;
				sign = false;
				argp = print_num(argp, lenght, sign, radix);
				break;
			default: // ignore invalid
				break;
			}
			state = PRINTF_NORMAL_STATE;
			lenght = PRINTF_LENGHT_DEFAULT;
			radix = 10;
			sign = false;
		}
		fmt++;
	}
}
/**
 * @brief
 *  Print char.
 * @param c
 * @return int
 */
int putChar(int c)
{
	char str[2];
	str[0] = c;
	str[1] = '\0';
	sys_write((char *)str, white);
	return str[0];
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

/**
 * @brief
 *
 * @param fmt
 * @param ...
 * @return int
 */
int scanf(const char *fmt, ...)
{
	return 0;
}
