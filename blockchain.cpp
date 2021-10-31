/**
 *  blockchain class for pow sim
 @Author : RayOShow
 @brief: 
    Mining, Verify are essential functions.
    User addr, P2P, real trx, auto mining, file save blocks are not provided.
    sha256 came from lib.    
 */
 
#include "blockchain.h"

/**
    Get random string. 
    srand must be declared outside.

    @param 
     * len : size of str.
    @return 
     * string : random string.
*/
string  blockchain::gen_random_str(const int len) {
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";

    string random_str;

    for (int i = 0; i < len; ++i) {
        random_str += alphanum[rand() % (sizeof(alphanum) - 1)];
    }

    return random_str;
}

/**
    Make fake transactions for test.

    @param 
     * transcations : transcations point for adding.
     * factor : changing factor.
    @return null
*/
void blockchain::mock_transactions(vector<string> *transcations, unsigned int factor)
{
    srand((unsigned int)time(0) + factor); 
    int trxCnt = (rand() % (_max_trx_cnt-_min_trx_cnt)) + _min_trx_cnt; // range for trx cnt between min~max
    
    for(int i =0; i < trxCnt ;i++)
    {
        transcations->push_back(gen_random_str(_trx_size));    
    }
}

/**
    Get sha256 hash data.

    @param 
     * content : hash  input
    @return 
     * Hash_result : consist of 2 elements.
                     hash_str : hash 0x string data 
                     hash_byte_c_array : 8bit *32 = 256bit , consequence hash number data.
*/
blockchain::Hash_result blockchain::calc_hash(string content)
{
    Hash_result hash_result;
    picosha2::hash256(content, hash_result.hash_byte_c_array, hash_result.hash_byte_c_array+HASH_DIGEST_SIZE);
    
    char buf[2*HASH_DIGEST_SIZE+1];
    buf[2*HASH_DIGEST_SIZE] = 0;
    for (int i = 0; i < HASH_DIGEST_SIZE; i++)
    {
        sprintf(buf+i*2, "%02x", hash_result.hash_byte_c_array[i]);
    }
     
    hash_result.hash_str = string(buf);
    
    return hash_result;
}

/**
    Check whether mining is success or not.

    @param 
     * hash_byte_c_array : 8bit *32 = 256bit , consequence hash number data.
                           must be lower than  difficulty's  target data.
                           ex) difficultyBits = 11, pow = 2^246 , target buffer = [00][00][0x20][00]...[00], 
                               if hash str under 001FF..FF, mining is ok.
    @return null
*/
bool blockchain::check_mining_success(unsigned char hash_byte_c_array[HASH_DIGEST_SIZE])
{
    
    for(int index =0 ; index < HASH_DIGEST_SIZE; index++ )
    {
        if(difficulty.target[index] > hash_byte_c_array[index])
        {
            return true;            
        }
        else  if (difficulty.target[index] < hash_byte_c_array[index])
        {
            return false; 
        }
        else 
        {
            // same... check next index 
        }
    }
    
    // must change other factor for re-calculate.
    return false;    
}

/**
    Assemble content data for hash input.

    @param 
     * prev_hash : previous hash string.
     * nonce :  variable for changing output.
     * timestamp : time info for mining.
     * transcations : all trx data in current blocks.
    @return null
*/
string blockchain::get_content(string prev_hash, unsigned long nonce, string timestamp, vector<string> transcations)
{
    string trx_total_str;
    
    for(int i =0; i < transcations.size() ;i++)
    {
        trx_total_str += transcations[i];
    }
    
    return prev_hash + to_string(nonce) + trx_total_str + timestamp;
}

/**
    Show blocks' info 

    @param Block : block structure for being showed.
    @return null
*/
void blockchain::print_block_log(Block _block)
{
    cout << "Block Info" <<'\n';
    cout << "----------------------------------------" <<'\n';
    cout <<"hash:"<< _block.hash <<'\n';    
    cout <<"prev_hash:"<< _block.prev_hash <<'\n';    
    cout <<"nonce:"<< _block.nonce <<'\n';
    cout <<"timestamp:"<< _block.timestamp <<'\n';
    
    for(int i=0; i < _block.transcations.size(); i++)
    {
        cout <<"TRX["<<i<<"]:"<<_block.transcations[i]<<"\n";
    }
    cout << "----------------------------------------" <<'\n'; 
}

