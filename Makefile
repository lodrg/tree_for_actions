TARGET = container_collection

C_SOURCE_FILES += $(wildcard ./src/*.c)

C_INCLUDES += ./src 



include ./cc-with-test.mk

blah: $(OBJECTS)
	cc -o build/main.exe $^