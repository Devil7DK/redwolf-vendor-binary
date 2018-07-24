clean:
	@rm -rf out
	$(info Clean completed...)

redwolf:
	$(info Building redwolf vendor binary...)
	@mkdir -p out
	@clang++ src/redwolf.cpp src/encryption.cpp -o out/redwolftmp -lstdc++ -static-libstdc++
	$(info Packing binary with upx...)
	@rm -rf out/redwolf
	@upx -9k out/redwolftmp -oout/redwolf
	@rm out/redwolftmp
	$(info Build completed.)

rw-encrypter:
	$(info Building redwolf encrypter binary...)
	@mkdir -p out
	@clang++ src/rw-encrypter.cpp src/encryption.cpp -o out/rw-encryptertmp -lstdc++ -static-libstdc++
	$(info Packing binary with upx...)
	@rm -rf out/rw-encrypter
	@upx -9k out/rw-encryptertmp -oout/rw-encrypter
	@rm out/rw-encryptertmp
	$(info Build completed.)

all: redwolf rw-encrypter
