cc = g++
target = mcc
object = mcc.o SymbolList.o common.o

TOP_PATH = $(shell pwd)

CPPFLAGS = -I$(TOP_PATH)/module/common

vpath % ./module/common
vpath % ./module/scanner

$(target): $(object)
	$(cc) $(object) -o $(target)

$(object, *.o): %.o : %.cpp
	$(cc) -c $(CPPFLAGS)  $< -o $@

.PHONY: clean
clean:
	-rm $(target) $(object)