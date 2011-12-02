#include "cslidermp_handle.h"

CSliderMP_Handle::CSliderMP_Handle(int initialPosValue,QObject * parent) : QObject(parent){
    position = initialPosValue;
    value = initialPosValue;
    pressed = QStyle::SC_None;
}
CSliderMP_Handle::CSliderMP_Handle(const CSliderMP_Handle & src,QObject * parent) : QObject(parent){
    this->position = src.position;
    this->value = src.value;
    this->pressed = src.pressed;
}

int CSliderMP_Handle::getPosition(void) const{
    return position;
}

int CSliderMP_Handle::getValue(void) const{
    return value;
}

QStyle::SubControl CSliderMP_Handle::getPressed(void) const{
    return pressed;
}

bool CSliderMP_Handle::isPosDifferOfValue(void) const{
    return value!=position;
}

void CSliderMP_Handle::setPosition(int value){
    position = value;
}

void CSliderMP_Handle::setValue(int value){
    this->value = value;
}

void CSliderMP_Handle::setPressed(QStyle::SubControl value){
    pressed = value;
}
