class FiveStageCore : public Core{
	public:
		//FiveStageCore(string ioDir, InsMem &imem, DataMem &dmem): Core(ioDir + "\\FS_", imem, dmem), opFilePath(ioDir + "\\StateResult_FS.txt") {}
		//Different dir tag for MacOS
		FiveStageCore(string ioDir, InsMem *imem, DataMem *dmem): Core(ioDir + "/FS_", imem, dmem), opFilePath(ioDir + "/StateResult_FS.txt") {}

		void step() {
			/* --------------------- WB stage --------------------- */
			//Update the values of registers in this stage.
			if(state.WB.nop == false) {
				if(state.WB.wrt_enable)
				myRF.writeRF(state.WB.Wrt_reg_addr,state.WB.Wrt_data);

			}else {

			}

			/* --------------------- MEM stage -------------------- */
			if(state.MEM.nop == false) {
				if(state.MEM.rd_mem )
					state.MEM.passingresult = ext_dmem->readDataMem(state.MEM.passingresult);
				if(state.MEM.wrt_mem)
					ext_dmem->writeDataMem(state.MEM.passingresult,state.MEM.Store_data);

				state.MEM.ALUresult = state.MEM.passingresult;

				nextState.WB.Wrt_data = state.MEM.passingresult;
				nextState.WB.Rs = state.MEM.Rs;
				nextState.WB.Rt = state.MEM.Rt;
				nextState.WB.Wrt_reg_addr = state.MEM.Wrt_reg_addr;
				nextState.WB.wrt_enable = state.MEM.wrt_enable;
				nextState.WB.ins_map = state.MEM.ins_map;
				nextState.WB.type_ins = state.MEM.type_ins;
				nextState.WB.ins_str = state.MEM.ins_str;
				nextState.WB.nop = false;
			}
			else {
				nextState.WB.nop = true;
			}

			/* --------------------- EX stage --------------------- */
			//Convert the 32 bits into an instruction. (Big endian)
			if(state.EX.nop == false) {
				excution(state.EX.ins_map,state.EX.type_ins);

				nextState.MEM.ins_map = state.EX.ins_map;
				nextState.MEM.type_ins = state.EX.type_ins;
				nextState.MEM.ins_str = state.EX.ins_str;
				nextState.MEM.wrt_mem = state.EX.wrt_mem;
				nextState.MEM.wrt_enable = state.EX.wrt_enable;
				nextState.MEM.rd_mem = state.EX.rd_mem;
				nextState.MEM.Rs = state.EX.Rs;
				nextState.MEM.Rt = state.EX.Rt;
				nextState.MEM.Wrt_reg_addr = state.EX.Wrt_reg_addr;
				//if(state.EX.type_ins == 'S') nextState.MEM.Store_data = state.EX.passingresult;
				//else nextState.MEM.ALUresult = state.EX.passingresult;
				// if(state.EX.type_ins == 'S')
				// 	cout<<"debugging"<<endl;
				nextState.MEM.passingresult = state.EX.passingresult;
				nextState.MEM.nop = false;
			}else {
				nextState.MEM.nop = true;
			}

			/* --------------------- ID stage --------------------- */
			//Convert the 32 bits into an instruction. (Big endian)
			if(state.ID.nop == false) {
				state.ID.ins_str = state.ID.Instr.to_string();
				state.ID.type_ins = insType(state.ID.ins_str);
				if(state.ID.type_ins != 'H' && state.ID.type_ins != 'U')
				state.ID.ins_map = binary2Ins(state.ID.ins_str, state.ID.type_ins);

				nextState.EX.ins_str = state.ID.ins_str;
				nextState.EX.type_ins = state.ID.type_ins;
				nextState.EX.ins_map = state.ID.ins_map;
				nextState.EX.nop = false;
				if(state.ID.type_ins == 'H') {nextState.IF.nop = true;}
			}
			else {
				nextState.EX.nop = true;
			}
			
			/* --------------------- IF stage --------------------- */
			//Read 4 lines of the IMEM file.
			if(state.IF.nop == false) {
				nextState.ID.Instr = ext_imem->readInstr(state.IF.PC);
				ins_num++;
				nextState.IF.PC = bitset_add(bitset<32>(4), state.IF.PC);

				nextState.ID.nop = false;
			}
			else {
				if(state.WB.nop == true && cycle != 0) {
					cout<< "Five Stage Core Performance Metrics" <<endl;
					cout<< "Number of cycles taken: " <<cycle  <<endl;
					cout<< "Cycles per instruction: " << (double)(cycle)/(ins_num-1) <<endl;
					cout << "Instructions per cycle: " << (double)(ins_num-1)/(cycle) <<endl;
				}
				nextState.ID.nop = true;
			}



			//halted = true;
			if (state.IF.nop && state.ID.nop && state.EX.nop && state.MEM.nop && state.WB.nop)
				halted = true;

            myRF.outputRF(cycle-1); // dump RF
			printState(state, cycle); //print states after executing cycle 0, cycle 1, cycle 2 ...

			state = nextState; //The end of the cycle and updates the current state with the values calculated in this cycle
			cycle++;
		}

