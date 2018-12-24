#include "DAG.h"

using namespace std;

int DAG::startBlockPosition = 0;
int DAG::endBlockPosition = 0;
int DAG::_position = 0;
int DAG::finalQuaterListPosition = 0;
int DAG::previousEndBlockPosition = 0;
int DAG::finalQuaterListPositionBeforeFinalQuaterList = 0;

std::string blockString[15] =
{
    "if", "ei", "$main", "end$main", "wh", "el", "we", "do", "arrayInit", "endArrayInit"
};

void DAG::locateStartPosition()//(ConstantList<Quaternary> &quaterList)//设定块开始位置
{
    if(endBlockPosition != 0
            && quaterList.get(endBlockPosition).getOption() != "end$main")//块出口的位置并不是最初或程序结束
    {
        //cout << "current finalQuaterListPosition " << finalQuaterListPosition << endl;
        startBlockPosition = previousEndBlockPosition + 1;//曾经的方法不能适应因简化而长度缩短的新quaterList
        //cout << "\nstartBlockPosition = " << startBlockPosition << endl;
        /*bool change = false;
        cout << "初期设定start时的finalQuaterListPosition: " << finalQuaterListPosition << endl;
        for(int i = finalQuaterListPosition; i < quaterList.size(); i++)
        {
            std::string str = quaterList.get(startBlockPosition).getOption();
            for(int j = 0; j < 10; j++)
            {
                if(str == blockString[j])
                {
                    startBlockPosition = i;
                    change = true;
                    cout << "得到的startBlockPostion = " << startBlockPosition << endl;
                    break;
                }
            }
            if(change)
            {
                break;
            }
        }*/
    }
    else
    {
        //cout << "current finalQuaterListPosition " << finalQuaterListPosition << endl;
        startBlockPosition = previousEndBlockPosition;
        //cout << "\nstartBlockPosition = " << startBlockPosition << endl;
    }
}

void DAG::locateEndPosition()//(ConstantList<Quaternary> &quaterList)//设定块结束位置
{
    if(quaterList.get(startBlockPosition).getOption() == "end$main")
    {
        endBlockPosition = startBlockPosition;
        //cout << "endBlockPosition = " << endBlockPosition << endl;
    }
    else
    {
        for(int i = startBlockPosition + 1; i < quaterList.size(); i++)
        {
            bool change = false;
            std::string str = quaterList.get(i).getOption();
            for(int j = 0; j < 10; j++)
            {
                if(str == blockString[j])
                {
                    endBlockPosition = i;
                    change = true;
                    //cout << "endBlockPosition = " << endBlockPosition << endl;
                    break;
                }
            }

            if(change)
            {
                break;
            }
        }
    }
    if(quaterList.get(endBlockPosition).getOption() == "end$main"
            ||quaterList.get(endBlockPosition + 1).getOption() == "end$main")
    {
        prepareForEnd = 1;
    }
}

DAG::DAGUnit* DAG::createUnit(Token token)//创建DAGUnit
{
    DAGUnitList[_position].num = _position;
    DAGUnitList[_position].tokenList[DAGUnitList[_position].tokenNum++] = token;
    DAGUnitList[_position].Fleaf = nullptr;//创建新Unit的初始情况
    DAGUnitList[_position].Lleaf = nullptr;
    DAGUnitList[_position].Rleaf = nullptr;
    _position++;
    return &DAGUnitList[_position-1];
}

void DAG::insertToken(DAGUnit &Unit, Token token)//将一个新的token插入Unit中
{
    Unit.tokenList[Unit.tokenNum++] = token;
}

void DAG::createLleaf(DAGUnit &Unit, DAGUnit &Lleaf)
{
    Unit.Lleaf = &Lleaf;
}

void DAG::createRleaf(DAGUnit &Unit, DAGUnit &Rleaf)
{
    Unit.Rleaf = &Rleaf;
}

void DAG::createFleaf(DAGUnit &Unit, DAGUnit &Fleaf)
{
    Unit.Fleaf = &Fleaf;
}

void DAG::setOperation(std::string operation, int _position)
{
    DAGUnitList[_position].operation = operation;
}

std::string DAG::getOperation(DAGUnit D)
{
    return D.operation;
}


int DAG::compareUnitAndToken(Token token)//锁定token应当放在哪个DAGUnit中
{
    //std::cout << "Current _position = " << _position << std::endl;
    for(int i = _position; i > -1; i--)
    {
        for(int j = 0; j < DAGUnitList[i].tokenNum; j++)
        {
            if(synbl.getCategory(token.getOffset()) == SymbolList::array)//对数组特殊操作
            {
                if(DAGUnitList[i].tokenList[j].getPosition() == token.getPosition()
                        &&DAGUnitList[i].tokenList[j].getOffset() == token.getOffset()
                        && DAGUnitList[i].tokenList[j].getCategory() == token.getCategory())
                {
                    return i;
                }
            }
            else
            {
                if(DAGUnitList[i].tokenList[j].getOffset() == token.getOffset()
                        && DAGUnitList[i].tokenList[j].getCategory() == token.getCategory())
                {
                    return i;
                }
            }

        }
    }
    return -1;
}

int DAG::locateFLeaf(int sameFirst, int sameSecond, std::string operation)
{
    for(int i = _position - 1; i > -1; i--)
    {
        if(DAGUnitList[i].Lleaf == nullptr
                || DAGUnitList[i].Rleaf == nullptr)
        {
            continue;
        }
        if(DAGUnitList[i].Lleaf->num == sameFirst
                && DAGUnitList[i].Rleaf->num == sameSecond
                && DAGUnitList[i].operation == operation)
        {
            return i;
        }
    }
    return -1;
}

