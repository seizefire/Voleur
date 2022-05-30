#include <iostream>
#include <tomcrypt.h>

#include "base64.h"
#include "logger.h"

unsigned char* P = b64_decode((unsigned char*)"3N35IcqKMJLdrg5HmSYa6duURBVDNZgj7BCnwcz/ufmuTgBqQSf3cxqHNTX31BTKJWBBdfF2LxA+uLRmTXZGzw==");
unsigned char* Q = b64_decode((unsigned char*)"hEN4EgQsu/HHDcZh9Qwxg43wkL8=");
unsigned char* G = b64_decode((unsigned char*)"gYixcJeFwqXYS9td15uvi1o5Ontd6U00qjtvo7aPo4ccNB7jt5SGLEBM9RPsYPKnmC8PRBze5gm2MBgZIm4YsQ==");
unsigned char* X = b64_decode((unsigned char*)"EaOoelpYoydZvGFMZHUobAPlfSY=");
unsigned char* Y = b64_decode((unsigned char*)"RSijtNxu4sTZc50YrQLR19KX3PEsIGSrvcRYdAUKb1nWGJNY0aAdt/E5HtbMbSqIFPI3mLcOYdgxu9WyzGkRNw==");
unsigned char* seed = b64_decode((unsigned char*)"J0VzBGcedEyNe+AWgq/mC/Wf9RU=");
unsigned char* empty = (unsigned char*)"";

unsigned char* jitbit_generate_code(unsigned char* licensor){
	if(register_hash(&sha1_desc) > 0){
		log_error("Registering SHA1 algorithm failed");
		return empty;
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
	if(hash_memory(sha1_id, licensor, length, hash, &hash_length) > 0){
		log_error("Licensor hash failed");
		return empty;
	}
	if(register_prng(&sprng_desc) > 0){
		log_error("Registering PRNG failed");
		return empty;
	}
	if(sprng_add_entropy(seed, 20, &rng_state) > 0){
		log_error("Adding entropy to PRNG failed");
		return empty;
	}
	if(sprng_ready(&rng_state) > 0){
		log_error("Preparing PRNG failed");
		return empty;
	}
	if(dsa_set_pqg(P, 64, Q, 20, G, 64, &key) > 0){
		log_error("Inputting DSA variables failed");
		return empty;
	}
	if(dsa_set_key(Y, 64, PK_PUBLIC, &key) > 0){
		log_error("Inputting DSA public key failed");
		return empty;
	}
	if(dsa_set_key(X, 20, PK_PRIVATE, &key) > 0){
		log_error("Inputting DSA private key failed");
		return empty;
	}
	memset(sign, 0, sign_length);
	memset(code, 255, 42);
	if(dsa_sign_hash(hash, 20, sign, &sign_length, &rng_state, find_prng("sprng"), &key) > 0){
		log_error("Signing hash failed");
		return empty;
	}
	dsa_free(&key);
	unsigned long index = 3;
	memcpy(code, sign + sign[index] - 16, 20);
	if(sign[index] == 21 && sign[4] > 0){
		log_error("Malformed DSA sign result");
		return empty;
	}
	index += sign[index] + 2;
	memcpy(code + 20, sign + index + sign[index] - 19, 20);
	return b64_encode(code, 42);
}