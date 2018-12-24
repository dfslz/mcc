#ifndef _DAG_H
#define _DAG_H
#include<iostream>
#include <string>
#include "Token.h"
#include "ConstantList.h"
#include "Quaternary.h"
#include "Common.h"
#include "Scanner.h"
#include "SymbolList.h"
#include "DAG.h"



class DAG
{
public:
    void simplify();//(ConstantList<Quaternary> &quaterList);
    void finalQuaterList();
    void setActivityList();
    int getStartBlockPosition();
    int getEndBlockPosition();
    int getPreviousEndBlockPosition();
    int getPrepareForEnd();
private:
    const static int _listMax = 1e2;

    int newQuaterListSize = 0;
    struct DAGUnit
    {
        int num = 0;
        std::string operation = "\0";
        Token tokenList[100];
        int tokenNum = 0;
        DAGUnit *Lleaf, *Rleaf, *Fleaf;
    };
    DAGUnit DAGUnitList[_listMax];
    struct activityList
    {
        std::string name;
        bool isActive;
        int position;
    };
    activityList List[_listMax];
    int listPosition = 0;//ActivityList锟斤拷未锟斤拷使锟矫碉拷锟斤拷锟轿伙拷锟�
    int prepareForEnd = 0;
    static int startBlockPosition;//锟介开始位锟斤拷
    static int endBlockPosition;
    static int _position;//DAGUnitList锟斤拷未锟斤拷使锟矫碉拷锟斤拷锟轿伙拷锟�
    static int finalQuaterListPosition;
    static int finalQuaterListPositionBeforeFinalQuaterList;
    static int previousEndBlockPosition;

    void locateStartPosition();//(ConstantList<Quaternary> &quaterList);
    void locateEndPosition();//(ConstantList<Quaternary> &quaterList);
    DAGUnit* createUnit(Token token);//锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷元
    void setOperation(std::string operation, int _position);//锟斤拷锟斤拷锟斤拷应锟斤拷锟斤拷锟斤拷
    std::string getOperation(DAGUnit D);
    void insertToken(DAGUnit &Unit, Token token);//锟斤拷锟斤拷Token
    void createLleaf(DAGUnit &Unit, DAGUnit &Lleaf);
    void createRleaf(DAGUnit &Unit, DAGUnit &Rleaf);
    void createFleaf(DAGUnit &Unit, DAGUnit &Fleaf);
    int compareUnitAndToken(Token token);//锟斤拷锟斤拷锟斤拷DAG锟斤拷锟叫斤拷锟叫憋拷
    void setActivityList(Token &token, bool IsActive);
    void displayDAGUnitList();
    int locateFLeaf(int sameFirst, int sameSecond, std::string operation);
    void sortTokenList(DAGUnit &D);

    void quaternaryInAnUnit(DAGUnit D, int &positionINFinalQuaterList);

    int findVariateInActivityList(std::string tempName, int position);
    void displayDAGUnit(int i);
    void clearDAGUnitList();
    void clearActivityList();

};


#endif // _DAG_H
