#include <iostream>
#include <stdio.h>
#include "config.h"
#include "blockchain.cpp"
#include <unistd.h>
#include <termios.h>

using namespace std;

char getch() {
    char buf = 0;
    struct termios old = {0};
    if (tcgetattr(0, &old) < 0)
            perror("tcsetattr()");
    old.c_lflag &= ~ICANON;
    old.c_lflag &= ~ECHO;
    old.c_cc[VMIN] = 1;
    old.c_cc[VTIME] = 0;
    if (tcsetattr(0, TCSANOW, &old) < 0)
            perror("tcsetattr ICANON");
    if (read(0, &buf, 1) < 0)
            perror ("read()");
    old.c_lflag |= ICANON;
    old.c_lflag |= ECHO;
    if (tcsetattr(0, TCSADRAIN, &old) < 0)
            perror ("tcsetattr ~ICANON");
    return (buf);
}

int main ()
{
    blockchain bc(POW_SIM_DIFFICULT_BITS,POW_SIM_MIN_TRX_CNT,
                  POW_SIM_MAX_TRX_CNT,POW_SIM_TRX_SIZE);
       
    while(true)
    {
        unsigned char sel;
		cout<<"----------------------------------------"<<std::endl;        
        cout<<"[Choose your action]"<<std::endl;
        cout<<"[m: mining v: verify chain s: show blocks c: show config e: exit]"<<std::endl;
		sel = getch();
        
        switch(sel)
        {
            case 'm':
                bc.mining();
                break;
            case 'v':
                bc.verify_chain();
                break;            
            case 's':
                bc.show_block_info();
                break;            
            case 'c':
                cout<<"Difficulty bits:  "<<POW_SIM_DIFFICULT_BITS<<std::endl;
                cout<<"Min transcations count:  "<<POW_SIM_MIN_TRX_CNT<<std::endl;     
                cout<<"Max transcations count:  "<<POW_SIM_MAX_TRX_CNT<<std::endl;
                cout<<"Transactions size:  "<<POW_SIM_TRX_SIZE<<std::endl;                
                break;            
            case 'e':
                return 0;
                break;            
            default:
                cout<<"You wrote down wrong selection!  "<<'\n'<<std::endl;
                break;           
        }
    }
	return 0;

}
