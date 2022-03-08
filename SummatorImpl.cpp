module sumator;

void Sumator::fill(int a,int b,int c,int d,int e)
{
    table[0] = a;
    table[1] = b;
    table[2] = c;
    table[3] = d;
    table[4] = e;
}

int Sumator::sum(){
    int sum  = 0;
    for(int i = 0;i<5;++i)
    {
        sum += table[i];
    }
    return sum;
}

int Sumator::sum3()
{
    int sum  = 0;
    for(int i = 0;i<5;++i)
    {
        if(table[i]%3 != 0){continue;}
        sum += table[i];
    }
    return sum;
}
