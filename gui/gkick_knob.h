#ifndef GKICK_KNOB_H
#define GKICK_KNOB_H

#include <QWidget>
#include <QMouseEvent>

#define GKICK_UI_DEFAULT_KNOB_RADIOUS 30

class GKickKnob : public QWidget
{
   Q_OBJECT
public:
   GKickKnob(QWidget *parent);
   virtual ~GKickKnob();
   int getRadius(void);
   int getWidth(void);
   int getHeight(void);
   double getPosition(void);
   double getValue(void);
   
   public slots:
     void setPosition(double v);
     void setValue(double v);
	  
 signals:
     void valueUpdated(double v);

 protected:
   void paintEvent(QPaintEvent *event);
   void mousePressEvent(QMouseEvent *event);
   void mouseReleaseEvent(QMouseEvent *event);
   void mouseDoubleClickEvent(QMouseEvent *event);
   void mouseMoveEvent(QMouseEvent *event);
   void resizeEvent(QResizeEvent *event);

 private:
   int knobRadius;
   QPoint lastPositionPoint;
   int knobValueDegree;
   double realValue;
   bool isSelected;
};

#endif
