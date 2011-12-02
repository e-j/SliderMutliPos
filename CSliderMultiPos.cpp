/**
 * @file	CSliderMultiPos.cpp
 * @brief 	Slider with multiple positions for interval tuning
 * @author	Eric Jouffrey
 * @date	30 nov. 2011
 */

#include "CSliderMultiPos.h"

void CSliderMultiPos::initMembers(void){

    lastPressedIndex = INVALID_INDEX;

    blockTracking = false;
    movement = FreeMovement;
    offset = 0;
    tempPosition = 0;
    toolTipIndicateValue = true;
    previousMousePos = QPoint(0,0);

//    spanColor << QColor(0,0,255) << QColor(0,255,0) << QColor(255,255,0);
//    spanColor << QColor(255,127,0) << QColor(255,0,0) << QColor(255,255,255);

    displayPosition = false;
    displayPositionOffset = QPoint(3,-7);
    displayIntervalLabel = false;
    intervalLabelPrefix = "#";

    //connect(this, SIGNAL(rangeChanged(int, int)), this,SLOT(updateRange(int, int)) );
    connect(this, SIGNAL(sliderReleased()), this,SLOT(movePressedHandle()) );
}

CSliderMultiPos::CSliderMultiPos(QWidget * parent) : QSlider(parent){

    initMembers();
}
CSliderMultiPos::CSliderMultiPos( Qt::Orientation orientation, QWidget * parent ) : QSlider(orientation,parent){

    initMembers();
}

CSliderMultiPos::~CSliderMultiPos(){
    // Auto-generated destructor stub
}

// PUBLIC FUNCTIONS
void CSliderMultiPos::setNbValues(int nb){

    handles.clear();
    while( handles.size()<nb ){
        handles.append( CSliderMP_Handle(minimum()+2*handles.size(),this) );
    }
    if( maximum()<nb ){
        setMaximum( nb );
    }

    intervals.clear();
    while( intervals.size()<(nb-1) ){
        intervals.append( CSliderMP_Interval(this) );
        intervals.last().assignHandles( &handles[intervals.count()-1],
                                        &handles[intervals.count()]
                                      );
    }

    update();
}
void CSliderMultiPos::setSpanColor(int spanIndex,QColor color){
    if( spanIndex<intervals.count() ){
        intervals[spanIndex].setSpanColor(color);
    }
}
void CSliderMultiPos::valueIndication(bool enable){
    toolTipIndicateValue = enable;
}

int CSliderMultiPos::nbValues(void){
    return handles.count();
}
QColor CSliderMultiPos::getSpanColor(int spanIndex){

    if( spanIndex<intervals.count() ){
        return intervals.at(spanIndex).getSpanColor();
    }
    else{
        return this->palette().color( QPalette::Dark ).light( 110 );
    }
}

CSliderMultiPos::HandleMovementMode CSliderMultiPos::handleMovementMode() const{
    return movement;
}
void CSliderMultiPos::setHandleMovementMode(HandleMovementMode mode){
    movement = mode;
}

