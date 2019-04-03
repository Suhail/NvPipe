################################################################################
#
# Copyright 2019 NVIDIA Corporation.  All rights reserved.
#
# Please refer to the NVIDIA end user license agreement (EULA) associated
# with this source code for terms and conditions that govern your use of
# this software. Any use, reproduction, disclosure, or distribution of
# this software and related documentation outside the terms of the EULA
# is strictly prohibited.
#
################################################################################
include ../../common.mk

# Target rules
all: build
build: AppEncMEOutputInVidMem

NvEncoder.o: ../../NvCodec/NvEncoder/NvEncoder.cpp ../../NvCodec/NvEncoder/NvEncoder.h
	$(GCC) $(CCFLAGS) $(INCLUDES) -o $@ -c $<

NvEncoderCuda.o: ../../NvCodec/NvEncoder/NvEncoderCuda.cpp ../../NvCodec/NvEncoder/NvEncoderCuda.h \
                 ../../NvCodec/NvEncoder/NvEncoder.h
	$(GCC) $(CCFLAGS) $(INCLUDES) -o $@ -c $<

NvEncoderOutputInVidMemCuda.o: ../../NvCodec/NvEncoder/NvEncoderOutputInVidMemCuda.cpp \
                               ../../NvCodec/NvEncoder/NvEncoderOutputInVidMemCuda.h \
                               ../../NvCodec/NvEncoder/NvEncoder.h \
                               ../../NvCodec/NvEncoder/NvEncoderCuda.h
	$(GCC) $(CCFLAGS) $(INCLUDES) -o $@ -c $<

AppEncMEOutputInVidMem.o: AppEncMEOutputInVidMem.cpp \
                          ../../NvCodec/NvEncoder/NvEncoderOutputInVidMemCuda.h \
                          ../../NvCodec/NvEncoder/NvEncoderCuda.h \
                          ../../NvCodec/NvEncoder/NvEncoder.h ../../Utils/NvCodecUtils.h \
                          ../../Utils/NvEncoderCLIOptions.h ../../Utils/Logger.h
	$(GCC) $(CCFLAGS) $(INCLUDES) -o $@ -c $<

AppEncMEOutputInVidMem: AppEncMEOutputInVidMem.o NvEncoder.o NvEncoderCuda.o NvEncoderOutputInVidMemCuda.o
	$(GCC) $(CCFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -rf AppEncMEOutputInVidMem AppEncMEOutputInVidMem.o NvEncoder.o NvEncoderCuda.o NvEncoderOutputInVidMemCuda.o
