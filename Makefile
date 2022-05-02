##########################################     CC      ####################################################
amd64_cc:=gcc
arm64_android_clang_cc:=$(NDK_ROOT)/toolchains/llvm/prebuilt/linux-x86_64/bin/aarch64-linux-android30-clang
##########################################     INC     ####################################################
SourceCode_INC:=codes/include
##########################################     DEMO    ####################################################
# Demo_SRC:=demo/RetinexDemo.c
Demo_SRC:=demo/ThresholdSeg.c
##########################################     SRC     ####################################################
SourceCode_SRC:=codes/common/*.c
SourceCode_SRC+=codes/algo_CRef/*.c
SourceCode_SRC+=codes/algo_COpt/*.c
SourceCode_SRC+=$(Demo_SRC)
##########################################     DEF     ####################################################
#TEST BMP INPUT
BMP_INPUT_NAME?=pic1.bmp
#AMD64
Amd64_DEF:= -D_PLATFORM_AMD64_
Amd64_DEF+= -D_USE_AVX128_ISA_
Amd64_DEF+= -D_USE_AVX256_ISA_
Amd64_DEF+= -D_USE_AVX512_ISA_
Amd64_DEF+= -D'_PIC1_BMP_PATH_3072x2048x3_="data/$(BMP_INPUT_NAME)"'
Amd64_DEF+= -D'_PIC1_RESULT_BMP_PATH_BGR2GRAY="output/pic1_thresholdSeg_amd64_rgb2gray.bmp"'
Amd64_DEF+= -D'_PIC1_RESULT_BMP_PATH_THRESHOLD="output/pic1_thresholdSeg_amd64_result.bmp"'
#ARM64
Arm64_DEF:= -D_PLATFORM_ARM64_
Arm64_DEF+= -D_USE_NEON_ISA_
Arm64_DEF+= -D_USE_SVE_ISA_
Arm64_DEF+= -D'_PIC1_BMP_PATH_3072x2048x3_="data/local/tmp/$(BMP_INPUT_NAME)"'
Arm64_DEF+= -D'_PIC1_RESULT_BMP_PATH_BGR2GRAY="data/local/tmp/pic1_thresholdSeg_arm64_rgb2gray.bmp"'
Arm64_DEF+= -D'_PIC1_RESULT_BMP_PATH_THRESHOLD="data/local/tmp/pic1_thresholdSeg_arm64_result.bmp"'
##########################################     FLAG    ####################################################
#AMD64
Amd64_FLAG:= -w -O0
Amd64_FLAG+= -lm
Amd64_FLAG+= -fopenmp
Amd64_FLAG+= -mavx
#ARM64
Arm64_FLAG:= -O0
Arm64_FLAG+= -lm
Arm64_FLAG+= -fopenmp -lomp
Arm64_FLAG+= -mfloat-abi=softfp -mfpu=neon
##########################################     EXE     ####################################################
out_amd64			:=  exe_amd64
out_arm64_android	:=  exe_arm64_android
##########################################     EXE     ####################################################
build:
	make -j build_amd64
	make -j build_arm64_android
run:
	make -j run_amd64_demo
	make -j run_arm64_demo
build_amd64:
	echo '##############build_amd64##############'
	$(amd64_cc) $(SourceCode_SRC) -I$(SourceCode_INC) $(Amd64_DEF) $(Amd64_FLAG) -o output/$(out_amd64)
build_arm64_android:
	echo '##############build_arm64##############'
	$(arm64_android_clang_cc) $(SourceCode_SRC) -I$(SourceCode_INC) $(Arm64_DEF) $(Arm64_FLAG) -o output/$(out_arm64_android)
clean:
	rm -rf output/*
run_amd64_demo:
	chmod +x output/$(out_amd64)
	echo '##############run_amd64_demo##############'
	./output/$(out_amd64)
run_arm64_demo:
	adb shell rm -rf /data/local/tmp/*
	adb push output/$(out_arm64_android) 																      /data/local/tmp/
	adb push data/$(BMP_INPUT_NAME)																			  /data/local/tmp/
	adb shell chmod +x /data/local/tmp/$(out_arm64_android)
	adb push $(NDK_ROOT)/toolchains/llvm/prebuilt/linux-x86_64/lib64/clang/11.0.5/lib/linux/aarch64/libomp.so /data/local/tmp/
	echo "export LD_LIBRARY_PATH=/data/local/tmp/ \n /data/local/tmp/$(out_arm64_android)" > cmd.txt
	echo '##############run_arm64_demo##############'
	adb shell <  cmd.txt
	rm -rf cmd.txt
	adb pull data/local/tmp																					  output/
