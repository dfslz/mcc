mcc: mcc.o SymbolList.o Common.o
	g++ -o mcc mcc.o SymbolList.o common.o

mcc.o: mcc.cpp
	g++ mcc.cpp -c

SymbolList.o: ./module/common/SymbolList.cpp
	g++ ./module/common/SymbolList.cpp -c

Common.o: ./module/common/common.cpp
	g++ ./module/common/common.cpp -c

clean:
	rm common.o
	rm mcc.o
	rm SymbolList.o