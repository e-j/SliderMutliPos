#include "cslidermp_interval.h"

CSliderMP_Interval::CSliderMP_Interval(QObject * parent) : QObject(parent){
    spanColor = QColor(Qt::gray);
    enabled = true;
    handleBegin = 0;
    handleEnd = 0;
}
CSliderMP_Interval::CSliderMP_Interval(const CSliderMP_Interval & src,QObject * parent) : QObject(parent){
    this->spanColor = src.spanColor;
    this->enabled = src.enabled;
    this->handleBegin = src.handleBegin;
    this->handleEnd = src.handleEnd;
}


QColor CSliderMP_Interval::getSpanColor(void) const{
    return spanColor;
}
bool CSliderMP_Interval::isEnable(void) const{
    return enabled;
}

void CSliderMP_Interval::setSpanColor(QColor value){
    spanColor = value;
}

void CSliderMP_Interval::setIsEnable(bool value){
    enabled = value;
}

void CSliderMP_Interval::assignHandles(CSliderMP_Handle * begin,CSliderMP_Handle * end){
    handleBegin = begin;
    handleEnd = end;
}
const int CSliderMP_Interval::beginValue(void){
    return handleValue(handleBegin);
}

const int CSliderMP_Interval::stopValue(void){
   return handleValue(handleEnd);
}


// PRIVATE FUNCTIONS
const int CSliderMP_Interval::handleValue(CSliderMP_Handle * handlePtr){
    if( handlePtr!=0 ){
        return handlePtr->getValue();
    }
    else{
        // Handle pointer haven't be defined
        return 0;
    }
}
