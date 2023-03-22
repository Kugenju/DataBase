#include <iostream>
#include <string.h>
#include<iomanip>
using namespace std;

//以下定义一系列需要用到的常量或简单函数
#define USERNAME_SIZE 32
#define EMAIL_SIZE 256
#define size_of_attribute(class,attribute) sizeof(((class*)0)->attribute)
//定义一个获取类中属性偏移值的函数，主要用来辅助表中属性的定义
const uint32_t MaxPageSize = 100; //某一表中最大存储的分页个数
const uint32_t PageSize = 4096; //分页的最大大小
const uint32_t TABLE_MAX_ROWNUMBLE = MaxPageSize * (PageSize/sizeof(Row));

//以下定义一系列枚举类型以增强代码的可读性
enum MetaCammandResult{
    //标识基础命令是否解析成功
    CAMMAND_PARSE_SUCCESS,
    CAMMAND_UNRECOGNIZED
};

enum CleckSQL{
    //标识sql语句是否解析成功
    PARSE_SQL_SUCCESS,
    UNRECOGNIZED_SQL,
    PARSE_SYNTAX_ERROR
};

enum StatementofSQL{
    //标识sql语句被解析为哪一种类型
    STATMENT_CREATE_TABLE,
    STATEMENT_INSERT,
    STATEMENT_SELECT,
    STATEMENT_DELETE,
    STATEMENT_UPDATE
};

//以下定义一系列需要用到的类

class Row{
    //定义表中元组类型
    public:
        uint32_t id;
        char username[USERNAME_SIZE];
        char email[EMAIL_SIZE];
        int AttributeNum = 3;
};
class Statement {
    //定义SQL语句类型
    public:
        StatementofSQL type;
        Row row_to_insert;
};


class table
{
    //定义一个表格类，用来储存创建的表格
public:
    void* pages[PageSize]; //存储数据的分页
    uint32_t nrow; //指示行数
    table();
    ~table();
    void* get_cur();
};

class DB{
    //定义数据库类型
    public:
        void start();
        void prompt_printed();
        void execute_sql(Statement &statement,table &table);
        MetaCammandResult do_meta_command(std::string command);
        CleckSQL do_sql_statement(std::string &input_line,Statement &statement);
        bool parse_SQL_statement(std::string &input_line, Statement &statement);
        bool parse_meta_command(std::string command);
};

//以下定义各个类中的成员函数
table::table()
{//初始化一个表
    nrow = 0;
    for(int i = 0;i < PageSize;i++){
        pages[i] = NULL;
    }
}

table::~table()
{//表类型的析构函数
    for(int i = 0;i < PageSize;i++){
        free(pages[i]);
    }
}

void* table::get_cur(){
    if(this->nrow > TABLE_MAX_ROWNUMBLE){
        //判断表是否已满
        std::cout<<"Error: Can't get cur, table is full"<<std::endl;
        return NULL;
    }
    else{
        //获得指向当前最后行所在分页的指针
        return this->pages[this->nrow/(PageSize/sizeof(Row)) + 1];
    }
}

void DB::prompt_printed(){
    // 来打印一个提示符
    std::cout<<"database > ";
}

bool DB::parse_meta_command(std::string command){
    // 对输入的基础命令进行解析
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

MetaCammandResult DB::do_meta_command(std::string command){
    //执行基本操作的函数
    if (command == ".exit"){
        //成功退出数据库
        std::cout<<"Bye!"<<std::endl;
        exit(EXIT_SUCCESS);
    }
    else{
        return CAMMAND_UNRECOGNIZED;
    }
    return CAMMAND_PARSE_SUCCESS;
}

bool DB::parse_SQL_statement(std::string &input_line, Statement &statement){
    //判断SQL语句是否解析成功的函数
    switch(do_sql_statement(input_line,statement)){
        case PARSE_SQL_SUCCESS:
            return false;
        case PARSE_SYNTAX_ERROR:
            std::cout<<"Syntax Error"<<std::endl;
            return true;
        case UNRECOGNIZED_SQL:
            std::cout<<"Unrecognized statement"<<std::endl;
            return true;
    }
    return true;
}

CleckSQL DB::do_sql_statement(std::string &input_line,Statement &statement){
    //解析SQL语句的函数
    if(!input_line.compare(0,6,"select")){
        statement.type = STATEMENT_SELECT;
        return PARSE_SQL_SUCCESS;
    }
    else if(!input_line.compare(0,6,"insert")){
        statement.type = STATEMENT_INSERT;
        int received_attribute = std::sscanf(
            input_line.c_str(),"insert %d %s %s",
            &(statement.row_to_insert.id),
            statement.row_to_insert.username,
            statement.row_to_insert.email);
        //接收要插入的元素
        if(received_attribute < statement.row_to_insert.AttributeNum){
            return PARSE_SYNTAX_ERROR; //插入的属性有缺失造成语法错误
        }
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
    else if(!input_line.compare(0,12,"create table")){
        statement.type = STATMENT_CREATE_TABLE;
        return PARSE_SQL_SUCCESS;
    }
    else
    {
        return UNRECOGNIZED_SQL;
    }
    
}

void DB::execute_sql(Statement &statement,table &table){
    //实现各种SQL操作的总函数
    switch (statement.type)
    {
    case STATEMENT_DELETE:
        std::cout<<"Execute delete statement"<<std::endl;
        break;
    case STATEMENT_INSERT:
        execute_select(statement,table);
        std::cout<<"Execute insert statement"<<std::endl;
        break;
    case STATEMENT_UPDATE:
        std::cout<<"Execute delete statement"<<std::endl;
        break;
    case STATEMENT_SELECT:
        std::cout<<"Execute select statement"<<std::endl;
        break;
    case STATMENT_CREATE_TABLE:
        std::cout<<"Execute creat table statement"<<std::endl;
        break;
    }
    return;
}
           
void DB::start(){
    //运行数据库程序的虚拟机
    while (true){
        prompt_printed();
        table table;
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

        execute_sql(statement,table);

    }
}

//以下为需要用到的各种函数的定义
void InsertRows(Row row, void* demition){
    //用来实现Insert功能的函数
    memcpy((char*)demition, &(row.id),size_of_attribute(Row,id));
    memcpy((char*)demition + size_of_attribute(Row,username),&(row.username),size_of_attribute(Row,username));
    memcpy((char*)demition + size_of_attribute(Row,username) + size_of_attribute(Row,email),&(row.email),size_of_attribute(Row,email));
    //按Row内数据成员偏移量为间隔插入信息，节约内存空间
}

void execute_select(Statement &statement, table &table){
    if(table.nrow >TABLE_MAX_ROWNUMBLE){
        std::cout<<"Error Table is full"<<std::endl;
    }
    else{
        void* page = table.get_cur();
        InsertRows(statement.row_to_insert,page);
        std::cout<<"Execute insert successful"<<std::endl;
    }
}

void SelectRows(Statement statement){
    
}

//以下为主函数
int main(){
    DB database;
    database.start();
}