main SEGMENT
L DB 1000 DUP(0)
ASSUME CS:main, DS:main
PIECE1:
LEA BX,L
MOV AX,12
MOV [BX+0],AX
MOV AX,[BX+0]
MOV [BX+8],AX
MOV AX,3
MOV [BX+12],AX
MOV AX,2
MOV [BX+16],AX
MOV AX,2
MOV [BX+20],AX
MOV AX,4
MOV [BX+24],AX
MOV AX,3
MOV [BX+32],AX
MOV AX,2
MOV [BX+33],AX
MOV AX,5
MOV [BX+34],AX
MOV AX,8
MOV [BX+35],AX
MOV AX,7
MOV [BX+36],AX
MOV AX,0
MOV [BX+73],AX
PIECE2:
MOV AX,[BX+73]
SUB AX,5
CMP AX,0
JGE PIECE7
MOV AX,20
MOV [BX+85],AX
MOV AX,[BX+73]
MOV [BX+89],AX
PIECE3:
MOV AX,[BX+89]
SUB AX,5
CMP AX,0
JGE PIECE6
MOV SI,[BX+73]
MOV AX,[BX+32+SI]
SUB SI,[BX+89]
SUB AX,[BX+32+SI]
CMP AX,0
JLE PIECE4
MOV SI,[BX+89]
MOV AX,[BX+32+SI]
MOV SI,[BX+73]
MOV [BX+32+SI],AX
MOV AX,[BX+0]
MOV SI,[BX+89]
MOV [BX+32+SI],AX
JMP PIECE5
PIECE4:
PIECE5:
MOV AX,[BX+89]
ADD AX,1
MOV [BX+89],AX
JMP PIECE3
PIECE6:
MOV SI,[BX+73]
MOV DL,[BX+32+SI]
ADD DL,30H
MOV AH,02H
INT 21H
MOV AX,[BX+73]
ADD AX,1
MOV [BX+73],AX
JMP PIECE2
PIECE7:
PIECE8:
MOV AH,4CH
INT 21H
main ENDS
END PIECE1