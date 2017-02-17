/** truc.cc - The main program for a TruPL Compiler
  * @author Dr. Robert Matthews
  * @author Andrew Stratmann
  * @version 14 November 2016
  */
#include <iostream>
#include <string>
#include "parser.h"


int main(int argc, char *argv[])
{
    if(argc <= 1)
    {
        if(argv[0])
        {
            std::cout << "Usage: " << argv[0] << " 1" << '\n';
        }
        else
        {
            std::cout << "Usage: " << "truc 1" << '\n';
        }
        
        exit(1);
    }
    
    Scanner *myScanner = new Scanner(argv[1]);
    Parser *myParser = new Parser(myScanner);
    
    bool isParseSuccessful = myParser->parse_program();
    bool isInputDone = myParser->done_with_input();

    if(isParseSuccessful && isInputDone)
    {
        //cout << "the parse succeeded" << endl;
    }
    else if(isParseSuccessful)
    {
        cout << "Valid Program with Additional Input" << endl;
    }
    else
    {
        cout << "the parse failed" << endl;
    }

    return 0;
}
