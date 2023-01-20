#include <iostream>
#include <string> 
//#include "math.h"
using namespace std;

int n;
char** input;

struct op
{
    const char* literal;
    int priority = -1;
    float (*impl)(float, float) = nullptr;
};

struct opstack
{
    int i = 0;
    op* arr;
};

struct vstack
{
    int i = 0;
    float* arr;
};




bool getOp(char* a, op*);


inline bool top(op*, opstack*);
inline bool pop(op*, opstack*);
inline void push(op, opstack*);
inline bool top(float*, vstack*);
inline bool pop(float*, vstack*);
inline void push(float, vstack*);

float sum(float, float);
float dif(float, float);
float mul(float, float);
float div(float, float);
float abs(float, float);
float pass(float, float);

const int opcount = 8;
op ops[opcount];

int main(int N, char** args)
{
    n = N-1;
    input = &(args[1]);
    
    for (int j = 0; j < n; j++)
        cout << " " << input[j];
    cout <<" ="<< endl;

    ops[0].literal = "+";
    ops[0].priority = 1;
    ops[0].impl = sum;

    ops[1].literal = "-";
    ops[1].priority = 1;
    ops[1].impl = dif;

    ops[2].literal = "x";
    ops[2].priority = 2;
    ops[2].impl = mul;

    ops[3].literal = "/";
    ops[3].priority = 2;
    ops[3].impl = div;

    ops[4].literal = "{";
    ops[4].priority = -1;
    ops[4].impl = pass;

    ops[5].literal = "}";
    ops[5].priority = -2;
    ops[5].impl = nullptr;

    ops[6].literal = "[";
    ops[6].priority = -1;
    ops[6].impl = abs;

    ops[7].literal = "]";
    ops[7].priority = -2;
    ops[7].impl = nullptr;

    //alocar stacks en el stack
    opstack opstack; 
    op bruh[n];
    opstack.arr = bruh;
    vstack valstack; 
    float ultrabruh[n];
    valstack.arr = ultrabruh;

    float first;
    float second;
    float res = 0;
    
    bool foundOpen = false; 

    int i = 0;
    op* currentOP = new op();
    op* tempOP  = new op();
    while (i < n)
    {   

        if (getOp(input[i], currentOP))
        {
            switch (currentOP->priority)
            {
            case -1:
                push(*currentOP, &opstack);
                break;
            case -2:
                foundOpen = false; 
                while (!foundOpen &&
                    top(tempOP, &opstack) && 
                    tempOP->priority >= currentOP->priority)
                    {
                        //for (int j = 0; j < opstack.i; j++)
                        //    cout << "\t" << opstack.arr[j].literal;
                        //cout << endl;

                        //for (int j = 0; j < valstack.i; j++)
                        //    cout << "\t" << valstack.arr[j];
                        //cout << endl;

                        pop(tempOP, &opstack);
                        pop(&second, &valstack);

                        if (tempOP->priority == -1)
                        {
                            foundOpen = true;
                            res = tempOP->impl(second, second);
                        }
                        else
                        {
                            pop(&first, &valstack);
                            res = tempOP->impl(first, second);
                        }
                        push(res, &valstack);
                    }
                break;
            default:
                while (top(tempOP, &opstack) && //no está vacío
                        tempOP->priority > 0 && //no es un paréntesis que abre
                        tempOP->priority >= currentOP->priority) //el actual tiene menor presedencia
                    {
                        pop(tempOP, &opstack);
                        pop(&second, &valstack);
                        pop(&first, &valstack);
                        res = tempOP->impl(first, second);
                        push(res, &valstack);
                    }
                    push(*currentOP, &opstack);
                break;
            }

        }
        else
            push(atof(input[i]), &valstack);
        i++;
    }

    while (pop(tempOP, &opstack))
    {
        pop(&second, &valstack);
        pop(&first, &valstack);
        res = tempOP->impl(first, second);
        push(res, &valstack);
    }
    pop(&first, &valstack);
    cout << first << endl;
    return 0;
}


bool getOp(char* a, op* out)
{
    int j;
    for (int i = 0; i < opcount; i++)
    {
        j = 0;
        while (ops[i].literal[j] == a[j]) //mientras sean iguales
        {
            j++;
            if (ops[i].literal[j] == 0 && a[j] == 0) //si tienen el mismo largo
                {
                    out->impl = ops[i].impl;
                    out->literal = ops[i].literal;
                    out->priority = ops[i].priority;
                    return true;
                }
        }
    }
    return false;
}

inline bool top(op* a, opstack* s)
{
    if (s->i < 1) return false;
    *a = s->arr[s->i-1];
    return true;
}

inline bool pop(op* a, opstack* s)
{
    if (s->i < 1) return false;
    s->i--;
    *a = s->arr[s->i];
    return true;
}

inline void push(op a, opstack* s)
{
    s->arr[s->i] = a;
    s->i++;
}

inline bool top(float* a, vstack* s)
{
    if (s->i < 1) return false;
    *a = s->arr[s->i-1];
    return true;
}

inline bool pop(float* a, vstack* s)
{
    if (s->i < 1) return false;
    s->i--;
    *a = s->arr[s->i];
    return true;
}

inline void push(float a, vstack* s)
{
    s->arr[s->i] = a;
    s->i++;
}

float sum(float a, float b)
{
    return a + b;
}
float dif(float a, float b)
{
    return a - b;
}
float mul(float a, float b)
{
    return a * b;
}
float div(float a, float b)
{
    return a / b;
}
float pass(float a, float b)
{
    return a;
}
float abs(float a, float b)
{
    //return fabs(a);
    int *abomination;
    abomination = (int*)&a;
    *abomination &= 0x7fffffff;
    float *out = (float*)abomination;
    return *out;
}
