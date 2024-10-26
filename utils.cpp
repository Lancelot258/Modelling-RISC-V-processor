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
