#include "jitbit.hpp"

#include <libtomcrypt/tomcrypt.h>
#include <Windows.h>

unsigned char* b64_decode(const char* input, unsigned long length){
	unsigned long input_length = strlen(input);
	unsigned long b64_length = length;
	unsigned char* output = new unsigned char[length];
	memset(output, 0, b64_length);
	base64_decode(input, input_length, output, &b64_length);
	if(length != b64_length){
		exit(0);
	}
	return output;
}

char* b64_encode(unsigned char* input, unsigned long size){
	unsigned long b64_length = (size - (size % 3)) / 3 * 4 + (size % 3 > 0 ? 4 : 0) + 1;
	char* output = new char[b64_length];
	memset(output, 0, b64_length);
	base64_encode(input, size, output, &b64_length);
	return output;
}

unsigned char* P = b64_decode("3N35IcqKMJLdrg5HmSYa6duURBVDNZgj7BCnwcz/ufmuTgBqQSf3cxqHNTX31BTKJWBBdfF2LxA+uLRmTXZGzw==", 64);
unsigned char* Q = b64_decode("hEN4EgQsu/HHDcZh9Qwxg43wkL8=", 20);
unsigned char* G = b64_decode("gYixcJeFwqXYS9td15uvi1o5Ontd6U00qjtvo7aPo4ccNB7jt5SGLEBM9RPsYPKnmC8PRBze5gm2MBgZIm4YsQ==", 64);
unsigned char* X = b64_decode("EaOoelpYoydZvGFMZHUobAPlfSY=", 20);
unsigned char* Y = b64_decode("RSijtNxu4sTZc50YrQLR19KX3PEsIGSrvcRYdAUKb1nWGJNY0aAdt/E5HtbMbSqIFPI3mLcOYdgxu9WyzGkRNw==", 64);
unsigned char* seed = b64_decode("J0VzBGcedEyNe+AWgq/mC/Wf9RU=", 20);

std::string crack_jitbit(const char* licensor){
	if(register_hash(&sha1_desc) > 0){
		return "[DSA] Registering SHA1 algorithm failed";
	}
	int sha1_id = find_hash("sha1");
	unsigned long length = strlen((char*) licensor);
	unsigned char code[42];
	unsigned char hash[20];
	unsigned char sign[64];
	unsigned long hash_length = 20;
	unsigned long sign_length = 64;
	
	dsa_key key;
	prng_state rng_state;
	if(hash_memory(sha1_id, (const unsigned char*) licensor, length, hash, &hash_length) > 0){
		return "[DSA] Failed to hash licensor";
	}
	if(register_prng(&sprng_desc) > 0){
		return "[DSA] Registering PRNG failed";
	}
	if(sprng_add_entropy(seed, 20, &rng_state) > 0){
		return "[DSA] Adding entropy to PRNG failed";
	}
	if(sprng_ready(&rng_state) > 0){
		return "[DSA] Preparing PRNG failed";
	}
	if(dsa_set_pqg(P, 64, Q, 20, G, 64, &key) > 0){
		return "[DSA] Inputting DSA variables failed";
	}
	if(dsa_set_key(Y, 64, PK_PUBLIC, &key) > 0){
		return "[DSA] Inputting DSA public key failed";
	}
	if(dsa_set_key(X, 20, PK_PRIVATE, &key) > 0){
		return "[DSA] Inputting DSA private key failed";
	}
	memset(sign, 0, sign_length);
	memset(code, 255, 42);
	if(dsa_sign_hash(hash, 20, sign, &sign_length, &rng_state, find_prng("sprng"), &key) > 0){
		return "[DSA] Signing hash failed";
	}
	dsa_free(&key);
	unsigned long index = 3;
	memcpy(code, sign + sign[index] - 16, 20);
	if(sign[index] == 21 && sign[4] > 0){
		return "[DSA] Malformed DSA sign result";
	}
	index += sign[index] + 2;
	memcpy(code + 20, sign + index + sign[index] - 19, 20);
	std::string final_key = b64_encode(code, 42);
	HKEY hkey;
	LONG result = RegOpenKeyExA(HKEY_CURRENT_USER, (LPCSTR)"Software\\Jitbit\\Macro Recorder", 0, KEY_ALL_ACCESS, &hkey);
	if(result != ERROR_SUCCESS){
		return "Registry key missing. Open and close Macro Recorder";
	}
	result = RegSetKeyValueA(hkey, NULL, (LPCSTR) "Email", REG_SZ, (LPCSTR) licensor, (strlen(licensor) + 1) * sizeof(char));
	if(result != ERROR_SUCCESS){
		return "Failed to set the licensor in registry";
	}
	result = RegSetKeyValueA(hkey, NULL, (LPCSTR) "Code", REG_SZ, (LPCSTR) final_key.c_str(), (final_key.length() + 1) * sizeof(char));
	if(result != ERROR_SUCCESS){
		return "Failed to set the serial key in registry";
	}
	RegCloseKey(hkey);
	return "";
}