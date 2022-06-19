default: ladron.exe

ladron.exe:
	g++ -DUSE_LTM -DLTM_DESC -I /usr/local/include src/*.cpp /usr/local/lib/libtomcrypt.a /usr/local/lib/libtommath.a -o ladron.exe
