#include <iostream>
using namespace std;

class MT19937_64
{
    private:
        unsigned long long mt [312];
        short int mti;
    public:
        MT19937_64()
            {
                mt[312]={0};
                mti = 313;
            }

        void seed(unsigned long long seed)
        {
            mt[0] = seed & 0xffffffffffffffff;
            for (int i=1;i<312;i++)
            {
                mt[i] = (6364136223846793005 * (mt[i-1] ^ (mt[i-1] >> 62)) + i) & 0xffffffffffffffff;
            }
            mti = 312;
        }
        unsigned long long int64()
        {
            if (mti >= 312){
                if (mti==313){
                    seed(5489);
                }
                for (int k=0;k<311;k++)
                {
                    unsigned long long y = (mt[k] & 0xFFFFFFFF80000000) | ( mt[k+1] & 0x7fffffff);
                    if (k< 312-156){
                        mt[k] = mt[k+156] ^ (y>>1) ^ (y &1 ? 0xB5026F5AA96619E9 : 0);
                    }else{
                        mt[k] = mt[k+156-624+sizeof(mt)/sizeof(mt[0])] ^ (y>>1) ^ (y &1 ? 0xB5026F5AA96619E9 : 0);
                    }                      
                }
                unsigned long long yy = (mt[311] & 0xFFFFFFFF80000000) | (mt[0] & 0x7fffffff);
                mt[311] = mt[155] ^ (yy>>1) ^ (yy &1 ? 0xB5026F5AA96619E9 : 0);
                mti = 0;
            }
            unsigned long long x = mt[mti];
            mti += 1;

            x ^= (x >> 29) & 0x5555555555555555;
            x ^= (x << 17) & 0x71D67FFFEDA60000;
            x ^= (x << 37) & 0xFFF7EEE000000000;
            x ^= (x >> 43);
            return x;
        }
};

string MT_random(unsigned long long private_seed)
{
    MT19937_64 first;
    first.seed(private_seed);
    MT19937_64 gen;
    gen.seed(first.int64());
    gen.int64();
    unsigned char key [4096];
    for(int i=0;i<4096;i+=8){
        unsigned long long num = gen.int64();
        for(int j=0;j<8;j++){
            key[i+j] = (num >> 64-(j+1)*8) & 0xff;
        }    
    }
    string s_key; 
    for (int i=0;i<4096;i++){
        s_key += key[i];
    }
    return s_key;
}