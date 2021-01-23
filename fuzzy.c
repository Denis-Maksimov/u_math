#include <stdint.h>
#include <math.h>
#include <stdio.h>

#define FUZZY_VERY_LOW_NEGATIVE -125
#define FUZZY_LOW_NEGATIVE  -90
#define FUZZY_MEDIUM_NEGATIVE  -55
#define FUZZY_SMALL_NEGATIVE  -20
#define fNO   0
#define FUZZY_SMALL_POSITIVE   20
#define FUZZY_MEDIUM_POSITIVE   55
#define FUZZY_LOW_POSITIVE   90
#define FUZZY_VERY_LOW_POSITIVE  125

#define MIN(x,y)    (((x)<(y))?(x):(y))
#define MAX(x,y)    (((x)>(y))?(x):(y))

typedef enum {AND,OR} op_type;

uint8_t num_of_rulez =0;

struct rule
{
    int8_t fe;  //fuzzy error
    op_type op; // and/or
    int8_t fde; //fuzzy delta e
    int8_t z;   //выходной сигнал
};

struct rule rules[14];

//x-элемент А-нечёткое множество
double mu(int8_t x, int8_t A){
    return exp(-(pow(x-A,2)/(2*pow(30,2))));
}


//x-элемент А-нечёткое множество
int32_t digital_mu(int32_t x, int32_t A){

    return exp(-(pow(x-A,2)/(2*pow(30,2))));
}

void add_rule (int8_t fe, op_type op, int8_t fde, int8_t z){
    rules[num_of_rulez].fe=fe;
    rules[num_of_rulez].op=op;
    rules[num_of_rulez].fde=fde;
    rules[num_of_rulez].z=z;
    num_of_rulez++;
}

// double and_conds(double a, double b){
//     return MIN(a,b);
// }

double processRules(int8_t e, int8_t de)
{
    double summ_alpha_c = 0, summ_alpha = 0;

    //пробегаем по правилам и смотрим в какой степени соответствуетт каждому правилу
    for (uint8_t i = 0; i < num_of_rulez; i++)
    {
        double alpha=0,mue=0,mude=0;

        mue=mu(e,rules[i].fe);
        mude=mu(de,rules[i].fde);
        alpha=rules[i].op==0?MIN(mue,mude):MAX(mue,mude);
        summ_alpha_c+=(alpha*rules[i].z);
        summ_alpha+=alpha;
    }
    return summ_alpha_c/summ_alpha;
}


double process(double in){

    static double x=0;
    x-=2.0;    //охлаждение
    if(in>0){
    x+=in;  //нагреватель
    printf("on %f  ",in);
    }
    printf("%f\n",x);
    return x;
}



int main(int argc, char const *argv[])
{
    add_rule(fNO,AND,fNO,fNO);

    add_rule(FUZZY_VERY_LOW_NEGATIVE,OR,FUZZY_VERY_LOW_NEGATIVE,FUZZY_VERY_LOW_POSITIVE);
    add_rule(FUZZY_VERY_LOW_POSITIVE,OR,FUZZY_VERY_LOW_POSITIVE,FUZZY_VERY_LOW_NEGATIVE);
    add_rule(FUZZY_LOW_NEGATIVE,AND,FUZZY_SMALL_NEGATIVE,FUZZY_VERY_LOW_POSITIVE);
    add_rule(FUZZY_LOW_POSITIVE,AND,FUZZY_SMALL_POSITIVE,FUZZY_VERY_LOW_NEGATIVE);
    add_rule(FUZZY_SMALL_NEGATIVE,AND,FUZZY_SMALL_NEGATIVE,FUZZY_SMALL_POSITIVE);
    add_rule(FUZZY_SMALL_POSITIVE,AND,FUZZY_SMALL_POSITIVE,FUZZY_SMALL_NEGATIVE);

    add_rule(FUZZY_MEDIUM_NEGATIVE,AND,FUZZY_MEDIUM_POSITIVE,FUZZY_SMALL_POSITIVE);
    add_rule(FUZZY_MEDIUM_NEGATIVE,AND,FUZZY_MEDIUM_NEGATIVE,FUZZY_LOW_POSITIVE);
    add_rule(FUZZY_MEDIUM_POSITIVE,AND,FUZZY_MEDIUM_NEGATIVE,FUZZY_SMALL_NEGATIVE);
    add_rule(FUZZY_MEDIUM_POSITIVE,AND,FUZZY_MEDIUM_NEGATIVE,FUZZY_LOW_NEGATIVE);

    double e=process(1.);
    double de=0;
    double olde=e;

    // for (size_t i = 0; i < 1000; i++)
    // {
    //     olde=e;
    //     e=processRules(e , de);
    //     e=process(e);
    //     de=e-olde;
        
    // }
    printf("%f\n",exp(1.));
    

    return 0;
}

