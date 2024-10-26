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
