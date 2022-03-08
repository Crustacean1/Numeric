import numeric;
import sumator;
import geometry;

import <iostream>;

int main(int argc,char **argv)
{
    //func();
    if(argc != 3){
        return -1;
    }
    auto a = std::atoi(argv[1]);
    auto b = std::atoi(argv[2]);
    std::cout<<a<<"\t"<<b<<std::endl;
    std::cout<<sum(a,b)<<std::endl;
    Sumator s;
    s.fill(2,1,3,7,6);
    std::cout<<"sum: "<<s.sum()<<std::endl;
    std::cout<<"sum: "<<s.sum3()<<std::endl;
    Triangle tr{Point{0,0},Point{6,0},Point{0,8}};
    std::cout<<tr.getCircumference()<<std::endl;
    return 0;
}
