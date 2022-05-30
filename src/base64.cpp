#include <tomcrypt.h>

unsigned char* b64_decode(unsigned char* input){
	unsigned long input_length = strlen((char*)input);
	unsigned long b64_length = input_length / 4 * 3 + 1;
	if(input[input_length - 2] == '='){
		b64_length -= 2;
	}else if(input[input_length - 1] == '='){
		b64_length--;
	}
	unsigned char* output = new unsigned char[b64_length];
	memset(output, 0, b64_length);
	base64_decode(input, input_length, output, &b64_length);
	return output;
}
unsigned char* b64_encode(unsigned char* input, unsigned long size){
	unsigned long b64_length = (size - (size % 3)) / 3 * 4 + (size % 3 > 0 ? 4 : 0) + 1;
	unsigned char* output = new unsigned char[b64_length];
	memset(output, 0, b64_length);
	base64_encode(input, size, output, &b64_length);
	return output;
}