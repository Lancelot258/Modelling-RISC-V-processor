
class DataMem
{
    public:
		string id, opFilePath, ioDir;
        DataMem(string name, string ioDir) : id{name}, ioDir{ioDir} {
            DMem.resize(MemSize);

        	//Dir tag for Windows
			//opFilePath = ioDir + "\\" + name + "_DMEMResult.txt";

        	//different dir tag for MacOS
        	opFilePath = ioDir + "/" + name + "_DMEMResult.txt";

            ifstream dmem;
            string line;
            int i=0;

        	//Dir tag for Windows
            //dmem.open(ioDir + "\\dmem.txt");

        	//different dir tag for MacOS
        	dmem.open(ioDir + "/dmem.txt"); // different tag for mac os

            if (dmem.is_open())
            {
                while (getline(dmem,line))
                {
                	line = line.substr(0,8);  // delete '\r' at the end of each line
                    DMem[i] = bitset<8>(line);
                    i++;
                }
            }
            else cout<<"Unable to open DMEM input file.";
                dmem.close();
        }

        bitset<32> readDataMem(bitset<32> Address) {
			// read data memory
			// return bitset<32> val
        	int dmem_index = static_cast<int>(Address.to_ulong()); //convert add to index
        	bitset<32> result;
        	// Place b1 into the lowest 8 bits
        	result |= bitset<32>(DMem[dmem_index + 0].to_ulong())<<24;

        	// Place b2 into the next 8 bits (shift by 8)
        	result |= bitset<32>(DMem[dmem_index + 1].to_ulong()) << 16;

        	// Place b3 into the next 8 bits (shift by 16)
        	result |= bitset<32>(DMem[dmem_index + 2].to_ulong()) << 8;

        	// Place b4 into the highest 8 bits (shift by 24)
        	result |= bitset<32>(DMem[dmem_index + 3].to_ulong());

        	// Output the result
        	//cout << "Resulting bitset<32>: " << result << endl;
        	return result;
		}

        void writeDataMem(bitset<32> Address, bitset<32> WriteData) {
			// write into memory
        	//cout<<"int writeDataMem: "<<this<<endl;
        	int dmem_index = static_cast<int>(Address.to_ulong());

        	DMem[dmem_index] = bitset<8>(WriteData.to_ulong() & 0xFF);
        	// get 8-15 bits，shift right 8 bits
        	DMem[dmem_index + 1] = bitset<8>((WriteData.to_ulong() >> 8) & 0xFF);
        	// get 16-23 bits，shift right 16 bits
        	DMem[dmem_index + 2] = bitset<8>((WriteData.to_ulong() >> 16) & 0xFF);
        	// get 24-31 bits，shift right 24 bits
        	DMem[dmem_index + 3] = bitset<8>((WriteData.to_ulong() >> 24) & 0xFF);
        }

        void outputDataMem() {
        	//cout<<"int outputDataMem: "<<this<<endl;
            ofstream dmemout;
            dmemout.open(opFilePath, std::ios_base::trunc);
            if (dmemout.is_open()) {
                for (int j = 0; j< 1000; j++)
                {
                    dmemout << DMem[j]<<endl;
                }

            }
            else cout<<"Unable to open "<<id<<" DMEM result file." << endl;
            dmemout.close();
        }

    private:
		vector<bitset<8> > DMem;
};
