main SEGMENT
L DB 1000 DUP(0)
ASSUME CS:main, DS:main
PIECE1:
LEA BX,L
MOV AX,12
MOV [BX+0],AX
MOV AX,[BX+0]
MOV [BX+8],AX
MOV AX,30
MOV [BX+12],AX
MOV AX,2
MOV [BX+16],AX
MOV AX,2
MOV [BX+20],AX
MOV AX,4
MOV [BX+24],AX
MOV AX,20
MOV [BX+32],AX
MOV AX,2
MOV [BX+33],AX
MOV AX,5
MOV [BX+34],AX
MOV AX,10
MOV [BX+35],AX
MOV AX,30
MOV [BX+36],AX
MOV AX,0
MOV [BX+72],AX
PIECE2:
MOV AX,[BX+72]
SUB AX,5
CMP AX,0
JGE PIECE7
MOV AX,20
MOV [BX+84],AX
MOV AX,[BX+72]
MOV [BX+88],AX
PIECE3:
MOV AX,[BX+88]
SUB AX,5
CMP AX,0
JGE PIECE6
MOV SI,[BX+72]
MOV AX,[BX+32+SI]
SUB SI,[BX+88]
SUB AX,[BX+32+SI]
CMP AX,0
JLE PIECE4
MOV SI,[BX+88]
MOV AX,[BX+32+SI]
MOV SI,[BX+72]
MOV [BX+32+SI],AX
MOV AX,[BX+0]
MOV SI,[BX+88]
MOV [BX+32+SI],AX
JMP PIECE5
PIECE4:
PIECE5:
MOV AX,[BX+88]
ADD AX,1
MOV [BX+88],AX
JMP PIECE3
PIECE6:
MOV AX,[BX+72]
ADD AX,1
MOV [BX+72],AX
JMP PIECE2
PIECE7:
PIECE8:
MOV AH,4CH
INT 21H
main ENDS
END PIECE1
