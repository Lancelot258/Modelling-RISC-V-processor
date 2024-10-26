#include<iostream>
#include<string>
#include<vector>
#include<bitset>
#include<fstream>

using namespace std;

#define MemSize 1000 // memory size, in reality, the memory size should be 2^32, but for this lab, for the space resaon, we keep it as this large number, but the memory is still 32-bit addressable.


struct IFStruct {
    bitset<32>  PC;
    bool        nop = false;
};

struct IDStruct {
    bitset<32>  Instr;
    bool        nop = true;
	string ins_str;
	char type_ins;
	unordered_map<string , string > ins_map;
};

struct EXStruct {
    bitset<32>  Read_data1;
    bitset<32>  Read_data2;
	bitset<32>  passingresult;
    bitset<16>  Imm;
    bitset<5>   Rs;
    bitset<5>   Rt;
    bitset<5>   Wrt_reg_addr;
    bool        is_I_type;
    bool        rd_mem;
    bool        wrt_mem;
    bool        alu_op;     //1 for addu, lw, sw, 0 for subu
    bool        wrt_enable;
    bool        nop = true;
	string ins_str;
	char type_ins;
	unordered_map<string , string > ins_map;
};

struct MEMStruct {
    bitset<32>  ALUresult;
    bitset<32>  Store_data;
	bitset<32>  passingresult;
    bitset<5>   Rs;
    bitset<5>   Rt;
    bitset<5>   Wrt_reg_addr;
    bool        rd_mem;
    bool        wrt_mem;
    bool        wrt_enable;
    bool        nop = true;
	string ins_str;
	char type_ins;
	unordered_map<string , string > ins_map;
};

struct WBStruct {
    bitset<32>  Wrt_data;
    bitset<5>   Rs;
    bitset<5>   Rt;
    bitset<5>   Wrt_reg_addr;
    bool        wrt_enable;
    bool        nop = true;
	string ins_str;
	char type_ins;
	unordered_map<string , string > ins_map;
};

struct stateStruct {
    IFStruct    IF;
    IDStruct    ID;
    EXStruct    EX;
    MEMStruct   MEM;
    WBStruct    WB;
};
