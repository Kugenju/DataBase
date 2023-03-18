#include <iostream>
#include <string>
#include<iomanip>
using namespace std;

enum MetaCammandResult{
    CAMMAND_PARSE_SUCCESS,
    CAMMAND_UNRECOGNIZED
};

enum CleckSQL{
    PARSE_SQL_SUCCESS,
    UNRECOGNIZED_SQL
};

enum StatementofSQL{
    STATEMENT_INSERT,
    STATEMENT_SELECT,
    STATEMENT_DELETE,
    STATEMENT_UPDATE
};

class Statement {
    public:
        StatementofSQL type;
};
class DB{
    public:
        void start();
        void prompt_printed();
        void execute_sql(Statement &statement);
        MetaCammandResult do_meta_command(std::string command);
        CleckSQL do_sql_statement(std::string &input_line,Statement &statement);
        bool parse_SQL_statement(std::string &input_line, Statement &statement);
        bool parse_meta_command(std::string command);
};

void DB::prompt_printed(){
    // 来打印一个提示符
    std::cout<<"database > ";
}

bool DB::parse_meta_command(std::string command){
    // 对输入的简单命令进行解析
    if (command[0] == '.'){
        switch (do_meta_command(command))
        {
        case CAMMAND_PARSE_SUCCESS:
            return true;
        case CAMMAND_UNRECOGNIZED:
            std::cout<<"Unrecognized command"<<std::endl;
            return true;
        }
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

        Statement statement;
        
        if (parse_SQL_statement(input_line,statement)){
            continue;
        }

        execute_sql(statement);

    }
}

MetaCammandResult DB::do_meta_command(std::string command){
    if (command == ".exit"){
        std::cout<<"Bye!"<<std::endl;
        exit(EXIT_SUCCESS);
    }
    else{
        return CAMMAND_UNRECOGNIZED;
    }
    return CAMMAND_PARSE_SUCCESS;
}

bool DB::parse_SQL_statement(std::string &input_line, Statement &statement){
    if(do_sql_statement(input_line,statement) == PARSE_SQL_SUCCESS){
        return false;
    }   
    else{
        std::cout<<"Unrecognized statement"<<std::endl;
        return true;
    }
}

CleckSQL DB::do_sql_statement(std::string &input_line,Statement &statement){
    if(!input_line.compare(0,6,"select")){
        statement.type = STATEMENT_SELECT;
        return PARSE_SQL_SUCCESS;
    }
    else if(!input_line.compare(0,6,"insert")){
        statement.type = STATEMENT_INSERT;
        return PARSE_SQL_SUCCESS;
    }
    else if(!input_line.compare(0,6,"delete")){
        statement.type = STATEMENT_DELETE;
        return PARSE_SQL_SUCCESS;
    }
    else if(!input_line.compare(0,6,"update")){
        statement.type = STATEMENT_UPDATE;
        return PARSE_SQL_SUCCESS;
    }
    else
    {
        return UNRECOGNIZED_SQL;
    }
    
}

void DB::execute_sql(Statement &statement){
    switch (statement.type)
    {
    case STATEMENT_DELETE:
        std::cout<<"Execute select statement"<<std::endl;
        break;
    case STATEMENT_INSERT:
        std::cout<<"Execute select statement"<<std::endl;
        break;
    case STATEMENT_UPDATE:
        std::cout<<"Execute select statement"<<std::endl;
        break;
    case STATEMENT_SELECT:
        std::cout<<"Execute select statement"<<std::endl;
        break;
    }
    return;
}
           
int main(){
    DB database;
    database.start();
}