void DAG::setActivityList(Token &token, bool IsActive)
{
    token.setActivity(IsActive);
}

void DAG::displayDAGUnit(int i)
{
    std::cout << "\noperation = " << DAGUnitList[i].operation << "  ";
    std::cout << "num = " << DAGUnitList[i].num << "  ";
    for(int j = 0; j < DAGUnitList[i].tokenNum; j++)
    {
        if(DAGUnitList[i].tokenList[j].getCategory() == Token::null)
        {
            std::cout << "name = " << "null" << "  ";
        }
        else if(DAGUnitList[i].tokenList[j].getCategory() == Token::id)
        {
            std::cout << "name = " << synbl.getName(DAGUnitList[i].tokenList[j].getOffset()) << "  ";
            std::cout << "offset = " << DAGUnitList[i].tokenList[j].getOffset() << "  ";
        }
        else if(DAGUnitList[i].tokenList[j].getCategory() == Token::integer)
        {
            std::cout << "name = " << intList.get(DAGUnitList[i].tokenList[j].getOffset()) << "  ";
            std::cout << "offset = " << DAGUnitList[i].tokenList[j].getOffset() << "  ";
        }
        else if(DAGUnitList[i].tokenList[j].getCategory() == Token::real)
        {
            std::cout << "name = " << floatList.get(DAGUnitList[i].tokenList[j].getOffset()) << "  ";
            std::cout << "offset = " << DAGUnitList[i].tokenList[j].getOffset() << "  ";
        }

        // std::cout << "activity = " << DAGUnitList[i].tokenList[j].getActivity() << std::endl;
    }
    if(DAGUnitList[i].Lleaf != nullptr)
        std::cout << "Lleaf = " << DAGUnitList[i].Lleaf->num << "  ";
    if(DAGUnitList[i].Rleaf != nullptr)
        std::cout << "Rleaf = " << DAGUnitList[i].Rleaf->num << std::endl;
    //if(DAGUnitList[i].Fleaf != nullptr)
    //std::cout << "Fleaf = " << DAGUnitList[i].Fleaf->num << std::endl;
}
void DAG::displayDAGUnitList()
{
    std::cout << std::endl;
    std::cout << "DAG for this time" << std::endl;
    for(int i = 0; i < _position; i++)
    {
        std::cout << "\noperation = " << DAGUnitList[i].operation << "  ";
        std::cout << "num = " << DAGUnitList[i].num << "  ";
        for(int j = 0; j < DAGUnitList[i].tokenNum; j++)
        {
            if(DAGUnitList[i].tokenList[j].getCategory() == Token::null)
            {
                std::cout << "name = " << "null" << "  ";
            }
            else if(DAGUnitList[i].tokenList[j].getCategory() == Token::id)
            {
                std::cout << "name = " << synbl.getName(DAGUnitList[i].tokenList[j].getOffset()) << "  ";
                std::cout << "offset = " << DAGUnitList[i].tokenList[j].getOffset() << "  ";
            }
            else if(DAGUnitList[i].tokenList[j].getCategory() == Token::integer)
            {
                std::cout << "name = " << intList.get(DAGUnitList[i].tokenList[j].getOffset()) << "  ";
                std::cout << "offset = " << DAGUnitList[i].tokenList[j].getOffset() << "  ";
            }
            else if(DAGUnitList[i].tokenList[j].getCategory() == Token::real)
            {
                std::cout << "name = " << floatList.get(DAGUnitList[i].tokenList[j].getOffset()) << "  ";
                std::cout << "offset = " << DAGUnitList[i].tokenList[j].getOffset() << "  ";
            }

            // std::cout << "activity = " << DAGUnitList[i].tokenList[j].getActivity() << std::endl;
        }
        if(DAGUnitList[i].Lleaf != nullptr)
            std::cout << "Lleaf = " << DAGUnitList[i].Lleaf->num << "  ";
        if(DAGUnitList[i].Rleaf != nullptr)
            std::cout << "Rleaf = " << DAGUnitList[i].Rleaf->num << std::endl;
        //if(DAGUnitList[i].Fleaf != nullptr)
        //std::cout << "Fleaf = " << DAGUnitList[i].Fleaf->num << std::endl;
    }
}


void DAG::sortTokenList(DAG::DAGUnit &D)
{
    //std::cout << "Let's begin sorting" << std::endl;
    if(D.tokenList[0].getCategory() == Token::integer//若某Unit中首个Token是常量
            || D.tokenList[0].getCategory() == Token::real)
    {
        for(int i = 1; i < D.tokenNum; i++)//则在该TokenList中将最新的常量放在首位
        {
            if(D.tokenList[i].getCategory() == Token::integer
                    ||D.tokenList[i].getCategory() == Token::real)
            {
                D.tokenList[0] = D.tokenList[i];
            }
        }
    }
    else if(D.tokenList[0].getCategory() == Token::id)//若某Unit中首个Token是符号表中变量
    {
        for(int i = 1; i < D.tokenNum; i++)//从第二位开始
        {
            if(D.tokenList[i].getCategory() == Token::integer//若后续中有常量
                    || D.tokenList[i].getCategory() == Token::real)
            {
                Token tempToken;//交换位置
                tempToken = D.tokenList[0];
                D.tokenList[0] = D.tokenList[i];
                D.tokenList[i] = tempToken;
            }
            else if(D.tokenList[i].getCategory() == Token::id)//若后续中有用户定义变量
            {
                //std::cout << "begin sorting for id" << std::endl;
                std::string tempName = synbl.getName(D.tokenList[i].getOffset());//取当前TokenList中Token所指变量的名字
                std::string tempNameFirst = synbl.getName(D.tokenList[0].getOffset());//取TokenList首个Token所指变量的名字
                //std::cout << "tempName = "  << tempName << std::endl;
                //std::cout << "tempNameFirst = "  << tempNameFirst << std::endl;
                if(tempNameFirst[0] != '@')//若首个变量即为用户定义变量
                {
                    break;//不进行操作且退出循环
                }
                else//首个变量为临时变量
                {
                    if(tempName[0] != '@')//若当前扫描到的变量是用户定义变量
                    {
                        Token tempToken;//交换位置
                        tempToken = D.tokenList[0];
                        D.tokenList[0] = D.tokenList[i];
                        D.tokenList[i] = tempToken;
                        break;
                    }
                }
            }
        }
    }

}

