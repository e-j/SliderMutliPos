#ifndef CSLIDERMP_INTERVAL_H
#define CSLIDERMP_INTERVAL_H

#include "cslidermp_handle.h"
#include <QColor>

class CSliderMP_Interval : public QObject{
    Q_OBJECT

    Q_PROPERTY(QColor spanColor READ getSpanColor WRITE setSpanColor )
    Q_PROPERTY(bool enabled READ isEnable WRITE setIsEnable )

public:
    CSliderMP_Interval(QObject * parent = 0);
    CSliderMP_Interval(const CSliderMP_Interval & src,QObject * parent = 0);

    QColor getSpanColor(void) const;
    bool isEnable(void) const;

    void setSpanColor(QColor value);
    void setIsEnable(bool value);

    void assignHandles(CSliderMP_Handle * begin,CSliderMP_Handle * end);
    const int beginValue(void);
    const int stopValue(void);

private:
    QColor spanColor;
    bool enabled;
    CSliderMP_Handle * handleBegin;
    CSliderMP_Handle * handleEnd;

    const int handleValue(CSliderMP_Handle * handlePtr);
};

#endif // CSLIDERMP_INTERVAL_H
