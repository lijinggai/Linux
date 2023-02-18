#include<iostream>

namespace ds_task{
    class Task

    {
    private:
        int _x;
        int _y;
        char _op;
    public:
    Task()
    {}
        Task(const int& x,const int& y,const char&  op)   
            :_x(x)
            ,_y(y)
            ,_op(op) 
        {}
        ~Task()
        {}
        void operator()()
        {
            int tatol=0;
            switch(_op)
            {
                case '+':
                    tatol=_x+_y;
                    break;
                case '-':
                    tatol=_x-_y;
                    break;
                case '*':
                    tatol=_x*_y;
                    break;
                case '/':
                    tatol=_x/_y;
                    break;
                default:
                    break;
            }
            std::cout<<_x<<_op<<_y<<"="<<tatol<<std::endl;
        }
    };
    
    
}