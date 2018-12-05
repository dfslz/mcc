cc = g++
target = mcc
main = mcc.o
object = SymbolList.o Common.o Token.o TypeList.o Buffer.o Scanner.o

TOP_PATH = $(shell pwd)

CPPFLAGS = -I$(TOP_PATH)/module/common -I$(TOP_PATH)/module/scanner -I$(TOP_PATH)/module/parser

vpath % ./module/common
vpath % ./module/scanner
vpath % ./module/parser

$(target): $(object) $(main)
	$(cc) $(object) $(main) -o $(target)

$(main):%.o: %.cpp
	$(cc) -c $(CPPFLAGS) $< 

$(object):%.o : %.h
$(object): %.o : %.cpp
	$(cc) -c $(CPPFLAGS) $<

.PHONY: clean
clean:
	-rm $(target) $(object) $(main)

