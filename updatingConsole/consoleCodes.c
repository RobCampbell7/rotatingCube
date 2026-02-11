// Source - https://stackoverflow.com/a/26423946
// Posted by David Ranieri, modified by community. See post 'Timeline' for change history
// Retrieved 2026-02-10, License - CC BY-SA 4.0

#include <stdio.h>

#define clear() printf("\033[H\033[J")
#define gotoxy(x,y) printf("\033[%d;%dH", (y), (x))

int main(void)
{
    int number;

    clear();
    printf(
        "Enter your number in the box below\n"
        "+-----------------+\n"
        "|                 |\n"
        "+-----------------+\n"
    );
    gotoxy(2, 3);
    scanf("%d", &number);
    return 0;
}

