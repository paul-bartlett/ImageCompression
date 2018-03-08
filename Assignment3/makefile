# A provided makefile for CS4481 assignment03
 
# The makefile format
#target-name: target dependencies
#{-tab-}actions

#==================================================
# MACRO definitions
CFLAG = -std=c99 -Wall -lm
CC = gcc

#==================================================
# All Targets

all All ALL: lz77_encoding lz77_decoding compare_pgm_images

#--------------------------------------------------
#Q3a: Executable lz77_encoding depends on 
#the source file lz77_encoding.c and 
#the object files
#  lz77_encoding.o
#  lz77_encoding_function.o
#  libpnm.o
Q3a lz77_encoding: lz77_encoding.c \
                      lz77_encoding_function.o \
                      libpnm.o
	$(CC) $(CFLAG) -o lz77_encoding \
                  lz77_encoding.c \
                  lz77_encoding_function.o \
                  libpnm.o


#--------------------------------------------------
#Q3b: Executable lz77_decoding depends on 
#the source file lz77_decoding.c and 
#the object files
#  lz77_decoding.o
#  lz77_decoding_function.o
#  libpnm.o
Q3b lz77_decoding: lz77_decoding.c \
                      lz77_decoding_function.o \
                      libpnm.o
	$(CC) $(CFLAG) -o lz77_decoding \
                  lz77_decoding.c \
                  lz77_decoding_function.o \
                  libpnm.o


#--------------------------------------------------
#Q3c: Executable compare_pgm_images depends on 
#the source file compare_pgm_images.c and 
#the object files
#  mean_absolute_error.o
#  libpnm.o
Q3c compare_pgm_images: compare_pgm_images.c \
                        mean_absolute_error.o \
                        libpnm.o
	$(CC) $(CFLAG) -o compare_pgm_images \
                      compare_pgm_images.c \
                      mean_absolute_error.o \
                      libpnm.o


#--------------------------------------------------
#Q1: lz77_encoding_function.o depends on 
#the source file 
#  lz77_encoding_function.c 
#the header files 
#  lz77_encoding_function.h
#  libpnm.h
Q1 lz77_encoding_function.o: lz77_encoding_function.c \
                               lz77_encoding_function.h \
                               libpnm.h
	$(CC) $(CFLAG) -c lz77_encoding_function.c


#--------------------------------------------------
#Q2: lz77_decoding_function.o depends on 
#the source file 
#  lz77_decoding_function.c 
#the header files 
#  lz77_decoding_function.h
#  libpnm.h
Q2 lz77_decoding_function.o: lz77_decoding_function.c \
                               lz77_decoding_function.h \
                               libpnm.h
	$(CC) $(CFLAG) -c lz77_decoding_function.c


#--------------------------------------------------
#mean_absolute_error.o depends on 
#the source file 
#  mean_absolute_error.c 
#the header files 
#  mean_absolute_error.h
#  libpnm.h
mean_absolute_error.o: mean_absolute_error.c \
                               mean_absolute_error.h \
                               libpnm.h
	$(CC) $(CFLAG) -c mean_absolute_error.c


#--------------------------------------------------
#libpnm.o depends on 
#the source file 
#  libpnm.c 
#the header file 
#  libpnm.h
libpnm.o: libpnm.c libpnm.h
	$(CC) $(CFLAG) -c libpnm.c

#==================================================
# test cases
 
testEncoderValidation: lz77_encoding 
#
# checking inputs validation for lz77_encoding
#
	@echo "----------------------------------------"
	@echo "Checking inputs validation for lz77_encoding"
	@echo
	./lz77_encoding 
	@echo "----------------------------------------"
	./lz77_encoding tttt 
	@echo "----------------------------------------"
	./lz77_encoding test_square.raw.pgm 
	@echo "----------------------------------------"
	./lz77_encoding smooth.raw.pgm   smooth.comp  tttt
	@echo "----------------------------------------"

testDecoderValidation: lz77_decoding 
#
# checking inputs validation for lz77_decoding
#
	@echo "----------------------------------------"
	@echo "Checking inputs validation for lz77_decoding"
	@echo
	./lz77_decoding
	@echo "----------------------------------------"
	./lz77_decoding tttt 
	@echo "----------------------------------------"
	./lz77_decoding test_square.comp
	@echo "----------------------------------------"
	./lz77_decoding test_square.raw.pgm 
	@echo "----------------------------------------"
	./lz77_decoding smooth.comp  smooth.decomp.pgm  tttt
	@echo "----------------------------------------"