void CSliderMultiPos::displayPositionPermanent(QPoint offset){
    displayPosition = true;
    displayPositionOffset = offset;
}
void CSliderMultiPos::stopDisplayPosition(void){
    displayPosition = false;
}
void CSliderMultiPos::displayLabels(QString labelPrefix){
    displayIntervalLabel = true;
    intervalLabelPrefix = labelPrefix;
}
void CSliderMultiPos::stopDisplayLabels(void){
    displayIntervalLabel = false;
}
// PUBLIC SLOTS
void CSliderMultiPos::movePressedHandle(){

    if( handles.at(lastPressedIndex).isPosDifferOfValue() ){
        triggerAction( QAbstractSlider::SliderMove );
    }
}
void CSliderMultiPos::setValue(int posIndex,int value){

    if( posIndex>=0 && posIndex<nbValues() ){
        if( handles.at(posIndex).getValue()!=value ){
            handles[posIndex].setValue(value);
            emit valueChange(posIndex,value);
        }
    }
}
// PROTECTED FUNCTIONS
void CSliderMultiPos::keyPressEvent(QKeyEvent* event){
    SliderAction action = SliderNoAction;

    QSlider::keyPressEvent(event);
    switch( event->key() ){
        case Qt::Key_Left :
            action = !invertedAppearance() ? SliderSingleStepSub : SliderSingleStepAdd;
            break;
        case Qt::Key_Right :
            action = !invertedAppearance() ? SliderSingleStepAdd : SliderSingleStepSub;
            break;
        case Qt::Key_Up :
            action = invertedControls() ? SliderSingleStepSub : SliderSingleStepAdd;
            break;
        case Qt::Key_Down :
            action = invertedControls() ? SliderSingleStepAdd : SliderSingleStepSub;
            break;
        case Qt::Key_Home :
            action = SliderToMinimum;
            break;
        case Qt::Key_End :
            action = SliderToMaximum;
            break;
        default :
            event->ignore();
            break;
    }

    if( action ){
        triggerAction(action );
    }
}
void CSliderMultiPos::mousePressEvent(QMouseEvent* event){
    int elemIndex;

    if( minimum()==maximum() || (event->buttons()^event->button()) ){
        event->ignore();
        return;
    }
    previousMousePos = event->globalPos();
    for(elemIndex=0;elemIndex<nbValues();elemIndex++){
        if( handleMousePress( event->pos(),
                              handles.at(elemIndex).getPressed(),
                              minimum(),elemIndex ) ){
            break;
        }
    }

    event->accept();
}
void CSliderMultiPos::mouseMoveEvent(QMouseEvent* event){
    QStyleOptionSlider opt;

    previousMousePos = event->globalPos();
    if( lastPressedIndex==INVALID_INDEX ){
        event->ignore();
        return;
    }

    initStyleOption( &opt );
    const int m = style()->pixelMetric( QStyle::PM_MaximumDragDistance,&opt,this );
    int newPosition = pixelPosToRangeValue( pick( event->pos() ) - offset );
    if( m >= 0 ){
        const QRect r = rect().adjusted( -m,-m,m,m );
        if( !r.contains( event->pos() ) ){
            newPosition = tempPosition;
        }
    }

    newPosition = collisionDetection( newPosition );
    setCurPosition( lastPressedIndex, newPosition);

    event->accept();
    update();
}
void CSliderMultiPos::mouseReleaseEvent(QMouseEvent* event){

    QSlider::mouseReleaseEvent( event );
    previousMousePos = QPoint(0,0);
    setSliderDown( false );
    if( lastPressedIndex!=INVALID_INDEX ){
        setValue( lastPressedIndex,handles.at(lastPressedIndex).getPosition() );
        handles[lastPressedIndex].setPressed(QStyle::SC_None);
        lastPressedIndex = INVALID_INDEX;
    }

    update();
}
void CSliderMultiPos::paintEvent(QPaintEvent* event){
    int currentIndex;
    QStyleOptionSlider opt;

    Q_UNUSED(event);
    QStylePainter painter(this);
    initStyleOption(&opt);

    // ticks
    opt.subControls = QStyle::SC_SliderTickmarks;
    painter.drawComplexControl(QStyle::CC_Slider, opt);

    // groove
    opt.sliderValue = 0;
    opt.sliderPosition = 0;
    opt.subControls = QStyle::SC_SliderGroove;
    painter.drawComplexControl(QStyle::CC_Slider, opt);

    for( currentIndex=0;currentIndex<nbValues()-1;currentIndex++ ){
        // handle rects
        opt.sliderPosition = handles.at(currentIndex).getPosition();
        const QRect rectHandleFirst = style()->subControlRect(QStyle::CC_Slider, &opt, QStyle::SC_SliderHandle, this);
        const int lrv  = pick(rectHandleFirst.center());

        opt.sliderPosition = handles.at(currentIndex+1).getPosition();
        const QRect rectHandleSecond = style()->subControlRect(QStyle::CC_Slider, &opt, QStyle::SC_SliderHandle, this);
        const int urv  = pick(rectHandleSecond.center());

        // span
        const int minv = qMin(lrv, urv);
        const int maxv = qMax(lrv, urv);
        const QPoint c = style()->subControlRect(QStyle::CC_Slider, &opt, QStyle::SC_SliderGroove, this).center();
        QRect spanRect;
        if( orientation()==Qt::Horizontal){
            spanRect = QRect(QPoint(minv, c.y() - 2), QPoint(maxv, c.y() + 1));
        }
        else{
            spanRect = QRect(QPoint(c.x() - 2, minv), QPoint(c.x() + 1, maxv));
        }
        drawSpan(&painter, spanRect,currentIndex);
    }

    // handles
    for(int index=0;index<nbValues();index++){
        // Call drawHandle for all, with the last pressed index at last of call (for being on top)
        drawHandle( &painter, (lastPressedIndex+index+1)%nbValues() );
    }
    event->ignore();
}
void  CSliderMultiPos::setInitialValue(int index,int value){

    setCurPosition( index,value );
    setValue(index,value );
}
// PRIVATE FUNCTIONS
void CSliderMultiPos::initStyleOption(QStyleOptionSlider* option,int handleIndex){

    if( !option || handleIndex==INVALID_INDEX ){
        return;
    }

    option->initFrom(this);
    option->subControls = QStyle::SC_None;
    option->activeSubControls = QStyle::SC_None;
    option->orientation = this->orientation();
    option->maximum = this->maximum();
    option->minimum = this->minimum();
    option->tickPosition = this->tickPosition();
    option->tickInterval = this->tickInterval();
    option->direction = Qt::LeftToRight; // we use the upsideDown option instead
    option->singleStep = this->singleStep();
    option->pageStep = this->pageStep();
    if( this->orientation()==Qt::Horizontal ){
        option->upsideDown = (this->invertedAppearance() != (option->direction == Qt::RightToLeft));
        option->state |= QStyle::State_Horizontal;
    }
    else{
        option->upsideDown = (!this->invertedAppearance());
    }

    option->sliderPosition = handles.at(handleIndex).getPosition();
    option->sliderValue = handles.at(handleIndex).getValue(); // value before the mouse was pressed
}
int CSliderMultiPos::pick(const QPoint& pt){
    if( this->orientation()==Qt::Horizontal ){
        return pt.x();
    }
    else{
        return pt.y();
    }
}
void CSliderMultiPos::drawSpan(QStylePainter* painter, const QRect& rect,int baseElemIndex){
    QStyleOptionSlider opt;
    QColor currentColor;
    QPointF point1,point2;

    // area
    initStyleOption( &opt,baseElemIndex );
    QRect groove = this->style()->subControlRect( QStyle::CC_Slider,&opt,QStyle::SC_SliderGroove,this );

    // Apply color depending on span index
    // Considering the base element index is the interval index
    currentColor = getSpanColor(baseElemIndex);

    if( opt.orientation == Qt::Horizontal ){
        groove.adjust( 0,0,-1,0 );
        point1.setX( groove.center().x() );
        point1.setY( groove.top() );
        point2.setX( groove.center().x() );
        point2.setY( groove.bottom() );
    }
    else{
        groove.adjust( 0,0,0,-1 );
        point1.setX( groove.left() );
        point1.setY( groove.center().y() );
        point2.setX( groove.right() );
        point2.setY( groove.center().y() );
    }
    setupPainter( painter,point1,point2,currentColor );

    // draw groove
    painter->drawRect( rect );//.intersected( groove ) );

    if( displayPosition ){
        QPoint endOfSpan;
        if( this->tickPosition()==TicksBelow ){
            endOfSpan = rect.bottomLeft();
        }
        else{
            endOfSpan = rect.topLeft();
        }
        drawSpanValue( painter,endOfSpan,baseElemIndex );
        if( baseElemIndex==nbValues()-2 ){
            if( this->tickPosition()==TicksBelow ){
                endOfSpan = rect.bottomRight();
            }
            else{
                endOfSpan = rect.topRight();
            }
            drawSpanValue( painter,endOfSpan,baseElemIndex+1 );
        }
    }

    if( displayIntervalLabel ){
        drawSpanLabel( painter,rect,baseElemIndex );
    }

}
void CSliderMultiPos::drawSpanValue(QStylePainter* painter,QPoint origin,int elemIndex){
    origin += displayPositionOffset;
    if( (origin.x()+10)>=this->rect().width() ){
        origin.setX( origin.x()-15 );
    }
    painter->drawText( origin,QString::number(handles.at(elemIndex).getValue()) );
}
void CSliderMultiPos::drawSpanLabel(QStylePainter* painter,QRect rect,int baseElemIndex){

    setupPainter( painter,rect.topLeft(),rect.bottomRight(),
                        this->palette().color( QPalette::Dark ).light( 50 ) );
    rect.setHeight(25);
    if( this->tickPosition()==TicksBelow ){
        rect.translate(0,-20);
    }
    painter->drawText( rect,Qt::AlignCenter,intervalLabelPrefix+QString::number(baseElemIndex+1) );
}
void CSliderMultiPos::drawHandle(QStylePainter* painter,int handleIndex){
    QStyleOptionSlider opt;

    initStyleOption( &opt,handleIndex);
    opt.subControls = QStyle::SC_SliderHandle;

    if( handles.at(handleIndex).getPressed()==QStyle::SC_SliderHandle ){
        opt.activeSubControls = QStyle::SC_SliderHandle;
        opt.state |= QStyle::State_Sunken;

        if( toolTipIndicateValue && previousMousePos!=QPoint(0,0) ){
            QToolTip::showText( previousMousePos,
                                    QString::number(handles.at(handleIndex).getPosition()),
                                    this );
        }
    }
    painter->drawComplexControl( QStyle::CC_Slider,opt );

}
void CSliderMultiPos::setupPainter(QPainter* painter,QPointF point1,QPointF point2,QColor color){

    QLinearGradient gradient(point1,point2);
    gradient.setColorAt(0, color.dark(120));
    gradient.setColorAt(1, color.light(108));
    painter->setBrush(gradient);

    if( this->orientation()==Qt::Horizontal) {
        painter->setPen(QPen(color.dark(130), 0));
    }
    else{
        painter->setPen(QPen(color.dark(150), 0));
    }
}
void CSliderMultiPos::triggerAction(QAbstractSlider::SliderAction action){
    bool noAction = false;
    int newPos = 0;
    int minPos;
    int maxPos;

    if( lastPressedIndex==0 ){
        minPos = minimum();
    }
    else{
        minPos = handles.at(lastPressedIndex-1).getPosition();
    }
    if( lastPressedIndex==(nbValues()-1) ){
        maxPos = maximum();
    }
    else{
        minPos = handles.at(lastPressedIndex-1).getPosition();
    }

    blockTracking = true;
    switch( action ){
        case QAbstractSlider::SliderSingleStepAdd :
            newPos = qBound( minPos,minPos + singleStep(),maxPos );
            break;
        case QAbstractSlider::SliderSingleStepSub :
            newPos = qBound( minPos,maxPos - singleStep(),maxPos );
            break;
        case QAbstractSlider::SliderToMinimum :
            newPos = minPos;
            break;
        case QAbstractSlider::SliderToMaximum :
            newPos = maxPos;
            break;
        case QAbstractSlider::SliderMove :
        case QAbstractSlider::SliderNoAction :
            noAction = true;
            break;
        default :
            qWarning( "QxtSpanSliderPrivate::triggerAction: Unknown action" );
            break;
    }

    if( noAction ){
        // Nothing to do for this action
    }
    else{
        newPos = collisionDetection(newPos);
        setCurPosition( lastPressedIndex,newPos );
    }
    blockTracking = false;
}
bool CSliderMultiPos::handleMousePress(const QPoint& pos, QStyle::SubControl control,
                                            int value,int handleIndex){

    QStyleOptionSlider opt;
    bool hasFind;

    hasFind = false;
    initStyleOption( &opt,handleIndex);//handle );
    const QStyle::SubControl oldControl = control;
    control = this->style()->hitTestComplexControl( QStyle::CC_Slider,&opt,pos,this );
    const QRect sr = this->style()->subControlRect( QStyle::CC_Slider,&opt,QStyle::SC_SliderHandle,this );
    if( control == QStyle::SC_SliderHandle ){
        tempPosition = value;
        offset = pick( pos - sr.topLeft() );
        lastPressedIndex = handleIndex;
        hasFind = true;
        this->setSliderDown( true );
    }
    if( control!=oldControl ){
        this->update( sr );
    }
    return hasFind;
}
int CSliderMultiPos::pixelPosToRangeValue(int pos) {

    QStyleOptionSlider opt;
    initStyleOption( &opt );

    int sliderMin = 0;
    int sliderMax = 0;
    int sliderLength = 0;

    const QRect gr = this->style()->subControlRect( QStyle::CC_Slider,&opt,QStyle::SC_SliderGroove,this );
    const QRect sr = this->style()->subControlRect( QStyle::CC_Slider,&opt,QStyle::SC_SliderHandle,this );
    if( this->orientation() == Qt::Horizontal ){
        sliderLength = sr.width();
        sliderMin = gr.x();
        sliderMax = gr.right() - sliderLength + 1;
    }
    else{
        sliderLength = sr.height();
        sliderMin = gr.y();
        sliderMax = gr.bottom() - sliderLength + 1;
    }
    return QStyle::sliderValueFromPosition( minimum(),maximum(),pos - sliderMin,sliderMax - sliderMin,
            opt.upsideDown );
}

