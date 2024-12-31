#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define SUCCESS 0
#define NOT_A_DIGIT -1
#define STRING_EMPTY 1
#define STRING_IS_NAN 2
#define NO_DECIMAL_POINT 3
#define INVAlID_DECIMAL_POINT_PLACEMENT 4
#define INVALID_BASE 5

int string_to_double(char *str, double *number, int base); // pretvara broj iz proizvoljne baze (do 36: 0-10 + 26 eng slova) u dekadski sustav
int locate_decimal_point(char *str, int *index);           // vraća poziciju na kojoj se nalazi decimalna točka
int is_digit(char digit);                                  // vraća vrijednost od 0 do 36 ukoliko je broj/slovo ili -1 ukoliko je ne dozvoljen znak
                                                           // radi jednostavnosti, funkcija ne prepozna brojeve jer ovisno o kontekstu, broj je negativan ako ispred sebe ima '-' ili ako mu u binarnom zapisu znamenka s najvećom težinskom vrijednosti ima vrijednost 1
int main(int argc, char **argv)
{
    if (argc != 3)
    {
        printf("Usage: $0 <number_to_convert_to_base10> <base_of_given_number>");
        return 0;
    }

    double converted;
    char *to_convert = argv[1];
    int base = atoi(argv[2]); // radi jednostavnosti, nećemo provjeravati je li argument 2 valjani broj

    int status = string_to_double(to_convert, &converted, base);
    if (status != SUCCESS)
        printf("NaN");

    else
        printf("%f", converted);
}

int string_to_double(char *str, double *number, int base)
{

    if (base < 2 || base > 36) // 36 = 10 znamenki + 26 slova engleske abecede
        return INVALID_BASE;

    if (str && !str[0])
        return STRING_EMPTY;

    int index;
    int status = locate_decimal_point(str, &index);

    if (status == INVAlID_DECIMAL_POINT_PLACEMENT)
        return STRING_IS_NAN;

    else if (status == NO_DECIMAL_POINT)
        index = strlen(str); // zbog načina kako pretvaramo broj, zamišljamo da ima decimalne točke i da se nalazi nakon svih znamenki

    // inače, u funkciji se provjerava i postojanje više decimalnih točki, no to se obavlja usputno sa ispravnošću ostalih znamenki

    *number = 0; // resetiramo vrijednost varijable ukoliko već sadrži neku

    for (int i = strlen(str) - 1; i >= 0; i--)
    {
        if (i == index) // ako smo na poziciji gdje se nalazi decimalna točka, preskačemo ostale korake
            continue;

        int digit = is_digit(str[i]);

        if (digit == NOT_A_DIGIT || digit >= base) // provjera ukoliko se dani znak nalazi u brojevnom sustavu
            return STRING_IS_NAN;

        if (i < index) // ukoliko se nalazimo lijevo od decimalne točke
            *number += digit * pow(base, index - i - 1);

        else if (i > index) // ukoliko se nalazimo desno od decimalne točke
            *number += digit * pow(base, index - i);
    }
    return SUCCESS;
}

int locate_decimal_point(char *str, int *index)
{
    int str_length = strlen(str);

    if (str[0] == '.' || str[str_length - 1] == '.')
        return INVAlID_DECIMAL_POINT_PLACEMENT;

    for (int i = str_length; i >= 0; i--)
    {
        if (str[i] == '.')
        {
            *index = i;
            return SUCCESS;
        }
    }
    return NO_DECIMAL_POINT;
}

int is_digit(char digit)
{
    if (isdigit(digit))
    {
        return digit - '0';
    }
    else if (isalpha(digit))
    {
        return toupper(digit) - 'A' + 10;
    }
    return NOT_A_DIGIT;
}