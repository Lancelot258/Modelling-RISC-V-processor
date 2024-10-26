class InsMem
{
	public:
		string id, ioDir;
        InsMem(string name, string ioDir) {
			id = name;
			IMem.resize(MemSize);
            ifstream imem;
			string line;
			int i=0;

        	//Dir tag for Windows
			//imem.open(ioDir + "\\imem.txt");

        	// different tag for mac os
        	imem.open(ioDir + "/imem.txt");

			if (imem.is_open())
			{
				while (getline(imem,line))
				{
					line = line.substr(0,8);  // delete '\r' at the end of each line
					//reverse(line.begin(),line.end());
					IMem[i] = bitset<8>(line);
					i++;
				}
				//cout<<IMem[3]<<endl;
			}
            else cout<<"Unable to open IMEM input file."<<endl;
			imem.close();
		}

		bitset<32> readInstr(bitset<32> ReadAddress ) {
			// read instruction memory
			// return bitset<32> val

        	int PC = ReadAddress.to_ulong();

        	bitset<32> result;
        	// Place b1 into the lowest 8 bits
        	result |= bitset<32>(IMem[PC + 0].to_ulong())<<24;

        	// Place b2 into the next 8 bits (shift by 8)
        	result |= bitset<32>(IMem[PC + 1].to_ulong()) << 16;

        	// Place b3 into the next 8 bits (shift by 16)
        	result |= bitset<32>(IMem[PC + 2].to_ulong()) << 8;

        	// Place b4 into the highest 8 bits (shift by 24)
        	result |= bitset<32>(IMem[PC + 3].to_ulong());

        	// Output the result

        	//cout << "Resulting bitset<32>: " << result << endl;
        	return result;

		}

    private:
        vector<bitset<8> > IMem;
};