void CSliderMultiPos::setCurPosition(int posIndex,int pos){

    if( handles.at(posIndex).getPosition()!=pos ){
        handles[posIndex].setPosition(pos);
        if( !hasTracking() ){
            update();
        }
        if( isSliderDown() ){
            // Could emit a signal
        }
        if( hasTracking() && !blockTracking ){
            triggerAction( SliderMove );
        }
    }
}
int CSliderMultiPos::collisionDetection(int newPos){
    int posModified;

    posModified = newPos;
    if( lastPressedIndex>0 ){
        switch( movement ){
            case NoCrossing :
                if( newPos<handles.at(lastPressedIndex-1).getPosition() ){
                    posModified = handles.at(lastPressedIndex-1).getPosition();
                }
                break;
            case NoOverlapping :
                if( newPos<=handles.at(lastPressedIndex-1).getPosition() ){
                      posModified = handles.at(lastPressedIndex-1).getPosition()+1;
                }
                break;
            case FreeMovement :
                if( newPos<handles.at(lastPressedIndex-1).getPosition() ){
                    setCurPosition( lastPressedIndex,handles.at(lastPressedIndex-1).getPosition() );
                    setValue( lastPressedIndex,handles.at(lastPressedIndex-1).getPosition() );
                    handles[lastPressedIndex].setPressed(QStyle::SC_None);
                    lastPressedIndex--;
                    handles[lastPressedIndex].setPressed(QStyle::SC_SliderHandle );
                }
                break;
        }
    }
    else{
        if( newPos<minimum() ){
            posModified = minimum();
        }
    }

    if( lastPressedIndex<nbValues()-1 ){
        switch( movement ){
            case NoCrossing :
                if( newPos>handles.at(lastPressedIndex+1).getPosition() ){
                    posModified = handles.at(lastPressedIndex+1).getPosition();
                }
                break;
            case NoOverlapping :
                if( newPos>=handles.at(lastPressedIndex+1).getPosition() ){
                    posModified = handles.at(lastPressedIndex+1).getPosition()-1;
                }
                break;
            case FreeMovement :
                if( newPos>handles.at(lastPressedIndex+1).getPosition() ){
                    setCurPosition( lastPressedIndex,handles.at(lastPressedIndex+1).getPosition() );
                    setValue( lastPressedIndex,handles.at(lastPressedIndex+1).getPosition() );
                    handles[lastPressedIndex].setPressed( QStyle::SC_None );
                    lastPressedIndex++;
                    handles[lastPressedIndex].setPressed( QStyle::SC_SliderHandle );
                }
                break;
        }
    }
    else{
        if( newPos>maximum() ){
            posModified = maximum();
        }
    }

    return posModified;
}
