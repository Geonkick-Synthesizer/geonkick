#include "oscillator_widget.h"
#include "gkickapi.h"
#include "mainwindow.h"

#include <QApplication>
#include <QDebug>
#include <QPushButton>
#include <QVector>

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);

  qDebug() << "Hello Geonkick";


  /*  struct geonckick *kick;

  kick = gkick_create();
  if (kick == NULL) {
	  qDebug() << "error: can't create geonckick";
	  exit(1);
  }
  gkick_set_opt_app_name(kick, "GeonKick");
  gkick_start(kick);*/
  
  //std::vector<std::unique_ptr<GKickOscillator>> ocillators = kickApi.getOscillators();

   MainWindow window;
  if (!window.init()) {
    qDebug() << "can't init main window";
    exit(1);
  }
  
  window.show();
  //OscillatorWidget plotWindow(NULL, ocillators[0].get());
  //plotWindow.show();
  //  gkick_free(kick);
  
  return a.exec();
}
