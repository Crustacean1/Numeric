#include <iostream>
#include <fstream>
import tester;
import numeric;

bool testTester(int a,int b){
    return a > b;
}

int main(int argc,char ** argv)
{
    if(argc != 2){
        std::cerr<<"Invalid argument count, proper syntax is: ./Numeric [test_file_name]"<<std::endl;
        return -1;
    }
    std::ofstream testFile(argv[1]);
    if(!testFile){
        std::cerr<<"Failed to open test file: "<<argv[1]<<" now closing"<<std::endl;
        return -1;
    }
    Tester<KCrypt::Numeric> tester;
    tester.addTest("comparison",testTester);
    tester.setReportStream(std::cout);
    tester.execute(testFile);
}