/**
    Constructor. 

    @param 
    @return null
*/
blockchain::blockchain(unsigned int difficulty_bits, unsigned int min_trx_cnt , unsigned int max_trx_cnt, unsigned int trx_size)
{
    // trx count
    _min_trx_cnt = min_trx_cnt;
    _max_trx_cnt = max_trx_cnt;
    _trx_size = trx_size;
    
    //  Difficulty.
    memset(difficulty.target, 0x00, sizeof( difficulty.target));
    
    if(difficulty_bits <= 0) {
        // impossible case. mining is always permitted.
        difficulty.bits = 0;        
        memset(difficulty.target, 0xFF, sizeof( difficulty.target));
    }
    else if(difficulty_bits >= 256) {
        // impossible case. mining is always prohobited.
        difficulty.bits = 256;
    }
    else {
        
        // normal case. 
        difficulty.bits = difficulty_bits;
        
        //  calculate target bits 
        //  ex) difficultyBits = 11, pow = 2^246 , target buffer = [00][00][0x20][00]...[00], mining allowed max hash str = 001FF..FF
        unsigned int index = (difficulty_bits) / 8;
        unsigned int mod = (difficulty_bits) % 8;
        
        if(mod == 0) 
        {
            // in case of mod = 0 , index =0 ,  only difficulty_bits = 0;        
            difficulty.target[index-1] =  0x01;
        }
        else  {
            difficulty.target[index] =  0x01 << (8 - mod);
        }
    }
        
    // Genesis block
    Block genesis_block;
    genesis_block.nonce = GENSIS_INIT_NONCE;
    genesis_block.timestamp = get_current_timestamp();
    genesis_block.prev_hash = GENSIS_PREV_HASH;
    mock_transactions(&genesis_block.transcations, 1);    
    Hash_result hash_result = calc_hash(get_content(genesis_block.prev_hash, genesis_block.nonce, 
                                        genesis_block.timestamp, genesis_block.transcations));
    genesis_block.hash = hash_result.hash_str;    
    blocks.push_back(genesis_block);
        
    cout << "Make genesis block!" <<'\n';    
    print_block_log(blocks[0]);    
}

blockchain::~blockchain()
{
}

/**
    Show blocks' info in chain.  

    @param null
    @return null
*/
void blockchain::show_block_info()
{
    cout << "[Start] show block info"<<'\n' << "Total height[" << blocks.size() <<"]" << endl; 
    
    for(int i = 0; i < blocks.size() ; i++)
    {
        cout <<'\n'<< "Block height[" << i+1 <<"]" << endl;    
        print_block_log(blocks[i]); 
    }
    
    cout << "[End] show block info"<<'\n'<< endl;
}

/**
    Mine new block and add to the blockchain.  

    @param null
    @return null
*/
void blockchain::mining()
{
    bool is_mining_success = false;
    Block new_block ;
    
    cout << "[Start] mining"<<'\n' << "Total height[" << blocks.size() <<"]" << endl; 
    
    new_block.prev_hash = blocks[blocks.size()-1].hash;
    
    // make fake transactions.
    mock_transactions(&new_block.transcations, blocks.size());

    // start mining.
    while(!is_mining_success)
    {
        // Get new timestamp.
        // If all nonces are failed, get new timestamp.
        // It suppose that all nonce's case fail need over 1sec.  
        // If not , sleep func is needed until 1sec go through.
        new_block.timestamp = get_current_timestamp();
        cout << "Timestamp [" << new_block.timestamp << "]" << endl;
    
        for(unsigned long _nonce = 0; _nonce <= ULONG_MAX; _nonce++) {
            
            Hash_result hash_result = calc_hash(get_content(new_block.prev_hash, _nonce, 
                                                new_block.timestamp, new_block.transcations));
                        
            if(check_mining_success(hash_result.hash_byte_c_array)) {
                cout <<"Success!" <<'\n' << "nonce["<< _nonce <<"] hash[" << hash_result.hash_str << "]" << endl;
                is_mining_success = true;
                new_block.nonce = _nonce;
                new_block.hash = hash_result.hash_str;
                break;
            }
        }
    }
    
    blocks.push_back(new_block);
        
    cout << "New block height[" << blocks.size() <<"]" << endl;    
    print_block_log(blocks[blocks.size()-1]);   
 
    cout << "[End] mining"<<'\n'<< endl; 
}


/**
    Verify all blocks' prev and current hashes from genesis block to the last.  

    @param null
    @return null
*/
void blockchain::verify_chain()
{
    cout << "[Start] verify Chain"<<'\n' << "Total height[" << blocks.size() <<"]" <<'\n' << endl; 
    
    // Genesis block's prev hash.
    string prev_hash =GENSIS_PREV_HASH;
        
    for(int i = 0; i < blocks.size() ; i++)
    {
        cout << "Block Height["<< i+1 <<"]" << endl;
        
        cout << "Saved prev hash["<< blocks[i].prev_hash <<"]" << endl;
        cout << "Recalc prev hash["<< prev_hash <<"]" << endl;        
        
        if(blocks[i].prev_hash != prev_hash) {
            
            cout << "Failed to compare prev hash" << endl; ; 
            return;
        }
        else 
        {
            cout << "Prev hash is verified!"<< endl;
        }
        
        Hash_result hash_result = calc_hash(get_content(blocks[i].prev_hash, blocks[i].nonce, 
                                                        blocks[i].timestamp, blocks[i].transcations));
       
        cout << "Saved hash["<< blocks[i].hash <<"]" << endl;
        cout << "Recalc hash["<< hash_result.hash_str <<"]" << endl;        

       
        if(blocks[i].hash != hash_result.hash_str) {
            
            cout << "Failed to compare current hash" << endl; ; 
            return;
        }
        else 
        {
            cout << "current hash is verified!"<<'\n'<< endl;
        }

        prev_hash = hash_result.hash_str;    
         
    }
    
    cout << "[Success] verify chain"<< endl;     
}