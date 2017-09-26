#include "blake2/blake2.h"
#include "xorshift.hpp"
#include <emscripten/emscripten.h>
#include <random>


void HexToBytes(char * hex1, uint8_t * bytes) {

    int j = 0;
    std::string hex(hex1);
    for (unsigned int i = 0; i < hex.length(); i += 2) 
    {
        std::string byteString = hex.substr(i, 2);
        uint8_t byte = (uint8_t) strtol(byteString.c_str(), NULL, 16);
        bytes[j] = byte;
        j++;
    }

}

uint64_t iterations(uint8_t * bytes)
{
    uint64_t threshold = 0xffffffc000000000;
    uint64_t work;
	uint64_t output = 0;
    blake2b_state hash;
    blake2b_init (&hash, sizeof(output));
    std::xorshift1024star rng;
    
    const int range_from  = 0;
    const int range_to    = 32767;
    std::random_device                  rand_dev;
    std::mt19937                        generator(rand_dev());
    std::uniform_int_distribution<int>  distr(range_from, range_to);
    
    for(int j = 0; j < 16; j++)
        rng.s[j] = distr(generator);
    
    // more iterations means this functions needs to be called more times
    // more iterations means this function will be running longer
    // as killing webworkers will stop the execution anyway I've set a high number of iterations
    // each thread doesnt need to stay aware of others' results, so they can run
    // all the time until it finds a solution or it is killed 
    // :)
    uint64_t iteration (10000000); //10M
    while(iteration && output < threshold)
    {
		work = rng.next ();
		blake2b_update (&hash, reinterpret_cast <uint8_t *> (&work), sizeof (work));
		blake2b_update (&hash, bytes, 32);
		blake2b_final (&hash, reinterpret_cast <uint8_t *> (&output), sizeof (output));
		blake2b_init (&hash, sizeof (output));
		iteration -= 1;
    }
        
    if(output > threshold)
    {
        printf("value: 0x%016llx\n", output);
        printf("value: 0x%016llx\n", work);
        return work;
    }
    printf("value: 0x%016llx\n", work);
    return 0;
}

#ifdef __cplusplus
extern "C" {
#endif


char * EMSCRIPTEN_KEEPALIVE launchPoW(char * hex)
{
    uint8_t * bytes;
    HexToBytes(hex, bytes);
    uint64_t work = iterations(bytes);
    char str[16];
    sprintf(str, "%016llx", work);
    return str;
}

#ifdef __cplusplus
}
#endif

