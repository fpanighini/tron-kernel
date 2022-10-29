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

static void printchar(char **str, int c)
{
	if (str) {
		**str = c;
		++(*str);
	}
	else putChar(c);
}

#define PAD_RIGHT 1
#define PAD_ZERO 2

static int prints(char **out, const char *string, int width, int pad)
{
	register int pc = 0, padchar = ' ';

	if (width > 0) {
		register int len = 0;
		register const char *ptr;
		for (ptr = string; *ptr; ++ptr) ++len;
		if (len >= width) width = 0;
		else width -= len;
		if (pad & PAD_ZERO) padchar = '0';
	}
	if (!(pad & PAD_RIGHT)) {
		for ( ; width > 0; --width) {
			printchar (out, padchar);
			++pc;
		}
	}
	for ( ; *string ; ++string) {
		printchar (out, *string);
		++pc;
	}
	for ( ; width > 0; --width) {
		printchar (out, padchar);
		++pc;
	}

	return pc;
}

/* the following should be enough for 32 bit int */
#define PRINT_BUF_LEN 12

static int printi(char **out, int i, int b, int sg, int width, int pad, int letbase)
{
	char print_buf[PRINT_BUF_LEN];
	register char *s;
	register int t, neg = 0, pc = 0;
	register unsigned int u = i;

	if (i == 0) {
		print_buf[0] = '0';
		print_buf[1] = '\0';
		return prints (out, print_buf, width, pad);
	}

	if (sg && b == 10 && i < 0) {
		neg = 1;
		u = -i;
	}

	s = print_buf + PRINT_BUF_LEN-1;
	*s = '\0';

	while (u) {
		t = u % b;
		if( t >= 10 )
			t += letbase - '0' - 10;
		*--s = t + '0';
		u /= b;
	}

	if (neg) {
		if( width && (pad & PAD_ZERO) ) {
			printchar (out, '-');
			++pc;
			--width;
		}
		else {
			*--s = '-';
		}
	}

	return pc + prints (out, s, width, pad);
}

static int print(char **out, int *varg)
{
	register int width, pad;
	register int pc = 0;
	register char *format = (char *)(*varg++);
	char scr[2];

	for (; *format != 0; ++format) {
		if (*format == '%') {
			++format;
			width = pad = 0;
			if (*format == '\0') break;
			if (*format == '%') goto out;
			if (*format == '-') {
				++format;
				pad = PAD_RIGHT;
			}
			while (*format == '0') {
				++format;
				pad |= PAD_ZERO;
			}
			for ( ; *format >= '0' && *format <= '9'; ++format) {
				width *= 10;
				width += *format - '0';
			}
			if( *format == 's' ) {
				register char *s = *((char **)varg++);
				pc += prints (out, s?s:"(null)", width, pad);
				continue;
			}
			if( *format == 'd' ) {
				pc += printi (out, *varg++, 10, 1, width, pad, 'a');
				continue;
			}
			if( *format == 'x' ) {
				pc += printi (out, *varg++, 16, 0, width, pad, 'a');
				continue;
			}
			if( *format == 'X' ) {
				pc += printi (out, *varg++, 16, 0, width, pad, 'A');
				continue;
			}
			if( *format == 'u' ) {
				pc += printi (out, *varg++, 10, 0, width, pad, 'a');
				continue;
			}
			if( *format == 'c' ) {
				/* char are converted to int then pushed on the stack */
				scr[0] = *varg++;
				scr[1] = '\0';
				pc += prints (out, scr, width, pad);
				continue;
			}
		}
		else {
		out:
			printchar (out, *format);
			++pc;
		}
	}
	if (out) **out = '\0';
	return pc;
}

/* assuming sizeof(void *) == sizeof(int) */

int printf(const char *format, ...)
{
	register int *varg = (int *)(&format);
	return print(0, varg);
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
    sys_write((char *) str, white);
    return str[0];
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
