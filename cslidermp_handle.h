#ifndef CSLIDERMP_HANDLE_H
#define CSLIDERMP_HANDLE_H

#include <QObject>
#include <QStyle>

class CSliderMP_Handle : public QObject{
    Q_OBJECT

    Q_PROPERTY(int position READ getPosition WRITE setPosition )
    Q_PROPERTY(int value READ getValue WRITE setValue )
    Q_PROPERTY(QStyle::SubControl pressed READ getPressed WRITE setPressed )

public:
    CSliderMP_Handle(int initialPosValue = 0,QObject * parent = 0);
    CSliderMP_Handle(const CSliderMP_Handle & src,QObject * parent = 0);


    int getPosition(void) const;
    int getValue(void) const;
    QStyle::SubControl getPressed(void) const;
    bool isPosDifferOfValue(void) const;

    void setPosition(int value);
    void setValue(int value);
    void setPressed(QStyle::SubControl value);

protected:
    int position;
    int value;
    QStyle::SubControl pressed;
};

#endif // CSLIDERMP_HANDLE_H
