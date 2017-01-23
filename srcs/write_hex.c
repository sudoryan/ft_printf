#include "ft_printf.h"

int hex_length(intmax_t n)
{
    int i;

    i = 0;
    while (n > 0)
    {
        n /= 16;
        i++;
    }
    return (i);
}

char    *convert_hex(intmax_t n, int letters)
{
    char *base;
    char *str;
    int length;
    int i;

    i = 0;
    length = hex_length(n);
    str = (char *)malloc(sizeof(char) * length + 1);
    if (letters == 0)
        base = "0123456789abcdef";
    else
        base = "0123456789ABCDEF";
    while (n > 0)
    {
        str[i++] = *(base + (n % 16));
        n /= 16;
    }
    str[i] = '\0';
    str = ft_strrev(str, i);
    return (str);
}

char *add_zeros(char *str, int precision)
{
    int i;
    char *tmp;
    int zeros;

    i = 0;
    zeros = precision - ft_strlen(str);
    if (zeros < 0)
        zeros = 0;
    tmp = (char *)malloc(sizeof(char) * zeros + 1);
    while (i < zeros)
        tmp[i++] = '0';
    tmp[i] = '\0';
    return (ft_strjoin(tmp, str));
}

void    write_str(param *params, char *str, intmax_t n, int letters)
{
    if (params->flags->hash && n != 0)
    {
        if (letters)
            write(1, "0X", 2);
        else
            write(1, "0x", 2);
    }
    ft_putstr(str);
}

int     hex_str_length(param *params, char *str, intmax_t n)
{
    int length;

    length = ft_strlen(str);
    if (params->flags->hash && n != 0)
        length += 2;
    return (length);
}

void write_hex(param *params, va_list args, int letters)
{	
    char *tmp;
    int length;
    intmax_t n;

    remove_conflict_flags(params);
    n = va_arg(args, intmax_t);
    n = convert_unsigned(params, n);
    tmp = convert_hex(n, letters);
    if (params->flags->zero)
        params->precision = params->width;
    tmp = add_zeros(tmp, params->precision);
    length = hex_str_length(params, tmp, n);
    if (!params->flags->minus && !params->flags->zero)
        write_spaces(params, params->width - length, params->precision - length);
    if (!(n == 0 && params->precision == 0))
        write_str(params, tmp, n, letters);
    if (params->flags->minus && !params->flags->zero)
        write_spaces(params, params->width - length, params->precision - length);
}