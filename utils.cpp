int32_t binaryStringToSigned32Bit(string binaryStr) {
	if (binaryStr.size() != 12) {
		throw std::invalid_argument("The input binary string must be 12/20 bits long.");
	}
	if(binaryStr.size() == 12) {
		// conversion from 12-bit str to unsigned integer
		uint16_t value = bitset<12>(binaryStr).to_ulong();
		// check the sign bit, if it is 1
		if (value & 0x800) {
			value |= 0xF000;  // extended to 16 bits, with 1-extended
		}
		return static_cast<int32_t>(static_cast<int16_t>(value));
	}else if(binaryStr.size() == 20) {
		// conversion from 20-bit str to unsigned integer
		uint32_t value = bitset<20>(binaryStr).to_ulong();
		// if the sign bit == 1, do sign-extended
		if (binaryStr[0] == '1') {
			value |= 0xFFF00000;  // high 12 bits fill with all 1
		}
		// return sign_extended value to bitset<32>
		return static_cast<int32_t>(static_cast<int16_t>(value));
	}
}


bitset<32> bitset_add( bitset<32> a, bitset<32> b) {
	bitset<32> result;
	bool carry = false;

	// addition from LSB to MSB
	for (size_t i = 0; i < 32; ++i) {
		bool bit_a = a[i];
		bool bit_b = b[i];
		// sum = bit_a ^ bit_b ^ carry
		result[i] = bit_a ^ bit_b ^ carry;
		// carry = (bit_a & bit_b) | (bit_a & carry) | (bit_b & carry)
		carry = (bit_a & bit_b) | (bit_a & carry) | (bit_b & carry);
	}
	return result;
}

bitset<32> bitset_sub(bitset<32> a, bitset<32> b) {
	bitset<32> result;
	bool borrow = false;
	for (size_t i = 0; i < 32; ++i) {
		bool bit_a = a[i];
		bool bit_b = b[i];
		// result[i] = (bit_a - bit_b - borrow)
		result[i] = (bit_a ^ bit_b) ^ borrow;
		//borrow = (!bit_a & bit_b) | ((!bit_a | bit_b) & borrow)
		borrow = (!bit_a & bit_b) | ((bit_a ^ bit_b) & borrow);
	}
	return result;
}



