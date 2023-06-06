#include <lib.h>

void *memset(void *destination, int32_t c, uint64_t length)
{
	uint8_t chr = (uint8_t)c;
	char *dst = (char *)destination;

	while (length--)
		dst[length] = chr;

	return destination;
}

void *memcpy(void *destination, const void *source, uint64_t length)
{
	/*
	 * memcpy does not support overlapping buffers, so always do it
	 * forwards. (Don't change this without adjusting memmove.)
	 *
	 * For speedy copying, optimize the common case where both pointers
	 * and the length are word-aligned, and copy word-at-a-time instead
	 * of byte-at-a-time. Otherwise, copy by bytes.
	 *
	 * The alignment logic below should be portable. We rely on
	 * the compiler to be reasonably intelligent about optimizing
	 * the divides and modulos out. Fortunately, it is.
	 */
	uint64_t i;

	if ((uint64_t)destination % sizeof(uint32_t) == 0 &&
		(uint64_t)source % sizeof(uint32_t) == 0 &&
		length % sizeof(uint32_t) == 0)
	{
		uint32_t *d = (uint32_t *)destination;
		const uint32_t *s = (const uint32_t *)source;

		for (i = 0; i < length / sizeof(uint32_t); i++)
			d[i] = s[i];
	}
	else
	{
		uint8_t *d = (uint8_t *)destination;
		const uint8_t *s = (const uint8_t *)source;

		for (i = 0; i < length; i++)
			d[i] = s[i];
	}

	return destination;
}

/**
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
		*(aux++) = *(src++);
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
		*(aux++) = *(src++);
	*aux = '\0';
	return aux;
}

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

int strcmp(char *str1, char *str2)
{
	while (*str1 && (*str1 == *str2))
	{
		str1++;
		str2++;
	}
	return *(const unsigned char *)str1 - *(const unsigned char *)str2;
}

int strlen(char *str)
{
	int i = 0;
	for (; str[i] != '\0' && *str != '\0'; i++)
		;
	return i;
}

void print(const char *fmt, va_list args)
{
	int state = 0;
	while (*fmt)
	{
		if (state == 0)
		{
			if (*fmt == '%')
				state = 1;
			else
				putChar(*fmt);
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
				sys_write(STDOUT, s, 0, WHITE);
				break;
			}
			case 'i':
			case 'd':
			{
				char buffer[27];
				int num = va_arg(args, int);
				numToBase(num, buffer, 10);
				sys_write(STDOUT, buffer, 0, WHITE);
				break;
			}
			case 'o':
			{
				char buffer[27];
				int num = va_arg(args, int);
				numToBase(num, buffer, 8);
				sys_write(STDOUT, buffer, 0, WHITE);
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
				sys_write(STDOUT, buffer, 0, WHITE);
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
				sys_write(STDOUT, buffer, 0, WHITE);
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

int putChar(int c)
{
	return putColorChar(c, WHITE);
}

int putColorChar(int car, Color c)
{
	char str[2];
	str[0] = car;
	str[1] = '\0';
	sys_write(STDOUT, (char *)str, 0, c);
	return str[0];
}

int numToBase(long value, char *buffer, int base)
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
