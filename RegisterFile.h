
class RegisterFile
{
    public:
		string outputFile;
     	RegisterFile(string ioDir): outputFile {ioDir + "RFResult.txt"} {
			Registers.resize(32);
			Registers[0] = bitset<32> (0);
        }

        bitset<32> readRF(bitset<5> Reg_addr) {
            // Fill in
     		int rg_index = static_cast<int>(Reg_addr.to_ulong()); //convert add to index
     		return Registers[rg_index];

        }

        void writeRF(bitset<5> Reg_addr, bitset<32> Wrt_reg_data) {
            // Fill in
     		if(Reg_addr.to_ulong()  == 0) { cout<<"rigister 0 can not be modified"<<endl; exit(-1);}
     		int rg_index = static_cast<int>(Reg_addr.to_ulong());
     		Registers[rg_index] = Wrt_reg_data;
        }

		void outputRF(int cycle) {
			ofstream rfout;
			if (cycle == 0)
				rfout.open(outputFile, std::ios_base::trunc);
			else
				rfout.open(outputFile, std::ios_base::app);
			if (rfout.is_open() && cycle >= 0)
			{
				rfout<<"State of RF after executing cycle:\t"<<cycle<<endl;
				for (int j = 0; j<32; j++)
				{
					rfout << Registers[j]<<endl;
				}
			}
			else if(cycle != -1)cout<<"Unable to open RF output file."<<endl;
			rfout.close();
		}

	private:
		vector<bitset<32> >Registers;
};
