#include <lib.h>
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
long atoi(long number, char *str) {
    int digits = 1;
	for (int n=number/10; n != 0; digits++, n /= 10);

    if(digits == 1) {
        str[0] = '0';
        str[1] = number + '0';
        str[2] = 0;
        return digits;
    }
	
	str[digits] = 0;    
	for (int i=digits-1; i>=0; i--) {
		str[i] = (number % 10) + '0';
		number /= 10;
	}
	
	return digits;
}