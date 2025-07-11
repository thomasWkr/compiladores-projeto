int a = 3;
int main()
{
    if (a == 2)
        return 2;
    while (0)
        a++;
    int b = 1;
    int c;
    b = a;
    c = a;
    a = a && b;
    return 0;
}
int b = 4;