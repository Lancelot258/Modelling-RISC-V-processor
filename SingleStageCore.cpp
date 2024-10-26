class SingleStageCore : public Core {
	public:
	  bitset<32> readAddress;
		//Dir tag for Windows
		//SingleStageCore(string ioDir, InsMem &imem, DataMem &dmem): Core(ioDir + "\\SS_", imem, dmem), opFilePath(ioDir + "\\StateResult_SS.txt") {}

		//Different dir tag for MacOs
		SingleStageCore(string ioDir, InsMem *imem, DataMem *dmem): Core(ioDir + "/SS_", imem, dmem), opFilePath(ioDir + "/StateResult_SS.txt") {}

		void step() {
			/* Your implementation*/
			string ins_str;
			char type_ins;
			unordered_map<string , string > ins_map;
			readAddress = state.IF.PC;
			bitset<32> ins_bit;

			ins_bit = ext_imem->readInstr(readAddress);
			ins_num ++;
			ins_str = ins_bit.to_string();
			//ins_str = "00000001010000100000000010110011";
			type_ins = insType(ins_str);

			if (type_ins != 'H') {
				ins_map = binary2Ins(ins_str, type_ins);
				excution(ins_map,type_ins);
			}
			else {
				cout<< "Single Stage Core Performance Metrics" <<endl;
				cout<< "Number of cycles taken: " <<cycle +1 <<endl;
				cout<< "Cycles per instruction: " << (double)(cycle + 1)/ins_num <<endl;
				cout << "Instructions per cycle: " << double(ins_num)/(cycle+1) <<endl;
				halted = true;

			}

			if(type_ins != 'B' && type_ins != 'J')
			readAddress = bitset_add(bitset<32>(4),readAddress); // address = address + 4

			nextState.IF.PC = readAddress;

			if (state.IF.nop)
				halted = true;

			myRF.outputRF(cycle); // dump RF
			printState(nextState, cycle); //print states after executing cycle 0, cycle 1, cycle 2 ...

			state = nextState; // The end of the cycle and updates the current state with the values calculated in this cycle
			cycle++;
		}

	void excution (unordered_map<string,string> ins_map, char type) {
			switch(type) {
				case 'R': {
					string f7 = ins_map["func7"], f3 = ins_map["func3"];
					bitset<5> rs2_bit(ins_map["rs2"]), rs1_bit(ins_map["rs1"]), rd_bit(ins_map["rd"]);
					bitset<32> rs2_v = myRF.readRF(rs2_bit), rs1_v = myRF.readRF(rs1_bit), rd_v;
					if(f7 == "0000000" && f3 == "000") {
						// add operation
						rd_v = bitset_add(rs2_v, rs1_v);
						myRF.writeRF(rd_bit, rd_v);
					}
					else if(f7 == "0100000" && f3 == "000") {
						// sub operation
						rd_v = bitset_sub(rs1_v, rs2_v);
						myRF.writeRF(rd_bit, rd_v);
					}
					else if(f7 == "0000000" && f3 == "100") {
						// xor operation
						rd_v = rs2_v^rs1_v;
						myRF.writeRF(rd_bit, rd_v);
					}
					else if(f7 == "0000000" && f3 == "110") {
						// or operation
						rd_v = rs2_v|rs1_v;
						myRF.writeRF(rd_bit, rd_v);
					}
					else if(f7 == "0000000" && f3 == "111") {
						// and operation
						rd_v = rs2_v&rs1_v;
						myRF.writeRF(rd_bit, rd_v);
					}
					else cout<<"Invalid Instruction in Execution!"<<endl;
					break;
				}
				case 'I': {
					string  f3 = ins_map["func3"], imm = ins_map["imm"] ,opcode = ins_map["opcode"];
					bitset<5>  rs1_bit(ins_map["rs1"]), rd_bit(ins_map["rd"]);
					bitset<32>  rs1_v = myRF.readRF(rs1_bit), rd_v, imm_v(static_cast<uint32_t>(binaryStringToSigned32Bit(imm)));
					if(f3 == "000" && opcode == "0010011") {
						//int imm_v = stoi(imm, nullptr, 2);
						rd_v = bitset_add(rs1_v, imm_v);
						myRF.writeRF(rd_bit, rd_v);
					}
					else if(f3 == "100") {
						rd_v = rs1_v ^ imm_v;
						myRF.writeRF(rd_bit, rd_v);
					}
					else if(f3 == "110") {
						rd_v = rs1_v | imm_v;
						myRF.writeRF(rd_bit, rd_v);
					}
					else if(f3 == "111") {
						rd_v = rs1_v & imm_v;
						myRF.writeRF(rd_bit, rd_v);
					}else if(f3 == "000" && opcode == "0000011") {
						//lw operation
						bitset<32> addr = bitset_add(rs1_v, imm_v);
						rd_v = ext_dmem->readDataMem(addr);
						myRF.writeRF(rd_bit, rd_v);

					}else cout<<"Invalid Instruction in Execution!"<<endl;
					break;
				}
				case 'S': {
					string imm4 = ins_map["imm4"], imm11 = ins_map["imm11"], f3 = ins_map["func3"];
					bitset<5>  rs1_bit(ins_map["rs1"]), rs2_bit(ins_map["rs2"]);
					string imm = imm11+imm4;
					bitset<32>  rs1_v = myRF.readRF(rs1_bit), rs2_v = myRF.readRF(rs2_bit), imm_v(static_cast<uint32_t>(binaryStringToSigned32Bit(imm)));

					if(f3 == "010") {
						bitset<32> addr = bitset_add(rs1_v, imm_v);
						ext_dmem->writeDataMem(addr, rs2_v);
					}
					else cout<<"Invalid Instruction in Execution!"<<endl;
					break;
				}
				case 'B': {
					string imm4 = ins_map["imm4"], imm10 = ins_map["imm10"], f3 = ins_map["func3"];
					bitset<5>  rs1_bit(ins_map["rs1"]), rs2_bit(ins_map["rs2"]);
					string imm;
					imm += imm10[0];
					imm += imm4[4];
					imm += imm10.substr(1, 6);
					imm += imm4.substr(0, 4);
					bitset<32>  rs1_v = myRF.readRF(rs1_bit), rs2_v = myRF.readRF(rs2_bit),
					imm_v(static_cast<uint32_t>(binaryStringToSigned32Bit(imm)));
					imm_v = imm_v <<1;

					if(f3 == "000") {
						//operation BEQ
						if(rs1_v == rs2_v)
							readAddress = bitset_add(readAddress, imm_v);
						else readAddress = bitset_add(bitset<32>(4), imm_v);
					}
					else if(f3 == "001") {
						//Operation BNQ
						if(rs1_v != rs2_v)
							readAddress = bitset_add(readAddress, imm_v);
						else readAddress = bitset_add(bitset<32>(4), imm_v);
					}else cout<<"Invalid Instruction in Execution!"<<endl;
				}
				case 'J': {
					string imm20 , tmp = ins_map["imm20"];
					// tmp [ 20 | 10:1 | 11 | 19:12 ]
					imm20 += tmp[0];
					imm20 += tmp.substr(12, 8);
					imm20 += tmp[11];
					imm20 += tmp.substr(1, 10);
					bitset<5>   rd_bit(ins_map["rd"]);
					bitset<32>  rd_v, imm_v(static_cast<uint32_t>(binaryStringToSigned32Bit(imm20)));
					imm_v = imm_v << 1;

					readAddress = bitset_add(readAddress, imm_v);
					rd_v = bitset_add(readAddress,bitset<32>(4));
					myRF.writeRF(rd_bit, rd_v);
				}
				case 'U': {

				}
				case 'H': {
					//No operation for HALT
					break;
				}
				default: cout<<"Invalid Instruction in Execution!"<<endl;
			}
		}

		void printState(stateStruct state, int cycle) {
    		ofstream printstate;
			if (cycle == 0)
				printstate.open(opFilePath, std::ios_base::trunc);
			else
    			printstate.open(opFilePath, std::ios_base::app);
    		if (printstate.is_open()) {
    			printstate<<"----------------------------------------------------------------------"<<endl;
    		    printstate<<"State after executing cycle:\t"<<cycle<<endl;

    		    printstate<<"IF.PC:\t"<<state.IF.PC.to_ulong()<<endl;
    		    printstate<<"IF.nop:\t"<<state.IF.nop<<endl;

    		}
    		else cout<<"Unable to open SS StateResult output file." << endl;
    		printstate.close();
		}
	private:
		string opFilePath;
};