void DAG::clearDAGUnitList()
{
    for(int i = 0; i < _position; i++)
    {
        DAGUnitList[i].tokenNum = 0;
        DAGUnitList[i].operation = "\0";
        DAGUnitList[i].Fleaf = nullptr;
        DAGUnitList[i].Lleaf = nullptr;
        DAGUnitList[i].Rleaf = nullptr;
    }
    _position = 0;
}

void DAG::clearActivityList()
{
    for(int i = 0; i < listPosition; i++)
    {
        List[i].isActive = 1;
        List[i].name = "\0";
    }
    listPosition = 0;
}



void DAG::simplify()//(ConstantList<Quaternary> &quaterList)//开始化简
{
    locateStartPosition();//定位块开始位置
    //cout << "startBlockPosition in simplify = " << startBlockPosition << endl;

    locateEndPosition();//定位块结束位置
    //cout << "endBlockPosition in simplify = " << endBlockPosition << endl;
    previousEndBlockPosition = endBlockPosition;//此处将endBlockPosition的值保存下来，为方便确定下一次的startBlockPosition
    bool singleOperator = false;//定义变量检查是否是赋值操作符
    if(quaterList.get(startBlockPosition).getOption() == "arrayInit"
            ||quaterList.get(endBlockPosition).getOption() == "endArrayInit")
    {
        /*cout << "出现数组情况" << endl;
        cout << "当前开始四元式：";
        displayQuaternary(quaterList.get(startBlockPosition));
        cout << "当前结束四元式：";
        displayQuaternary(quaterList.get(endBlockPosition));
        */
    }
    else
    {
        /*cout << "当前开始四元式：";
        displayQuaternary(quaterList.get(startBlockPosition));
        cout << "当前结束四元式：";
        displayQuaternary(quaterList.get(endBlockPosition));*/
        for(int i = startBlockPosition; i < endBlockPosition; i++)
        {
            if(quaterList.get(i).getTarget().getCategory() == Token::null)//Token是空的
            {
                continue;
            }
            else//Token非空
            {
                DAGUnit *LeafAndFather[3];//0, 1, 2分别是左叶，右叶，父叶
                if(quaterList.get(i).getOption() == "=")
                {
                    singleOperator = true;
                }
                else
                {
                    singleOperator = false;
                }
                if(singleOperator)//赋值运算符情况
                {
                    /*for(int j = i; j < endBlockPosition; j++)//防止重复赋值
                    {
                        Quaternary currentQuaternary = quaterList.get(j);//取当前四元式
                        Quaternary nextQuaternary = quaterList.get(j + 1);//取下一个四元式
                        if(synbl.getName(currentQuaternary.getTarget().getOffset())
                           == synbl.getName(nextQuaternary.getTarget().getOffset())
                           &&synbl.getName(currentQuaternary.getFirst().getOffset())
                           == synbl.getName(nextQuaternary.getFirst().getOffset()))//若二者赋值对象相同
                        {
                            i++;//将当前指针后移
                        }
                        else break;
                    }*/


                    int sameFirst = compareUnitAndToken(quaterList.get(i).getFirst()); //标记在UnitList具有与QuaterList中的FirstToken相同Token的Unit在UnitList中的位置
                    //std::cout << "sameFirst: "  << sameFirst << std::endl;
                    if(sameFirst == -1)
                    {
                        LeafAndFather[0] = createUnit(quaterList.get(i).getFirst());//创建FirstToken
                        insertToken(*LeafAndFather[0], quaterList.get(i).getTarget());//插入targetToken
                        sortTokenList(*LeafAndFather[0]);//还需要一个sort函数调整用户变量与临时变量的顺序
                        for(int i = 0; i < 3; i++)
                        {
                            LeafAndFather[i] = nullptr;//操作完后赋值为空
                        }
                    }
                    else
                    {
                        insertToken(DAGUnitList[sameFirst], quaterList.get(i).getTarget());
                        sortTokenList(DAGUnitList[sameFirst]);//还需要一个sort函数调整用户变量与临时变量的顺序
                    }
                    //displayDAGUnitList();
                }
                else//非赋值运算符情况
                {
                    int sameFirst = compareUnitAndToken(quaterList.get(i).getFirst()); //标记在UnitList具有与QuaterList中的FirstToken相同Token的Unit在UnitList中的位置
                    int sameSecond = compareUnitAndToken(quaterList.get(i).getSecond());//标记在UnitList具有与QuaterList中的SecondToken相同Token的Unit在UnitList中的位置
                    //std::cout << "sameFirst: "  << sameFirst << " sameSecond: " << sameSecond << std::endl;
                    if(sameFirst == -1)//Unit表中没有该项
                    {
                        LeafAndFather[0] = createUnit(quaterList.get(i).getFirst());
                        //std::cout << "current Lleaf = " << LeafAndFather[0]->num << std::endl;
                    }
                    else
                    {
                        LeafAndFather[0] = &DAGUnitList[sameFirst];
                    }
                    if(sameSecond == -1)
                    {
                        LeafAndFather[1] = createUnit(quaterList.get(i).getSecond());
                        //std::cout << "current Rleaf = " << LeafAndFather[1]->num << std::endl;
                    }
                    else
                    {
                        LeafAndFather[1] = &DAGUnitList[sameSecond];
                    }
                    if(sameFirst == -1 || sameSecond == -1)//target仅在First 与 Second均已存在于DAG图中才不需要新创建
                    {
                        LeafAndFather[2] = createUnit(quaterList.get(i).getTarget());
                        setOperation(quaterList.get(i).getOption(), _position - 1);//与tatget相关则要考虑setoption
                        //-1 是因为CreateUnit之后自动operation++，此处用抵消-1
                        createLleaf(*LeafAndFather[2], *LeafAndFather[0]);
                        createRleaf(*LeafAndFather[2], *LeafAndFather[1]);
                        createFleaf(*LeafAndFather[0], *LeafAndFather[2]);
                        createFleaf(*LeafAndFather[1], *LeafAndFather[2]);
                        for(int i = 0; i < 3; i++)
                        {
                            LeafAndFather[i] = nullptr;//操作完后赋值为空
                        }
                        // displayDAGUnitList();
                    }
                    else//本情况下只需要insertToken即可
                    {
                        int sameFather = locateFLeaf(sameFirst, sameSecond, quaterList.get(i).getOption());
                        if(sameFather == -1)//没有能够直接插入的父亲
                        {
                            LeafAndFather[2] = createUnit(quaterList.get(i).getTarget());
                            setOperation(quaterList.get(i).getOption(), _position - 1);//与tatget相关则要考虑setoption
                            createLleaf(*LeafAndFather[2], *LeafAndFather[0]);
                            createRleaf(*LeafAndFather[2], *LeafAndFather[1]);
                            for(int i = 0; i < 3; i++)
                            {
                                LeafAndFather[i] = nullptr;//操作完后赋值为空
                            }
                        }
                        else
                        {
                            insertToken(DAGUnitList[sameFather], quaterList.get(i).getTarget());
                            sortTokenList(DAGUnitList[sameFather]);//一个sort函数
                        }
                    }
                }
            }
        }
    }

    //displayDAGUnitList();
}

