#include "gkickapi.h"
#include "gkickoscillator.h"

#include <geonkick.h>

#include <QDebug>

GKickApi::GKickApi() :
  isError(false),
  gKickApi(NULL)
{
  	if (geonkick_create(&gKickApi) != GEONKICK_OK) {
  		setError(true);
  	}
}

GKickApi::~GKickApi()
{
  	if (gKickApi) {
  	   geonkick_free(&gKickApi);
  	}
}

void GKickApi::setError(bool b)
{
  isError = b;
}

QVector<GKickOscillator*> GKickApi::getOscillators(void)
{
  QVector<GKickOscillator*> oscillators;
//
//  int n = 0;//geonkick_get_oscllators_number(&gKickApi);
//  for (int i = 0; i < n; i++) {
//    //oscillators << GKickOscillator(this, i);
//  }
//
	return oscillators;
}

QPolygonF GKickApi::getOscEvelopePoints(int osc, int envelope)
{
  double *buf;
  QPolygonF points;
  size_t npoints = 0;

  qDebug() << "GKickApi::getOscEvelopePoints(int osc, int envelope)";
  //geonkick_osc_envelope_get_points(osc, envelope, &buf, &npoints);

  for (size_t i = 0; i < 10/*2 * npoints*/; i += 2) {
    points << QPointF(100, 100/*buf[i], buf[i+1]*/);
  }

  return points;
}


void GKickApi::addOscEnvelopePoint(int osc,
				   int evenlope,
				   const QPointF &point)
{
    qDebug() << "GKickApi::addOscEnvelopePoint";

  //geonkick_osc_envelope_add_point(gkickApi,
  //				  osc,
  //				  envelope,
  //				  point.(), point.y());
}

void GKickApi::removeOscEvelopePoint(int osc,
				    int envelope,
				    int index)
{
  qDebug() << "GKickApi::removeOscEvelopePoint";

  //geonkick_osc_envelope_remove_point(gkickApi, osc, envelope, index);
}

void GKickApi::updateOscEvelopePoint(int osc, int envelope,
				   int index, const QPointF &point)
{
    qDebug() << "GKickApi::updateOscEvelopePoint";
  //geonkick_osc_envelope_update_point(gkickApi, osc, envelope,
  //				     index, point.x(), point.y());  
}


