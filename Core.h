class Core {
	public:
		RegisterFile myRF;
		uint32_t cycle = 0;
		uint32_t ins_num = 0;
		bool halted = false;
		string ioDir;
		struct stateStruct state, nextState;
		InsMem *ext_imem;
		DataMem *ext_dmem;

		Core(string ioDir, InsMem *imem, DataMem *dmem): myRF(ioDir), ext_imem(imem), ext_dmem(dmem){}

		// Core(string ioDir, InsMem *imem, DataMem *dmem) {
		// 	this->ioDir = ioDir;
		// 	myRF = RegisterFile(ioDir);
		// 	ext_imem = *imem;
		// 	ext_dmem = *dmem;
		// };
		//: myRF(ioDir), ioDir{ioDir}, ext_imem (imem), ext_dmem (dmem) {}
		virtual void step() {}

		virtual void printState() {}
};