void DAG::quaternaryInAnUnit(DAGUnit D, int &positionInFinalQuaterList)
{
    Token nullToken;//为了应对空Token
    nullToken.setCategory(Token::null);
    Quaternary tempQ;
    std::string tempName;
    //std::cout << positionINFinalQuaterList << std ::endl;
    if(D.tokenList[0].getCategory() != Token::id)//表明首位是常量
    {
        if(D.tokenList[0].getCategory() == Token::integer
                || D.tokenList[0].getCategory() == Token::real)//出现常数，若接下来有用户定义变量，则需要赋值
        {
            for(int j = 1; j < D.tokenNum; j++)//对用户变量进行赋值
            {
                //注意从1开始
                if(D.tokenList[j].getCategory() == Token::id)
                {
                    tempName = synbl.getName(D.tokenList[j].getOffset());
                    if(tempName[0] != '@')
                    {
                        tempQ.setOption("=");
                        tempQ.setFirst(D.tokenList[0]);
                        tempQ.setSecond(nullToken);
                        tempQ.setTarget(D.tokenList[j]);
                        quaterList.set(positionInFinalQuaterList++, tempQ);
                    }
                }

            }
        }
    }
    else//表明首位是符号表中的量
    {
        tempName = synbl.getName(D.tokenList[0].getOffset());
        if(tempName[0] != '@')
        {
            for(int j = 1; j < D.tokenNum; j++)
            {
                tempName = synbl.getName(D.tokenList[j].getOffset());
                if(tempName[0] != '@')
                {
                    tempQ.setOption("=");
                    tempQ.setFirst(D.tokenList[0]);
                    tempQ.setSecond(nullToken);
                    tempQ.setTarget(D.tokenList[j]);
                    quaterList.set(positionInFinalQuaterList++, tempQ);
                }
            }
        }
    }
    //displayDAGUnitList();
}

