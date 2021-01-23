
#include <stdio.h>
#define ACCURACY 16
#define DIGITS (1<<ACCURACY) // экспонента
#define EPS 20 // константа устанавливающая границы приближенности вычисления корня

#define EXPONENTA 2.71828182845904523536028747135266249775724709369995
#if (ACCURACY==12) 
#define fEXP 0x2B7E
#elif (ACCURACY==11) 
#define fEXP 0x15BF
#elif (ACCURACY==10) 
#define fEXP 0xADF
#elif (ACCURACY==9)
#define fEXP 0x56f
#elif (ACCURACY==8)
#define fEXP 0x2B7
#elif (ACCURACY==7)
#define fEXP 0x15B
#elif (ACCURACY==6)
#define fEXP 0xAD
#else
// #error Accuracy is XyeBoe
#endif
// typedef signed int __int32_t;
typedef signed int Fixed;

Fixed int2fix(signed int val){
        return (val*DIGITS);
}
            
float fixed2float(Fixed x){
        return ((float)x)/DIGITS;
}
Fixed f_fromFloat(float val){
        return (signed int)(val*DIGITS);
}

Fixed f_add(Fixed a,Fixed b){
        return (a+b);
}

Fixed f_sub(Fixed a,Fixed b){
        return (a-b);
}
Fixed f_mul(Fixed a,Fixed b){
        // int sign = (a&0x80000000) ^ (b&0x80000000);
        // a &= ~0x80000000;
        // b &= ~0x80000000;
        Fixed rv;
        if((!a)||(!b)) return 0;
        signed int c=a*b;
        if(c/b != a){
            // Overflow!
            // printf("OF\n");
            signed int i1 = a/DIGITS;
            signed int i2 = b/DIGITS;

            unsigned int f1 = (a&(DIGITS-1));
            unsigned int f2 = (b&(DIGITS-1));


            rv=((i1*i2)*DIGITS+(f1*f2)/DIGITS+i1*f2+i2*f1);

        }else{
            rv = (c/DIGITS);
            
        }
        
        // rv &= ~0x80000000;
        // rv |= sign;
        return rv;
}

Fixed f_div(Fixed a,Fixed b){
        if(a > (0x80000000>>ACCURACY)){
            // Overflow!

            signed int i = a/DIGITS;
            signed int f = (a&(DIGITS-1));
            return (((i*DIGITS)/b)*DIGITS+(f*DIGITS)/b);
        }else{
            return ((a*DIGITS)/b);
        }
}

Fixed f_div_i(Fixed a,signed int b){
        return f_div(a, DIGITS*b);
}
Fixed f_mul_i(Fixed a,signed int b){
        return f_mul(a, int2fix(b));
}
Fixed f_abs(Fixed k){
    return (k < 0)?(-k):(k);
}

Fixed f_sqrt(Fixed k){
        Fixed tmp=0;
        tmp = k>>1;
        signed int min = 0;
        signed int max = k;
        Fixed quick=0;
        do{
            tmp = (min+max)>>1;
            quick = f_mul(tmp,tmp);
   
            if(f_abs(k-quick) < EPS) return (tmp);

            if(quick>k){
                    max = tmp;
            }else{
                    min = tmp;
            }

        }while(1);
}


int factorial(unsigned int n){
    return (n==0)?(1):(n*factorial(n-1));
}


Fixed f_pow(Fixed x,unsigned int n){
    if(n==0) return DIGITS;
    Fixed rv=x;
    for (unsigned int i=1;i<n;i++){
        rv=f_mul(rv,x);
    }
    return rv;
}

#include <stdio.h>
float fp_pow(float x,unsigned int n){
    if(n==0) return 1;
    float rv=x;
    for (unsigned int i=1;i<n;i++){
        rv=rv*x;
    }
    return rv;
}
Fixed f_exp(Fixed x){
    Fixed rv=0;
    Fixed it=0;
    for(int i=0;i<ACCURACY;i++){
        rv+=f_div_i(f_pow(x,i),factorial(i));
        it+=DIGITS;
    }
    return rv;
}

Fixed f_exp_v2(Fixed x){
    Fixed rv=0;
    
    rv=DIGITS+f_div(x,15*DIGITS);
    Fixed rvv=rv;

    for(int i=0;i<15;i++){
        rvv=f_mul(rvv,rv);
    }
    return rvv;
}
float fp_exp_v2(float x){
    float rv=0;
    
    rv=1+x/15.;
    float rvv=rv;

    for(int i=0;i<15;i++){
        rvv=rvv*rv;
    }
    return rvv;
}

float fp_exp(float x){
    float rv=0;
    float it=0;
    for(int i=0;i<12;i++){

        rv+= (fp_pow(x,i)/((float)factorial(i)));
        it+=1.;
    }
    return rv;
}
#include <math.h>
//x-элемент А-нечёткое множество
Fixed fGauss(Fixed a, Fixed b,Fixed c,Fixed x){
    // a=f_div(a,DIGITS*2);
    Fixed up=f_mul(b-x,x-b);
    Fixed down=(f_mul(c, f_mul_i(c,2)));
    return f_mul(a, f_exp(f_div(up,down)));

    // return f_mul(a,  f_exp( f_mul(b-x,x-b)/ (f_mul(c, f_mul_i(c,2)))  ) );
}

/*********************************
 *            2
 *       (x-b)
 *     - -----   
 *       2*c*c
 *   a*e
 * 
 * 
 *********************************/
float fpGauss(float a, float b,float c,float x){
    // f_div(a,DIGITS*2);
    float up;
    float down;
    return a*exp((((x-b)*(b-x))/(c*c*2)));
}


int main(int argc, char const *argv[])
{
    Fixed f=int2fix(250);
    // f.Fixed(f.fromInt(25));
    f=f_sqrt(f);
    f=f_mul(f,f);
    printf("0x%X\n", f_fromFloat(EXPONENTA));

    
    // printf("%f\n",fixed2float(f));

    Fixed step =f_fromFloat(0.1);
    for (int i = 0; i < 50; i++)
    {
        // f=f_exp(f_mul_i(step,i));
        f=fGauss(DIGITS, DIGITS, DIGITS, f_mul(step,i*DIGITS));
        printf("%f - fixed\n",fixed2float( f));
        printf("%f - float\n",fpGauss(1., 0.1,1.,(float)i*0.1));
        // printf("%f - float\n",exp((float)i*0.1));
        // printf("%f - fp\n",fp_exp_v2(i*0.1));
    }
    
    
    // printf("%f\n",fpGauss(1,1,1,0));
    return 0;
}
