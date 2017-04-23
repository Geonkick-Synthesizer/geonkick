#include "oscillator_widget.h"
#include "gkickapi.h"

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
  
  GKickApi kickApi;

  std::vector<std::unique_ptr<GKickOscillator>> socillators = kickApi.getOscillators();
   
  OscillatorWidget plotWindow(NULL, socillators[0].get());
  plotWindow.show();
  //  gkick_free(kick);
  
  return a.exec();
}
