cc = g++
target = mcc
main = mcc.o
object = SymbolList.o Common.o Token.o TypeList.o Buffer.o Scanner.o

TOP_PATH = $(shell pwd)

CPPFLAGS = -I$(TOP_PATH)/module/common -I$(TOP_PATH)/module/scanner

vpath % ./module/common
vpath % ./module/scanner

$(target): $(object) $(main)
	$(cc) $(object) $(main) -o $(target)

$(main):%.o: %.cpp
	$(cc) -c $(CPPFLAGS) $< -o $@

$(object):%.o : %.h
$(object): %.o : %.cpp
	$(cc) -c $(CPPFLAGS)  $< -o $@

.PHONY: clean
clean:
	-rm $(target) $(object)