void DAG::finalQuaterList()
{
    Quaternary startBlockQuaternary = quaterList.get(startBlockPosition);//存一下开始四元式
    Quaternary endBlockQuaternary = quaterList.get(endBlockPosition);//存一下结束四元式
    finalQuaterListPositionBeforeFinalQuaterList = finalQuaterListPosition;//保存在最终四元式生成中发生改变的位置变量
    //因为开始四元式就是上一个的结束四元式，所以只需要存结束的就可以
    //int positionInFinalQuaterList;//用于生成最终四元式的循环变量
    //if(quaterList.get(startBlockPosition).getTarget().getCategory() == Token::null)
    //{
    //    positionInFinalQuaterList = startBlockPosition + 1;//当startBlockPosition指向某些特定操作
    //}
    //else
    //{
    //    positionInFinalQuaterList = startBlockPosition;//startBlockPosition指向一般四元式
    //}
    if(quaterList.get(finalQuaterListPosition).getTarget().getCategory() == Token::null)
    {
        finalQuaterListPosition++;//跳过Block分界四元式
    }
    Token nullToken;//设置临时空Token
    nullToken.setCategory(Token::null);
    Quaternary tempQ;//设置临时四元式
    std::string tempName;

    if(startBlockQuaternary.getOption() != "$main")//上一个块的结束四元式的下一个也是块的分界四元式
    {
        if(startBlockQuaternary.getTarget().getCategory() == Token::null
                && quaterList.get(startBlockPosition - 1).getTarget().getCategory() == Token::null)
        {
            quaterList.set(finalQuaterListPosition++, startBlockQuaternary);
        }

    }
    //处理数组情况
    if(quaterList.get(startBlockPosition).getOption() == "arrayInit"
            ||quaterList.get(endBlockPosition).getOption() == "endArrayInit")
    {
        for(int i = startBlockPosition; i < endBlockPosition; i++)
        {
            quaterList.set(finalQuaterListPosition++, quaterList.get(i));
        }
    }
    else
    {
        for(int i = 0; i < _position; i++)
        {
            if(DAGUnitList[i].Lleaf == nullptr)//没有子叶的情况
            {
                quaternaryInAnUnit(DAGUnitList[i], finalQuaterListPosition);//针对一个Unit单元生成四元式
            }
            else//有子叶的情况
            {
                tempQ.setOption(DAGUnitList[i].operation);

                tempQ.setFirst(DAGUnitList[i].Lleaf->tokenList[0]);

                tempQ.setSecond(DAGUnitList[i].Rleaf->tokenList[0]);

                tempQ.setTarget(DAGUnitList[i].tokenList[0]);

                quaterList.set(finalQuaterListPosition++, tempQ);

                quaternaryInAnUnit(DAGUnitList[i], finalQuaterListPosition);//针对一个Unit单元生成四元式
            }
        }
    }

    //quaterList.set(positionInFinalQuaterList, endBlockQuaternary);//补上结尾处的四元式,endBlockPosition即为结尾四元式位置
    //考虑到Block的情况，在此将endBlockPosition之后的四元式提前到简化后四元式处
    //int conveyNum = positionInFinalQuaterList;
    //for(int i = endBlockPosition; i < quaterList.size(); i++)
    //{
    //    quaterList.set(conveyNum++, quaterList.get(i));
    //}
    /*if(endBlockPosition - startBlockPosition == 1
       && quaterList.get(startBlockPosition).getOption() != "main")//排除开头的情况
    {
        displayQuaternary(startBlockQuaternary);
        displayQuaternary(endBlockQuaternary);
        quaterList.set(finalQuaterListPosition++, startBlockQuaternary);
        quaterList.set(finalQuaterListPosition, endBlockQuaternary);
    }
    else*/
    //quaterList.set(finalQuaterListPosition, endBlockQuaternary);//补充结尾

    //cout << "the finalQuaterListPosition = " << finalQuaterListPosition << endl;



    quaterList.set(finalQuaterListPosition, endBlockQuaternary);//补充结尾


    /*for(int i = finalQuaterListPosition; i > startBlockPosition; i--)
    {
        cout << "i = " << i << endl;
        if(synbl.getName(quaterList.get(i).getTarget().getOffset())
           == synbl.getName(quaterList.get(i - 1).getTarget().getOffset()))//若两相邻四元式赋值对象相同
        {
            cout << "\n开始简化无意义赋值" << endl;
            for(int j = i - 1; j < finalQuaterListPosition; j++)
            {
                quaterList.set(j, quaterList.get(j + 1));//位于后方的四元式将取代前方的四元式
            }
        }
    }*/
    /*cout << "\nAfter Final QuaterList" << endl;
    for(int i = 0; i < quaterList.size(); i++)
    {
        cout << "(" << i << ")";
        displayQuaternary(quaterList.get(i));
    }
    cout << "\n准备简化无意义赋值" << endl;
    cout << "start = " << startBlockPosition << endl;
    cout << "end = " << endBlockPosition << endl;*/
    /*if(quaterList.get(finalQuaterListPosition).getOption() == "endArrayInit")
    {
        //如果是数组的初始化则不进行简化
        for(int i = finalQuaterListPosition; i < previousEndBlockPosition; i++)
        {
            quaterList.set(i, quaterList.get(i + 1));//将之后的四元式前移
        }
        int searchForArrayInit = finalQuaterListPosition;
        while(quaterList.get(searchForArrayInit).getOption() != "arrayInit")
        {
            searchForArrayInit--;
        }
        //此时searchForArrayInit为arrayInit所在四元式位置
        for(int i = searchForArrayInit; i < previousEndBlockPosition; i++)
        {
            quaterList.set(i, quaterList.get(i + 1));//将之后的四元式前移
        }
    }*/
    //else
    {
        int i = finalQuaterListPosition - 1; //此时finalQuaterListPosition指向的是应该插入块出口四元式的位置
        while(quaterList.get(i).getTarget().getCategory() != Token:: null)
        {
            if(synbl.getName(quaterList.get(i).getTarget().getOffset())
                    == synbl.getName(quaterList.get(i - 1).getTarget().getOffset()))//若两相邻四元式赋值对象相同
            {
                if(synbl.getCategory(quaterList.get(i).getTarget().getOffset()) == SymbolList::array)//目标操作数为array
                    //||synbl.getCategory(quaterList.get(i - 1).getTarget().getOffset()) == SymbolList::array)
                {
                    /*cout << "正在简化array, 当前四元式为:";
                    displayQuaternary(quaterList.get(i));
                    Quaternary tempQForArray;
                    tempQForArray = quaterList.get(i);//取当前四元式
                    Token tempTokenForArray;
                    tempTokenForArray = quaterList.get(i).getTarget();//取当前四元式targetToken
                    tempTokenForArray.setPosition(intList.get(quaterList.get(i - 1).getFirst().getOffset()));//把上一个对数组位置进行的赋值语句内容复制
                    tempQForArray.setTarget(tempTokenForArray);
                    quaterList.set(i, tempQForArray);*/
                }//出现数组
                else
                {
                    //cout << "\n开始简化无意义赋值" << endl;
                    for(int j = i - 1; j < finalQuaterListPosition; j++)
                    {
                        quaterList.set(j, quaterList.get(j + 1));//位于后方的四元式将取代前方的四元式
                    }
                    finalQuaterListPosition--;
                }

            }
            if((quaterList.get(i).getFirst().getCategory() == Token::integer&&quaterList.get(i).getSecond().getCategory() == Token::integer))//常数运算的简化
            {
                if(quaterList.get(i).getOption() == ">"
                        ||quaterList.get(i).getOption() == "<"
                        ||quaterList.get(i).getOption() == "=="
                        ||quaterList.get(i).getOption() == ">="
                        ||quaterList.get(i).getOption() == "<="
                        ||quaterList.get(i).getOption() == "!=")//遇到逻辑运算，不操作
                {

                }
                else
                {
                    //cout << "情况一：" << endl;
                    int first, second;
                    first = intList.get(quaterList.get(i).getFirst().getOffset());//取操作数
                    second = intList.get(quaterList.get(i).getSecond().getOffset());
                    Token tempToken;
                    Quaternary tempQuaternary;
                    tempToken.setCategory(Token::integer);
                    if(quaterList.get(i).getOption() == "+")
                    {
                        tempToken.setOffset(intList.insert(first + second));
                    }
                    else if(quaterList.get(i).getOption() == "-")
                    {
                        tempToken.setOffset(intList.insert(first - second));
                    }
                    else if(quaterList.get(i).getOption() == "*")
                    {
                        tempToken.setOffset(intList.insert(first * second));
                    }
                    else if(quaterList.get(i).getOption() == "/")
                    {
                        tempToken.setOffset(intList.insert(first / second));
                    }

                    tempQuaternary.setOption("=");
                    tempQuaternary.setFirst(tempToken);
                    tempQuaternary.setSecond(nullToken);
                    tempQuaternary.setTarget(quaterList.get(i).getTarget());
                    quaterList.set(i, tempQuaternary);
                }
            }
            else if((quaterList.get(i).getFirst().getCategory() == Token::real&&quaterList.get(i).getSecond().getCategory() == Token::real)
                    ||(quaterList.get(i).getFirst().getCategory() == Token::integer&&quaterList.get(i).getSecond().getCategory() == Token::real)
                    ||(quaterList.get(i).getFirst().getCategory() == Token::real&&quaterList.get(i).getSecond().getCategory() == Token::integer))
            {
                if(quaterList.get(i).getOption() == ">"
                        ||quaterList.get(i).getOption() == "<"
                        ||quaterList.get(i).getOption() == "=="
                        ||quaterList.get(i).getOption() == ">="
                        ||quaterList.get(i).getOption() == "<="
                        ||quaterList.get(i).getOption() == "!=")//遇到逻辑运算，不操作
                {

                }
                else
                {
                    //cout << "情况二：" << endl;
                    float first, second, result;
                    if(quaterList.get(i).getFirst().getCategory() == Token::real
                            &&quaterList.get(i).getSecond().getCategory() == Token::real)
                    {
                        first = floatList.get(quaterList.get(i).getFirst().getOffset());//取操作数
                        second = floatList.get(quaterList.get(i).getSecond().getOffset());
                    }
                    else if(quaterList.get(i).getFirst().getCategory() == Token::integer
                            &&quaterList.get(i).getSecond().getCategory() == Token::real)
                    {
                        first = intList.get(quaterList.get(i).getFirst().getOffset());//取操作数
                        second = floatList.get(quaterList.get(i).getSecond().getOffset());
                    }
                    else if(quaterList.get(i).getFirst().getCategory() == Token::real
                            &&quaterList.get(i).getSecond().getCategory() == Token::integer)
                    {
                        first = floatList.get(quaterList.get(i).getFirst().getOffset());//取操作数
                        second = intList.get(quaterList.get(i).getSecond().getOffset());
                    }
                    Token tempToken;
                    Quaternary tempQuaternary;

                    tempToken.setCategory(Token::real);
                    if(quaterList.get(i).getOption() == "+")
                    {
                        result = first + second;
                        tempToken.setOffset(floatList.insert(result));
                    }
                    else if(quaterList.get(i).getOption() == "-")
                    {
                        result = first - second;
                        tempToken.setOffset(floatList.insert(result));
                    }
                    else if(quaterList.get(i).getOption() == "*")
                    {
                        result = first * second;
                        tempToken.setOffset(floatList.insert(result));
                    }
                    else if(quaterList.get(i).getOption() == "/")
                    {
                        result = first / second;
                        tempToken.setOffset(floatList.insert(result));
                    }

                    tempQuaternary.setOption("=");
                    tempQuaternary.setFirst(tempToken);
                    tempQuaternary.setSecond(nullToken);
                    tempQuaternary.setTarget(quaterList.get(i).getTarget());
                    quaterList.set(i, tempQuaternary);
                }
            }
            i--;
        }
    }
    /*cout << "\nAfter Final QuaterList(简化)" << endl;
    for(int i = 0; i < quaterList.size(); i++)
    {
        cout << "(" << i << ")";
        displayQuaternary(quaterList.get(i));
    }*/

    //cout << "quaterListSize = " << quaterList.size() << endl;
    //cout << "finalQuaterListPosition = " << finalQuaterListPosition << endl;
    //endBlockPosition = positionInFinalQuaterList;
}