		void excution (unordered_map<string,string> ins_map, char type) {

			if(type == 'I') state.EX.is_I_type = true;
			else state.EX.is_I_type = false;
			state.EX.rd_mem = false; state.EX.wrt_mem = false;

			switch(type) {
				case 'R': {
					string f7 = ins_map["func7"], f3 = ins_map["func3"];
					bitset<5> rs2_bit(ins_map["rs2"]), rs1_bit(ins_map["rs1"]), rd_bit(ins_map["rd"]);
					bitset<32> rs2_v = myRF.readRF(rs2_bit), rs1_v = myRF.readRF(rs1_bit), rd_v;
					state.EX.Rs = rs1_bit; state.EX.Rt = rs2_bit;
					state.EX.Read_data1 = rs1_v;
					state.EX.Wrt_reg_addr = rd_bit;
					state.EX.wrt_enable = true;
					if(state.MEM.Wrt_reg_addr != bitset<5>(0)  &&
					(state.MEM.Wrt_reg_addr == state.EX.Rs ||
						state.MEM.Wrt_reg_addr == state.EX.Rt )) {
						if(state.MEM.Wrt_reg_addr == state.EX.Rs) rs1_v = state.MEM.passingresult;
						else rs2_v = state.MEM.passingresult;
					}
					state.EX.Read_data2 = rs2_v;

					if(f7 == "0000000" && f3 == "000") {
						// add operation
						state.EX.alu_op = true;
						rd_v = bitset_add(rs2_v, rs1_v);
						//myRF.writeRF(rd_bit, rd_v);
					}
					else if(f7 == "0100000" && f3 == "000") {
						// sub operation
						state.EX.alu_op = false;
						rd_v = bitset_sub(rs1_v, rs2_v);
						//myRF.writeRF(rd_bit, rd_v);
					}
					else if(f7 == "0000000" && f3 == "100") {
						// xor operation
						rd_v = rs2_v^rs1_v;
						//myRF.writeRF(rd_bit, rd_v);
					}
					else if(f7 == "0000000" && f3 == "110") {
						// or operation
						rd_v = rs2_v|rs1_v;
						//myRF.writeRF(rd_bit, rd_v);
					}
					else if(f7 == "0000000" && f3 == "111") {
						// and operation
						rd_v = rs2_v&rs1_v;
						//myRF.writeRF(rd_bit, rd_v);
					}
					else cout<<"Invalid Instruction in Execution!"<<endl;

					state.EX.passingresult = rd_v;
					break;
				}
				case 'I': {
					string  f3 = ins_map["func3"], imm = ins_map["imm"] ,opcode = ins_map["opcode"];
					bitset<5>  rs1_bit(ins_map["rs1"]), rd_bit(ins_map["rd"]);
					bitset<32>  rs1_v = myRF.readRF(rs1_bit), rd_v, imm_v(static_cast<uint32_t>(binaryStringToSigned32Bit(imm)));
					state.EX.wrt_enable = true; state.EX.Wrt_reg_addr = rd_bit;
					state.EX.Rs = rs1_bit;

					if(state.MEM.Wrt_reg_addr != bitset<5>(0)  &&
					state.MEM.Wrt_reg_addr == state.EX.Rs ) {
						 rs1_v = state.MEM.passingresult;
						}
					state.EX.Read_data1 = rs1_v;

					if(f3 == "000" && opcode == "0010011") {
						//int imm_v = stoi(imm, nullptr, 2);
						rd_v = bitset_add(rs1_v, imm_v);
						//myRF.writeRF(rd_bit, rd_v);
					}
					else if(f3 == "100") {
						rd_v = rs1_v ^ imm_v;
						//myRF.writeRF(rd_bit, rd_v);
					}
					else if(f3 == "110") {
						rd_v = rs1_v | imm_v;
						//myRF.writeRF(rd_bit, rd_v);
					}
					else if(f3 == "111") {
						rd_v = rs1_v & imm_v;
						//myRF.writeRF(rd_bit, rd_v);
					}else if(f3 == "000" && opcode == "0000011") {
						//lw operation
						state.EX.rd_mem = true;
						state.EX.alu_op = true;
						bitset<32> addr = bitset_add(rs1_v, imm_v);
						state.EX.passingresult = addr;
						//rd_v = ext_dmem->readDataMem(addr);
						//myRF.writeRF(rd_bit, rd_v);
					}else cout<<"Invalid Instruction in Execution!"<<endl;

					if(!state.EX.rd_mem )
					state.EX.passingresult = rd_v;
					break;
				}
				case 'S': {
					string imm4 = ins_map["imm4"], imm11 = ins_map["imm11"], f3 = ins_map["func3"];
					bitset<5>  rs1_bit(ins_map["rs1"]), rs2_bit(ins_map["rs2"]);
					string imm = imm11+imm4;
					bitset<32>  rs1_v = myRF.readRF(rs1_bit), rs2_v = myRF.readRF(rs2_bit), imm_v(static_cast<uint32_t>(binaryStringToSigned32Bit(imm)));
					state.EX.wrt_mem = true;
					state.EX.alu_op = true;
					state.EX.wrt_enable = false;
					state.EX.Rs = rs1_bit; state.EX.Rt = rs2_bit;
					state.EX.Read_data1 = rs1_v;state.EX.Read_data2 = rs2_v;

					if(state.MEM.Wrt_reg_addr != bitset<5>(0)  &&
					(state.MEM.Wrt_reg_addr == state.EX.Rs ||
						state.MEM.Wrt_reg_addr == state.EX.Rt )) {
						if(state.MEM.Wrt_reg_addr == state.EX.Rs) rs1_v = state.MEM.passingresult;
						else rs2_v = state.MEM.passingresult;
						}

					if(f3 == "010") {
						//sw operation
						bitset<32> addr = bitset_add(rs1_v, imm_v);
						state.EX.passingresult = addr;
						nextState.MEM.Store_data = rs2_v;
						//ext_dmem->writeDataMem(addr, rs2_v);
					}
					else cout<<"Invalid Instruction in Execution!"<<endl;


					break;
				}
				case 'B': {
				state.EX.alu_op = false;
				state.EX.wrt_enable = false;
					string imm4 = ins_map["imm4"], imm10 = ins_map["imm10"], f3 = ins_map["func3"];
					bitset<5>  rs1_bit(ins_map["rs1"]), rs2_bit(ins_map["rs2"]);
					state.EX.Rs = rs1_bit; state.EX.Rt = rs2_bit;
					string imm;
					imm += imm10[0];
					imm += imm4[4];
					imm += imm10.substr(1, 6);
					imm += imm4.substr(0, 4);
					bitset<32>  rs1_v = myRF.readRF(rs1_bit), rs2_v = myRF.readRF(rs2_bit),
					imm_v(static_cast<uint32_t>(binaryStringToSigned32Bit(imm)));
					imm_v = imm_v <<1;
					state.EX.Imm = bitset<16> (imm);
					state.EX.Read_data1 = rs1_v;state.EX.Read_data2 = rs2_v;

					if(f3 == "000") {
						//operation BEQ
						if(rs1_v == rs2_v) {
							state.IF.PC = bitset_sub(state.IF.PC, bitset<32>(12));
							state.IF.PC = bitset_add(state.IF.PC, imm_v);
							state.ID.nop = true;
							state.IF.nop = true;
						}

					}
					else if(f3 == "001") {
						//Operation BNQ
						if(rs1_v != rs2_v) {
							state.IF.PC = bitset_sub(state.IF.PC, bitset<32>(12));
							state.IF.PC = bitset_add(state.IF.PC, imm_v);
							state.ID.nop = true;
							state.IF.nop = true;
						}
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
					state.EX.Wrt_reg_addr = rd_bit;
					state.EX.wrt_enable = true;
					bitset<32>  rd_v, imm_v(static_cast<uint32_t>(binaryStringToSigned32Bit(imm20)));
					imm_v = imm_v << 1;

					state.EX.passingresult = bitset_add(state.IF.PC, bitset<32>(4));
					state.IF.PC = bitset_sub(state.IF.PC, bitset<32>(12));
					state.IF.PC = bitset_add(state.IF.PC, imm_v);
					state.ID.nop = true;
					state.IF.nop = true;
					state.EX.passingresult = rd_v;
					//myRF.writeRF(rd_bit, rd_v);
				}
				case 'U': {
					break;
				}
				case 'H': {
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
		    if (printstate.is_open() && cycle-1 >=0 ) {
		    	printstate<<"------------------------------------------------"<<endl;
		        printstate<<"State after executing cycle:\t"<<cycle - 1<<endl;

		        printstate<<"IF.PC:\t"<<state.IF.PC.to_ulong()<<endl;
		        printstate<<"IF.nop:\t"<<state.IF.nop<<endl;

		        printstate<<"ID.Instr:\t"<<state.ID.Instr<<endl;
		        printstate<<"ID.nop:\t"<<state.ID.nop<<endl;

		        printstate<<"EX.Read_data1:\t"<<state.EX.Read_data1<<endl;
		        printstate<<"EX.Read_data2:\t"<<state.EX.Read_data2<<endl;
		        printstate<<"EX.Imm:\t"<<state.EX.Imm<<endl;
		        printstate<<"EX.Rs:\t"<<state.EX.Rs<<endl;
		        printstate<<"EX.Rt:\t"<<state.EX.Rt<<endl;
		        printstate<<"EX.Wrt_reg_addr:\t"<<state.EX.Wrt_reg_addr<<endl;
		        printstate<<"EX.is_I_type:\t"<<state.EX.is_I_type<<endl;
		        printstate<<"EX.rd_mem:\t"<<state.EX.rd_mem<<endl;
		        printstate<<"EX.wrt_mem:\t"<<state.EX.wrt_mem<<endl;
		        printstate<<"EX.alu_op:\t"<<state.EX.alu_op<<endl;
		        printstate<<"EX.wrt_enable:\t"<<state.EX.wrt_enable<<endl;
		        printstate<<"EX.nop:\t"<<state.EX.nop<<endl;

		        printstate<<"MEM.ALUresult:\t"<<state.MEM.ALUresult<<endl;
		        printstate<<"MEM.Store_data:\t"<<state.MEM.Store_data<<endl;
		        printstate<<"MEM.Rs:\t"<<state.MEM.Rs<<endl;
		        printstate<<"MEM.Rt:\t"<<state.MEM.Rt<<endl;
		        printstate<<"MEM.Wrt_reg_addr:\t"<<state.MEM.Wrt_reg_addr<<endl;
		        printstate<<"MEM.rd_mem:\t"<<state.MEM.rd_mem<<endl;
		        printstate<<"MEM.wrt_mem:\t"<<state.MEM.wrt_mem<<endl;
		        printstate<<"MEM.wrt_enable:\t"<<state.MEM.wrt_enable<<endl;
		        printstate<<"MEM.nop:"<<state.MEM.nop<<endl;

		        printstate<<"WB.Wrt_data:\t"<<state.WB.Wrt_data<<endl;
		        printstate<<"WB.Rs:\t"<<state.WB.Rs<<endl;
		        printstate<<"WB.Rt:\t"<<state.WB.Rt<<endl;
		        printstate<<"WB.Wrt_reg_addr:\t"<<state.WB.Wrt_reg_addr<<endl;
		        printstate<<"WB.wrt_enable:\t"<<state.WB.wrt_enable<<endl;
		        printstate<<"WB.nop:\t"<<state.WB.nop<<endl;
		    }
		    else if(cycle != 0) cout<<"Unable to open FS StateResult output file." << endl;
		    printstate.close();
		}
	private:
		string opFilePath;
};
