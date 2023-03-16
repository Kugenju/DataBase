#include <iostream>
#include <string>

class DB{
    public:
        void start();
        void prompt_printed();

        bool parse_meta_command(std::string command);
};

void DB::prompt_printed(){
    // 来打印一个提示符
    std::cout<<"database > ";
}

bool DB::parse_meta_command(std::string command){
    // 对输入的简单命令进行解析
    if (command == ".exit"){
        std::cout<<"Bye!"<<std::endl;
        exit(EXIT_SUCCESS);
    }
    else{
        std::cout<<"Unrecognized command" << command << std::endl;
        return true;
    }
    return false;
}

void DB::start(){
    while (true){
        prompt_printed();
        std::string input_line;
        std::getline(std::cin,input_line);

        if (parse_meta_command(input_line))
        {
            continue;
        }
    }
}

int main(){
    DB database;
    database.start();
}
