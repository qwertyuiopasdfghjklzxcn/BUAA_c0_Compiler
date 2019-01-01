//
// Created by andyson on 18-12-28.
//

#include "Optimizer.h"

Optimizer &Optimizer::getOptimizer(Quadruple &theOriginQuadruple) {
    static Optimizer instance(theOriginQuadruple);
    return instance;
}

void Optimizer::optimize() {
    if (constantCombinationOption) {
        constantCombinationOptimize();
    }
    if (blockPublicExpressionOption) {
        //blockPublicExpressionOptimize();
    }

}

void Optimizer::constantCombinationOptimize() {
    for (int i = 0; i < originQuadruple.length(); i++) {
        if ((originQuadruple.quadrupleList[i]->op == ADD ||
             originQuadruple.quadrupleList[i]->op == SUB ||
             originQuadruple.quadrupleList[i]->op == MUL ||
             originQuadruple.quadrupleList[i]->op == DIV) &&
            isConst(originQuadruple.quadrupleList[i]->src1) &&
            isConst(originQuadruple.quadrupleList[i]->src2)) {
            string replace = originQuadruple.quadrupleList[i]->dst;
            int num1;
            int num2;
            if(originQuadruple.quadrupleList[i]->src1[0] == '\''){
                num1 = (int)originQuadruple.quadrupleList[i]->src1[1];
            } else{
                num1 = atoi(originQuadruple.quadrupleList[i]->src1.c_str());
            }
            if(originQuadruple.quadrupleList[i]->src2[0] == '\''){
                num2 = (int)originQuadruple.quadrupleList[i]->src2[1];
            } else{
                num2 = atoi(originQuadruple.quadrupleList[i]->src2.c_str());
            }
            int resConst = originQuadruple.quadrupleList[i]->op == ADD ? num1 + num2 :
                           originQuadruple.quadrupleList[i]->op == SUB ? num1 - num2 :
                           originQuadruple.quadrupleList[i]->op == MUL ? num1 * num2 :
                           originQuadruple.quadrupleList[i]->op == DIV ? num1 / num2 : 0;
            originQuadruple.quadrupleList.erase(originQuadruple.quadrupleList.begin() + i);
            i--;
            for (int k = i; originQuadruple.length(); k++) {
                if (originQuadruple.quadrupleList[k]->src1 == replace) {
                    originQuadruple.quadrupleList[k]->src1 = to_string(resConst);
                    break;
                } else if (originQuadruple.quadrupleList[k]->src2 == replace) {
                    originQuadruple.quadrupleList[k]->src2 = to_string(resConst);
                    break;
                }
            }
        }
    }
}

void Optimizer::blockPublicExpressionOptimize() {
    return;
}

bool Optimizer::isConst(string a) {
    if (isdigit(a[0]) || a[0] == '+' || a[0] == '-' || a[0] == '\'') {
        return true;
    }
    return false;
}

Optimizer::Optimizer(Quadruple &theOriginQuadruple)
        : originQuadruple(theOriginQuadruple) {
    constantCombinationOption = true;
    blockPublicExpressionOption = true;
}