unordered_map<string, string> binary2Ins( string input, char type){
    string instruction;
    string result;

	switch (type) {
		case 'R': {
			vector<string> Rformat = {"func7","rs2","rs1","func3","rd","opcode"};
			unordered_map<string,unordered_map<string, string>> Rmap;
			bool flag_R = false;
			//R instruction = "OPcode(7) + Func3(3) + Func7(7)"

			unordered_map<string,string> shortRmap;
			shortRmap["add"] = "01100110000000000";
			shortRmap["sub"] = "01100110000100000";
			shortRmap["xor"] = "01100111000000000";
			shortRmap["or"] =  "01100111100000000";
			shortRmap["and"] = "01100111110000000";

			for( auto i : shortRmap ) {
				string name = i.first;   // name = "add"
				string value = i.second;  // value = "01100110000100000"
				for(int i = 0; i < 3; i++) {
					if(i == 0)
						Rmap[name][Rformat[5]] = value.substr(0, 7);
					if(i == 1)
						Rmap[name][Rformat[3]] = value.substr(7, 3);
					if(i == 2)
						Rmap[name][Rformat[0]] = value.substr(10, 7);
				}
			}

			for(auto i : Rmap) {
				string name = i.first;
				unordered_map<string,string> Rmap_temp = i.second;
				if(Rmap_temp[Rformat[0]] == input.substr(0, 7) &&
					Rmap_temp[Rformat[3]] == input.substr(17, 3) &&
					Rmap_temp[Rformat[5]] == input.substr(25, 7)) {
					//cout<<name<<" "<<Rmap_temp[Rformat[0]]<<endl; // -> add
					instruction = name;
					flag_R = true;
					Rmap[name][Rformat[1]] = input.substr(7, 5);
					Rmap[name][Rformat[2]] = input.substr(12, 5);
					Rmap[name][Rformat[4]] = input.substr(20, 5);
					}
			}
			if (flag_R) return Rmap[instruction];

		}
		case 'I': {
			vector<string> Iformat = {"imm","rs1","func3","rd","opcode"};
			unordered_map<string,unordered_map<string, string>> Imap;
			bool flag_I = false;
			//I instruction = "OPcode(7) + Func3(3) + Func7(7)"

			unordered_map<string,string> shortImap;
			shortImap["lw"] = "0000011000";
			//shortImap["lh"] = "0000011001";
			shortImap["addi"] = "0010011000";
			shortImap["xori"] = "0010011100";
			shortImap["ori"] =  "0010011110";
			shortImap["andi"] = "0010011111";

			for( auto i : shortImap ) {
				string name = i.first;   // name = "add"
				string value = i.second;  // value = "01100110000100000"
				for(int i = 0; i < 2; i++) {
					if(i == 0)
						Imap[name][Iformat[4]] = value.substr(0, 7);
					if(i == 1)
						Imap[name][Iformat[2]] = value.substr(7, 3);
				}
			}
			for(auto i : Imap) {
				string name = i.first;
				unordered_map<string,string> Imap_temp = i.second;
				if(Imap_temp[Iformat[4]] == input.substr(25, 7) &&
					Imap_temp[Iformat[2]] == input.substr(17, 3) ) {
					//cout<<name<<endl; // -> add
					instruction = i.first;
					flag_I = true;
					Imap[name][Iformat[0]] = input.substr(0, 12);
					Imap[name][Iformat[1]] = input.substr(12, 5);
					Imap[name][Iformat[3]] = input.substr(20, 5);
					}
			}
			if(flag_I) return Imap[instruction];
		}
		case 'S': {
			vector<string> Sformat = {"imm11","rs2","rs1","func3","imm4","opcode"};
			unordered_map<string,unordered_map<string, string>> Smap;
			bool flag_S = false;
			//I instruction = "OPcode(7) + Func3(3) + Func7(7)"
			//Imap["addi"] = "0010011000";
			unordered_map<string,string> shortSmap;
			// shortSmap["sb"] = "0100011000";
			// shortSmap["sh"] = "0100011001";
			shortSmap["sw"] = "0100011010";

			for( auto i : shortSmap ) {
				string name = i.first;   // name = "add"
				string value = i.second;  // value = "01100110000100000"
				for(int i = 0; i < 2; i++) {
					if(i == 0)
						Smap[name][Sformat[5]] = value.substr(0, 7);
					if(i == 1)
						Smap[name][Sformat[3]] = value.substr(7, 3);
				}
			}
			for(auto i : Smap) {
				string name = i.first;
				unordered_map<string,string> Smap_temp = i.second;
				if(Smap_temp[Sformat[5]] == input.substr(25, 7) &&
					Smap_temp[Sformat[3]] == input.substr(17, 3) ) {
					//cout<<name<<endl; // -> add
					instruction = i.first;
					flag_S = true;
					Smap[name][Sformat[0]] = input.substr(0, 7);
					Smap[name][Sformat[1]] = input.substr(7, 5);
					Smap[name][Sformat[2]] = input.substr(12, 5);
					Smap[name][Sformat[4]] = input.substr(20, 5);
					}
			}
			if(flag_S) return Smap[instruction];

		}
		case 'B': {
			vector<string> Bformat = {"imm10","rs2","rs1","func3","imm4","opcode"};
			unordered_map<string,unordered_map<string, string>> Bmap;
			bool flag_B = false;
			//I instruction = "OPcode(7) + Func3(3) + Func7(7)"
			//Imap["addi"] = "0010011000";

			unordered_map<string,string> shortBmap;
			shortBmap["beq"] = "1100011000";
			shortBmap["bne"] = "1100011001";


			for( auto i : shortBmap ) {
				string name = i.first;   // name = "add"
				string value = i.second;  // value = "01100110000100000"
				for(int i = 0; i < 2; i++) {
					if(i == 0)
						Bmap[name][Bformat[5]] = value.substr(0, 7);
					if(i == 1)
						Bmap[name][Bformat[3]] = value.substr(7, 3);
				}
			}

			for(auto i : Bmap) {
				string name = i.first;
				unordered_map<string,string> Bmap_temp = i.second;
				if(Bmap_temp[Bformat[5]] == input.substr(25, 7) &&
					Bmap_temp[Bformat[3]] == input.substr(17, 3) ) {
					//cout<<name<<endl; // -> add
					instruction = i.first;
					flag_B = true;
					Bmap[name][Bformat[0]] = input.substr(0, 7);
					Bmap[name][Bformat[1]] = input.substr(7, 5);
					Bmap[name][Bformat[2]] = input.substr(12, 5);
					Bmap[name][Bformat[4]] = input.substr(20, 5);
					}
			}
			if(flag_B) return Bmap[instruction];

		}
		case 'J': {
			vector<string> Jformat = {"imm20","rd","opcode"};
			unordered_map<string,unordered_map<string, string>> Jmap;
			bool flag_J = false;
			//I instruction = "OPcode(7) + Func3(3) + Func7(7)"
			//Imap["addi"] = "0010011000";

			unordered_map<string,string> shortJmap;
			shortJmap["jal"] = "1101111";


			for( auto i : shortJmap ) {
				string name = i.first;   // name = "add"
				string value = i.second;  // value = "01100110000100000"
				for(int i = 0; i < 1; i++) {
					if(i == 0)
						Jmap[name][Jformat[2]] = value.substr(0, 7);
				}
			}

			for(auto i : Jmap) {
				string name = i.first;
				unordered_map<string,string> Jmap_temp = i.second;
				if(Jmap_temp[Jformat[2]] == input.substr(25, 7) ) {
					//cout<<name<<endl; // -> add
					instruction = i.first;
					flag_J = true;
					Jmap[name][Jformat[0]] = input.substr(0, 20);
					Jmap[name][Jformat[1]] = input.substr(20, 5);
					//Jmap[name][Jformat[2]] = input.substr(25, 7);
				}
			}
			if(flag_J) return Jmap[instruction];
		}
		case 'U': {
			vector<string> Uformat = {"imm20","rd","opcode"};
			unordered_map<string,unordered_map<string, string>> Umap;
			bool flag_U = false;
			//I instruction = "OPcode(7) + Func3(3) + Func7(7)"
			//Imap["addi"] = "0010011000";

			unordered_map<string,string> shortUmap;
			//shortJmap["jal"] = "1101111";

			for( auto i : shortUmap ) {
				string name = i.first;   // name = "add"
				string value = i.second;  // value = "01100110000100000"
				for(int i = 0; i < 1; i++) {
					if(i == 0)
						Umap[name][Uformat[2]] = value.substr(0, 7);
				}
			}

			for(auto i : Umap) {
				string name = i.first;
				unordered_map<string,string> Umap_temp = i.second;
				if(Umap_temp[Uformat[2]] == input.substr(25, 7) ) {
					//cout<<name<<endl; // -> add
					instruction = i.first;
					flag_U = true;
					Umap[name][Uformat[0]] = input.substr(0, 20);
					Umap[name][Uformat[1]] = input.substr(20, 5);
					//Jmap[name][Jformat[2]] = input.substr(25, 7);
				}
			}
			if(flag_U) return Umap[instruction];

		}
		default: {
			cout<<"Invalid Instruction"<<endl;
		}
	}

}

char insType ( string  s ) {
	if(s.substr(25,7) == "0110011") {return 'R';}
	else if(s.substr(25,7) == "0010011" || s.substr(25,7) == "0000011") {return 'I';}
	else if(s.substr(25,7) == "1101111") {return 'J';}
	else if(s.substr(25,7) == "0100011") {return 'S';}
	else if(s.substr(25,7) == "1100011") {return 'B';}
	else if(s.substr(25,7) == "1111111") {return 'H';}
	else return 'U';
}
