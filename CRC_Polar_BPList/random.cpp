#include "random.h"
#include<iostream>
#include<fstream>
#include<string>
#include<cmath>
#include<cstdio>
#include<cstdlib>
#include<vector>

const int CRandom::A = 48271;
const unsigned long CRandom::MM = 2147483647L;
const int CRandom::Q =MM / A;
const int CRandom::R = MM % A;

// randInt returns the next random number and updates state.
// If on UNIX, call random instead.
int CRandom::RandomInt()
{
#ifdef UNIX
    return random();
#else
    int tmpState = A * ( state % Q ) - R * ( state / Q );
    if ( tmpState >= 0 )
        state = tmpState;
    else
        state = tmpState + MM;

    return state;
#endif
}

// Return a random double in 0..1.
double CRandom::RandomReal()
{
    return RandomInt() / (double)MM;
}

// Return random int in low..high
// Not best algorithm for linear congruential generators.
int CRandom::RandomInt( int low, int high )
{
    return RandomInt() % ( high - low + 1 ) + low;
    //return int(RandomReal() * (high - low + 1)) + low;	// this method should be better
}

// Return random number according to Poisson distribution.
int CRandom::Poisson( double expectedValue )
{
    double limit = -expectedValue;
    double sum = log( RandomReal() );

    int count;
    for( count = 0; sum > limit; count++ )
        sum += log( RandomReal() );

    return count;
}

// Return random number according to negative exponential distribution.
double CRandom::NegExp( double expectedValue )
{
    return - expectedValue * log(RandomReal());
}


static CRandom TheRand(17L);

int RandInt()
{
    return TheRand.RandomInt();
}

double RandD()
{
    return TheRand.RandomReal();
}

int Random01()
{
    return( (RandInt() >> 16) %2 );
}
