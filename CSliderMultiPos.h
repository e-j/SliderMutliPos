/**
 * @file	CSliderMultiPos.h
 * @brief       Header file for CSliderMultiPos
 * @author	Eric Jouffrey
 * @date	30 nov. 2011
 */

#ifndef CSLIDERMULTIPOS_H_
#define CSLIDERMULTIPOS_H_

#include <QSlider>
#include <QList>

#include <QKeyEvent>
#include <QMouseEvent>
#include <QApplication>
#include <QStylePainter>
#include <QStyleOptionSlider>
#include <QToolTip>
#include "cslidermp_handle.h"
#include "cslidermp_interval.h"

class CSliderMultiPos : public QSlider{
    Q_OBJECT

public:
    CSliderMultiPos(QWidget * parent = 0);
    CSliderMultiPos( Qt::Orientation orientation, QWidget * parent = 0 );
    virtual ~CSliderMultiPos();

    void setNbValues(int nb);
    void setSpanColor(int spanIndex,QColor color);
    void valueIndication(bool enable);

    int nbValues(void);
    QColor getSpanColor(int spanIndex);

    enum HandleMovementMode {
        FreeMovement,
        NoCrossing,
        NoOverlapping
    };

    HandleMovementMode handleMovementMode() const;
    void setHandleMovementMode(HandleMovementMode mode);

    void displayPositionPermanent(QPoint offset=QPoint(3,-7));
    void stopDisplayPosition(void);
    void displayLabels(QString labelPrefix);
    void stopDisplayLabels(void);

    #define INVALID_INDEX 0xFF

public slots:
    void movePressedHandle();
    void setValue(int posIndex,int value);

signals:
    void valueChange(int indexValue,int value);

protected:
    virtual void keyPressEvent(QKeyEvent* event);
    virtual void mousePressEvent(QMouseEvent* event);
    virtual void mouseMoveEvent(QMouseEvent* event);
    virtual void mouseReleaseEvent(QMouseEvent* event);
    virtual void paintEvent(QPaintEvent* event);


    void setInitialValue(int index,int value);

private:
    void initMembers(void);

    QList<CSliderMP_Handle> handles;
    QList<CSliderMP_Interval> intervals;


    //QList<QColor> spanColor;
    int lastPressedIndex;

    bool blockTracking;
    HandleMovementMode movement;
    int offset;
    int tempPosition;
    bool toolTipIndicateValue;
    QPoint previousMousePos;

    bool displayPosition;
    QPoint displayPositionOffset;

    bool displayIntervalLabel;
    QString intervalLabelPrefix;

    void initStyleOption(QStyleOptionSlider* option,int handleIndex = 0);
    int pick(const QPoint& pt);
    void drawSpan(QStylePainter* painter, const QRect& rect,int baseElemIndex = 0);
    void drawSpanLabel(QStylePainter* painter,QRect rect,int baseElemIndex);
    void drawSpanValue(QStylePainter* painter,QPoint origin,int elemIndex);
    void drawHandle(QStylePainter* painter,int handleIndex);
    void setupPainter(QPainter* painter,QPointF point1,QPointF point2,QColor color);
    void triggerAction(QAbstractSlider::SliderAction action);

    bool handleMousePress(const QPoint& pos, QStyle::SubControl control, int value,int handleIndex);
    int pixelPosToRangeValue(int pos);
    void setCurPosition(int posIndex,int pos);

    int collisionDetection(int newPos);
};

#endif /* CSLIDERMULTIPOS_H_ */
