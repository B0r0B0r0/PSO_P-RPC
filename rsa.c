#include<stdio.h>  
#include<stdlib.h>  
#include<math.h>  
#include<string.h>  
  
long int p=17, q=59, n, t, flag, e[100], d[100], temp[100], j, m[100], en[100], i;  
char msg[100];  
  
// int prime(long int);  
// void ce();  
// long int cd(long int);  
// void encrypt();  
// void decrypt();  
 //am putea genera random cele 2 numere prime 

long int cd(long int x)  
{  
    long int k = 1;  
    while (1)  
    {  
        k = k + t;  
        if (k % x == 0)  
            return (k / x);  
    }  
}  ;

 void ce()  
{  
    int k;  
    k = 0;  
    for (i = 2; i < t; i++)  
    {  
        if (t % i == 0)  
            continue;  
        flag = prime(i);  
        if (flag == 1 && i != p && i != q)  
        {  
            e[k] = i;  
            flag = cd(e[k]);  
            if (flag > 0)  
            {  
                d[k] = flag;  
                k++;  
            }  
            if (k == 99)  
                break;  
        }  
    }  
} ; 
  
  
void Message(char *ms)
{
    memcpy(msg,ms,strlen(ms));
    for (i = 0; i < strlen(msg); i++)  
        m[i] = msg[i];  
  
    n = p * q;  
    t = (p - 1) * (q - 1);  
  
    ce();  
    // encrypt();  
    // decrypt();  
  
};
  
int prime(long int pr)  
{  
    int i;  
    if (pr == 1)  
        return 0;  
  
    for (i = 2; i <= sqrt(pr); i++)  
    {  
        if (pr % i == 0)  
            return 0;  
    }  
    return 1;  
}  ;
  

char* encrypt(char *ms)  
{  
    Message(ms);

    long int pt, ct, key = e[0], k, len;  
    i = 0;  
    len = strlen(msg);  
    while (i < len)  
    {  
        pt = m[i];  
        pt = pt - 96;  
        k = 1;  
        for (j = 0; j < key; j++)  
        {  
            k = k * pt;  
            k = k % n;  
        }  
        temp[i] = k;  
        ct = k + 96;  
        en[i] = ct;  
        i++;  
    }  
    en[i] = -1;  
    char *crypted=malloc(i);
    printf("\nTHE ENCRYPTED MESSAGE IS:\n");  
    for (i = 0; en[i] != -1; i++)  
        crypted[i]=(char)en[i];  
        return crypted;
};  
  
char* decrypt(char *ms)  
{  
    Message(ms);
    memcpy(en,ms,strlen(ms));
    long int pt, ct, key = d[0], k;  
        i = 0;  
    while (en[i] != -1)  
    {  
        ct = temp[i];  
        k = 1;  
        for (j = 0; j < key; j++)  
        {  
            k = k * ct;  
            k = k % n;  
        }  
        pt = k + 96;  
        m[i] = pt;  
        i++;  
    }  
    m[i] = -1;  
    char *decrypted=malloc(i);
    printf("\nTHE DECRYPTED MESSAGE IS:\n");  
    for (i = 0; m[i] != -1; i++)  
        decrypted[i]=(char)m[i];  
        return decrypted;
}  ;