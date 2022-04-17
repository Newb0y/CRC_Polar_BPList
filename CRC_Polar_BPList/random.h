
#ifndef __RANDOM
#define __RANDOM

#ifdef UNIX
    extern "C" void srandom( unsigned );
    extern "C" long random( );
#endif

class CRandom{

private:
    unsigned long state;

    static const int A;
    static const unsigned long MM;
    static const int Q;
    static const int R;

public:
    CRandom( unsigned long initVal = 1L ) : state (initVal)
    {
#ifdef UNIX
    srandom( initVal );
#endif
    }

    // Uniform distributions
    int RandomInt( );              // 1 to 2^31 - 1
    double RandomReal( );          // >0.0 to <1.0
    int RandomInt( int low, int high );

    // Non-uniform distribution
    int Poisson( double expectedVal );
    double NegExp( double expectedVal );
};

int RandInt();
double RandD();
int Random01();
#endif
