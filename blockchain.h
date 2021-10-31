#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H

#include <iostream>
#include <vector>
#include <ctime>
#include <sstream>
#include <cstdlib>
#include <math.h>
#include <string.h>
#include <string>
#include <climits>
#include <stdio.h>


// sha 
#include "picoSha2.h"
#define HASH_DIGEST_SIZE 32

// genesis
#define GENSIS_INIT_NONCE 0
#define GENSIS_PREV_HASH "Genesis"

using namespace std;

class blockchain
{
public:
    blockchain(unsigned int difficulty_bits, unsigned int min_trx_cnt , unsigned int max_trx_cnt, unsigned int trx_size);
    ~blockchain();
    
    void show_block_info();
    void mining();
    void verify_chain();

private:
    struct Block {
        string hash; 
        string prev_hash; 
        unsigned long nonce;
        vector<string> transcations;
        string timestamp;
    };
    
    struct Hash_result {
        string hash_str;
        unsigned char hash_byte_c_array[32];
    };
    
    struct Difficulty {
        int bits;
        unsigned char target[32];                
    };
    
    vector<Block> blocks;
    Difficulty difficulty;
    unsigned int _min_trx_cnt;
    unsigned int _max_trx_cnt;    
    unsigned int _trx_size;
    
    Hash_result calc_hash(string content);
    string gen_random_str(const int len) ;
    void mock_transactions(vector<string> *transcations, unsigned int factor);
    void print_block_log(Block _block);
    string get_content(string prev_hash, unsigned long nonce, string timestamp, vector<string> transcations);
    bool check_mining_success(unsigned char hash_byte_c_array[32]);
    
    /////////////////////////////////////////////
    // inline funcs
    string get_current_timestamp()
    {
        stringstream s;
        s << time(0);
        return s.str();
    }

    template <class T>
    string to_string (const T& t)
    {
        std::stringstream s;
        s << t;
        return s.str();
    }
    /////////////////////////////////////////////

};

#endif // BLOCKCHAIN_H