int DAG::findVariateInActivityList(std::string tempName, int position)
{
    for(int j = 0; j < listPosition; j++)//将四元式中的变量存入
    {
        if(List[j].name == tempName && List[j].position == position)//找到了相同变量
        {
            return j;
        }
    }
    return -1;
}

void DAG::setActivityList()//设置活动信息表
{
    Token nullToken;
    nullToken.setCategory(Token::null);
    Quaternary endBlockQuaternary;
    if(prepareForEnd)
    {
        endBlockQuaternary.setOption("end$main");
        endBlockQuaternary.setFirst(nullToken);
        endBlockQuaternary.setSecond(nullToken);
        endBlockQuaternary.setTarget(nullToken);
    }
    //先进行初始化
    //初始化的方法是在DAGUitList中将相关的变量存好
    //早期的是依靠StartBlockPosition与endBlockPosition，但是此二者在实际操作中都可能因为quaterList简化而不能正确指明开头与结尾
    //重新定位开始位置
    if(quaterList.get(finalQuaterListPositionBeforeFinalQuaterList).getTarget().getCategory() == Token::null)
    {
        startBlockPosition = finalQuaterListPositionBeforeFinalQuaterList + 1;
    }
    else
    {
        startBlockPosition = finalQuaterListPositionBeforeFinalQuaterList;
    }
    locateEndPosition();//由于简化导致了原quaterList长度缩短，此处需要重新定义尾部


    //以下是活动信息表的建立
    for(int i = startBlockPosition; i < endBlockPosition; i++)
    {
        /*cout << "current start: " << startBlockPosition << endl;
        cout << "current end: " << endBlockPosition << endl;
        cout << "after Simplify finalQuaterListPosition: " << finalQuaterListPosition << endl;
        cout << "initialing :";*/

        //displayQuaternary(quaterList.get(i));
        if(quaterList.get(i).getFirst().getCategory() != Token::null
                &&quaterList.get(i).getFirst().getCategory() != Token::integer
                &&quaterList.get(i).getFirst().getCategory() != Token::real)
        {
            std::string tempNameFirst = synbl.getName(quaterList.get(i).getFirst().getOffset());
            int tempPosition = quaterList.get(i).getFirst().getPosition();
            if(findVariateInActivityList(tempNameFirst, tempPosition) == -1)
            {
                List[listPosition].name = tempNameFirst;
                if(tempNameFirst[0] == '@')
                {
                    List[listPosition].isActive = 0;
                }
                else
                {
                    List[listPosition].isActive = 1;
                }
                List[listPosition].position = tempPosition;
                listPosition++;
            }
        }
        if(quaterList.get(i).getSecond().getCategory() != Token::null
                &&quaterList.get(i).getSecond().getCategory() != Token::integer
                &&quaterList.get(i).getSecond().getCategory() != Token::real)
        {
            std::string tempNameSecond = synbl.getName(quaterList.get(i).getSecond().getOffset());
            int tempPosition = quaterList.get(i).getSecond().getPosition();
            if(findVariateInActivityList(tempNameSecond, tempPosition) == -1)
            {
                List[listPosition].name = tempNameSecond;
                if(tempNameSecond[0] == '@')
                {
                    List[listPosition].isActive = 0;
                }
                else
                {
                    List[listPosition].isActive = 1;
                }
                List[listPosition].position = tempPosition;
                listPosition++;
            }
        }
        if(quaterList.get(i).getTarget().getCategory() != Token::null
                &&quaterList.get(i).getTarget().getCategory() != Token::integer
                &&quaterList.get(i).getTarget().getCategory() != Token::real)
        {
            std::string tempNameTarget = synbl.getName(quaterList.get(i).getTarget().getOffset());
            int tempPosition = quaterList.get(i).getTarget().getPosition();
            //std::cout << tempNameTarget << std::endl;
            if(findVariateInActivityList(tempNameTarget, tempPosition) == -1)
            {
                List[listPosition].name = tempNameTarget;
                //std::cout << "ListName = " << List[listPosition].name << std::endl;
                if(tempNameTarget[0] == '@')
                {
                    List[listPosition].isActive = 0;
                }
                else
                {
                    List[listPosition].isActive = 1;
                }
                List[listPosition].position = tempPosition;
                listPosition++;
            }
        }
    }
    //cout << "after initial:" << endl;
    /*for(int i = 0; i < listPosition; i++)
    {
        cout << List[i].name << " " << List[i].isActive << endl;
    }*/


    //以下开始操作
    int arrayFlag = 0;//标示遇到数组
    while(quaterList.get(endBlockPosition).getOption() == "endArrayInit"
            ||quaterList.get(endBlockPosition).getOption() == "arrayInit")
    {
        //是数组初始化则无活动信息表操作，但是要删除相应四元式
        arrayFlag = 1;
        /*cout << "出现array情况\n";
        displayQuaternary(quaterList.get(endBlockPosition));
        cout << "当前previousEndBlockPosition " << previousEndBlockPosition << endl;
        cout << "当前EndBlockPosition " << endBlockPosition << endl;
        cout << "当前StartBlockPosition " << startBlockPosition << endl;
        cout << "当前finalQuaterListPosition " << finalQuaterListPosition << endl;
        cout << "当前finalQuaterListPositionBeforeChange" << finalQuaterListPositionBeforeFinalQuaterList << endl;
        */
        //for(int i = 0; i < quaterList.size(); i++)
        //{
        //    cout << "(" << i << ")";
        //    displayQuaternary(quaterList.get(i));
        //}

        int tempNumForArray = endBlockPosition;
        //cout << "break point 1" << endl;
        //cout << "开始修改" << endl;
        for(int i = tempNumForArray; i < quaterList.size(); i++)
        {
            /*cout << "将";
            displayQuaternary(quaterList.get(i));
            cout << "替换为";
            displayQuaternary(quaterList.get(i + 1));
            cout << "i = " << i << endl;*/
            quaterList.set(i, quaterList.get(i + 1));
        }
        /*cout << "第一次修改结束, 结果为" << endl;
        //cout << "当前tempNumForArray:" << tempNumForArray << endl;
        for(int i = 0; i < quaterList.size(); i++)
        {
            cout << "(" << i << ")";
            displayQuaternary(quaterList.get(i));
        }*/

        int findArrayInitFlag = 1;
        while(quaterList.get(tempNumForArray).getOption() != "arrayInit")
        {
            if(quaterList.get(tempNumForArray).getOption() != "$main")
            {
                findArrayInitFlag = 0;
                break;
            }
            tempNumForArray--;
        }
        //cout << "break point 3" << endl;
        if(findArrayInitFlag == 1)
        {
            for(int i = tempNumForArray; i < quaterList.size(); i++)
            {
                //cout << "将";
                //displayQuaternary(quaterList.get(i));
                //cout << "替换为";
                //displayQuaternary(quaterList.get(i + 1));
                //cout << "i = " << i << endl;
                quaterList.set(i, quaterList.get(i + 1));
            }
        }
        //cout << "第二次修改结束, 结果为" << endl;
        //cout << "当前tempNumForArray:" << tempNumForArray << endl;
        /*for(int i = 0; i < quaterList.size(); i++)
        {
            cout << "(" << i << ")";
            displayQuaternary(quaterList.get(i));
        }*/
        //初始化array之后finalQuaterListPositon在endArrayInit处
        //但是之前的arrayInit四元式与当前的endArrayInit被删除了
        //所以要减一
        if(quaterList.get(endBlockPosition).getOption() == "endArrayInit")//如果是初始化，就呆在删除初始化语句位置
        finalQuaterListPosition--;
    }
    if(arrayFlag == 0)//不是数组初始化
    {
        //cout << "不是数组初始化的情况" << endl;
        for(int i = endBlockPosition; i >= startBlockPosition; i--)
        {
            if(quaterList.get(i).getTarget().getCategory() != Token::null
                    &&quaterList.get(i).getTarget().getCategory() != Token::integer
                    &&quaterList.get(i).getTarget().getCategory() != Token::real)//不是空
            {
                std::string tokenName = synbl.getName(quaterList.get(i).getTarget().getOffset());//截取名字
                int tempPosition = quaterList.get(i).getTarget().getPosition();
                int tempListPosition = findVariateInActivityList(tokenName, tempPosition);//比较是否有相同的
                if(tempListPosition != -1)//有相同的
                {
                    Quaternary tempQ;
                    Token tempT;
                    tempQ = quaterList.get(i);
                    tempT = tempQ.getTarget();
                    tempT.setActivity(List[tempListPosition].isActive);
                    tempQ.setTarget(tempT);
                    quaterList.set(i, tempQ);
                    List[tempListPosition].isActive = false;

                }
            }
            else
            {
                //std::cout << "targetNullToken" << std::endl;
            }
            if(quaterList.get(i).getSecond().getCategory() != Token::null
                    &&quaterList.get(i).getSecond().getCategory() != Token::integer
                    &&quaterList.get(i).getSecond().getCategory() != Token::real)//不是空
            {
                std::string tokenName = synbl.getName(quaterList.get(i).getSecond().getOffset());//截取名字
                int tempPosition = quaterList.get(i).getSecond().getPosition();
                int tempListPosition = findVariateInActivityList(tokenName, tempPosition);//比较是否有相同的
                if(tempListPosition != -1)//有相同的
                {

                    Quaternary tempQ;
                    Token tempT;
                    tempQ = quaterList.get(i);
                    tempT = tempQ.getSecond();
                    tempT.setActivity(List[tempListPosition].isActive);
                    //std::cout << "current List position = " << tempListPosition << std::endl;
                    //std::cout << "current List name = " << List[tempListPosition].name << std::endl;
                    //std::cout << "current List active = " << List[tempListPosition].isActive << std::endl;
                    tempQ.setSecond(tempT);
                    quaterList.set(i, tempQ);
                    List[tempListPosition].isActive = true;
                }
            }
            else
            {
                //std::cout << "SecondnullToken" << std::endl;
            }
            if(quaterList.get(i).getFirst().getCategory() != Token::null)//不是空
            {
                std::string tokenName = synbl.getName(quaterList.get(i).getFirst().getOffset());//截取名字
                int tempPosition = quaterList.get(i).getFirst().getPosition();
                int tempListPosition = findVariateInActivityList(tokenName, tempPosition);//比较是否有相同的
                if(tempListPosition != -1)//有相同的
                {

                    Quaternary tempQ;
                    Token tempT;
                    tempQ = quaterList.get(i);
                    tempT = tempQ.getFirst();
                    tempT.setActivity(List[tempListPosition].isActive);
                    tempQ.setFirst(tempT);
                    quaterList.set(i, tempQ);
                    List[tempListPosition].isActive = true;
                }
            }
            else
            {
                //std::cout << "FirstnullToken" << std::endl;
            }
        }
    }
    //cout << "23333333333333333333333333" << endl;
    //cout << "after activity quaterList" << endl;
    /*for(int i = 0; i < quaterList.size() ; i++)
    {
        cout << "(" << i << ")";
        displayQuaternary(quaterList.get(i));
    }*/

    clearActivityList();
    clearDAGUnitList();
    if(prepareForEnd)
    {
        if(quaterList.get(finalQuaterListPosition).getOption() == "end$main")
        {

        }
        else
        {
            finalQuaterListPosition++;
            quaterList.set(finalQuaterListPosition, endBlockQuaternary);
        }
        newQuaterListSize = finalQuaterListPosition + 1;
        quaterList.setSize(newQuaterListSize);
        //if(quaterList.get(finalQuaterListPosition).getOption)
    }


}

int DAG::getStartBlockPosition()
{
    return startBlockPosition;
}

int DAG::getEndBlockPosition()
{
    return endBlockPosition;
}

int DAG::getPrepareForEnd()
{
    return prepareForEnd;
}

int DAG::getPreviousEndBlockPosition()
{
    return previousEndBlockPosition;
}