testCompareValidation: compare_pgm_images
#
# checking inputs validation for compare_pgm_images
#
	@echo "----------------------------------------"
	@echo "Checking inputs validation for compare_pgm_images"
	@echo
	./compare_pgm_images
	@echo "----------------------------------------"
	./compare_pgm_images tttt 
	@echo "----------------------------------------"
	./compare_pgm_images test_square.raw.pgm 
	@echo "----------------------------------------"
	./compare_pgm_images smooth.raw.pgm  smooth.decomp.pgm  tttt
	@echo "----------------------------------------"

testValidation: lz77_encoding lz77_decoding compare_pgm_images
	make testEncoderValidation
	make testDecoderValidation
	make testCompareValidation

#............................................................

testCompression: lz77_encoding
#
# Generating LZ77 compressed images
#
	@echo "----------------------------------------"
	@echo "Generating  LZ77 compressed images"
	@echo 
	./lz77_encoding peppers.raw.pgm 5120
	@echo "----------------------------------------"
	./lz77_encoding peppers.raw.pgm 1024
	@echo "----------------------------------------"
	./lz77_encoding peppers.raw.pgm 256
	@echo "----------------------------------------"
	./lz77_encoding goldhill.raw.pgm 5120
	@echo "----------------------------------------"
	./lz77_encoding goldhill.raw.pgm 1024
	@echo "----------------------------------------"
	./lz77_encoding goldhill.raw.pgm 256
	@echo "----------------------------------------"

testDecompression: lz77_decoding
#
# Generating LZ77 compressed images
#
	@echo "----------------------------------------"
	@echo "Generating  LZ77 compressed images"
	@echo 
	./lz77_decoding peppers.raw.pgm.5120.lz
	@echo "----------------------------------------"
	./lz77_decoding peppers.raw.pgm.1024.lz
	@echo "----------------------------------------"
	./lz77_decoding peppers.raw.pgm.256.lz
	@echo "----------------------------------------"
	./lz77_decoding goldhill.raw.pgm.5120.lz
	@echo "----------------------------------------"
	./lz77_decoding goldhill.raw.pgm.1024.lz
	@echo "----------------------------------------"
	./lz77_decoding goldhill.raw.pgm.256.lz
	@echo "----------------------------------------"

testComparingImages: compare_pgm_images
#
# Comparing images
#
	@echo "----------------------------------------"
	./compare_pgm_images peppers.raw.pgm peppers.raw.pgm.5120.lz.pgm
	@echo "----------------------------------------"
	./compare_pgm_images peppers.raw.pgm peppers.raw.pgm.1024.lz.pgm
	@echo "----------------------------------------"
	./compare_pgm_images peppers.raw.pgm peppers.raw.pgm.256.lz.pgm
	@echo "----------------------------------------"
	./compare_pgm_images goldhill.raw.pgm goldhill.raw.pgm.5120.lz.pgm
	@echo "----------------------------------------"
	./compare_pgm_images goldhill.raw.pgm goldhill.raw.pgm.1024.lz.pgm
	@echo "----------------------------------------"
	./compare_pgm_images goldhill.raw.pgm goldhill.raw.pgm.256.lz.pgm
	@echo "----------------------------------------"

testAll TestAll  testALL TestALL  testall Testall: #
# All testing cases
#
	make testCompression
	make testDecompression
	make testComparingImages

#==================================================
#Clean all objected files and the executable file
clean  Clean CLEAN:
	rm -f *.o
	rm -f  lz77_encoding lz77_decoding compare_pgm_images


#Clean all compressed images
cleanCOMPRESSED:
	rm -f *.lz
	rm -f *.offsets.csv
	rm -f *.lengths.csv

#Clean all decompressed images
cleanDECOMPRESSED:
	rm -f *.lz.pgm

#Clean compressed/decompressed files, object files, and executable file 
cleanAll CleanAll  cleanALL CleanALL cleanall Cleanall:
	make clean
	make cleanCOMPRESSED cleanDECOMPRESSED
#==================================================
