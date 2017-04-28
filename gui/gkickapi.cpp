#include "gkickapi.h"
#include "gkickoscillator.h"
#include <memory>

#include <geonkick.h>

#include <QDebug>

GKickApi::GKickApi() :
  isError(false),
  gKickApi(NULL)
{
  	if (geonkick_create(&gKickApi) != GEONKICK_OK) {
  		setError(true);
  	}
	qDebug() << "gKickApi:: end constructor";
}

GKickApi::~GKickApi()
{
  qDebug() << "gKickApi:: destrcutor";

  	if (gKickApi) {
	    qDebug() << "gKickApi:: destrcutor[0]";
  	   geonkick_free(&gKickApi);
  	}
}

void GKickApi::setError(bool b)
{
  isError = b;
}

bool GKickApi::hasErrors(void)
{
  return isError;
}

std::vector<std::unique_ptr<GKickOscillator>> GKickApi::getOscillators(void)
{ 
  std::vector<std::unique_ptr<GKickOscillator>> oscillators;

  if (!gKickApi) {
    qDebug() << "no points";
    return oscillators;
  }
    qDebug() << "get points points";  
  int n = geonkick_get_oscillators_number(gKickApi);
  qDebug() << "N oscialltors:" << n;  
  for (int i = 0; i < n; i++) {
    oscillators.push_back(std::make_unique<GKickOscillator>(this, i));
  }

  return oscillators;
}

QPolygonF GKickApi::getOscEvelopePoints(int osc, int envelope)
{
  double *buf;
  QPolygonF points;
  size_t npoints = 0;

  if (!gKickApi) {
    return points;
  }

  qDebug() << "GKickApi::getOscEvelopePoints(int osc, int envelope)";
  geonkick_osc_envelope_get_points(gKickApi, osc, envelope, &buf, &npoints);
  qDebug() << "points" << npoints;

  for (size_t i = 0; i < 2 * npoints; i += 2) {
    qDebug() << "API:" << buf[i] << ", " << buf[i+1];
    points.push_back(QPointF(buf[i], buf[i+1]));
  }

  if (buf != NULL) {
    free(buf);
  }

  return points;
}


void GKickApi::addOscEnvelopePoint(int osc,
				   int envelope,
				   const QPointF &point)
{
    qDebug() << "GKickApi::addOscEnvelopePoint";

    if (!gKickApi) {
      return;
    }
    
  geonkick_osc_envelope_add_point(gKickApi,
  				  osc,
  				  envelope,
  				  point.x(), point.y());
}

void GKickApi::removeOscEvelopePoint(int osc,
				    int envelope,
				    int index)
{
  qDebug() << "GKickApi::removeOscEvelopePoint";
    if (!gKickApi) {
      return;
    }

  
  geonkick_osc_envelope_remove_point(gKickApi, osc, envelope, index);
}

void GKickApi::updateOscEvelopePoint(int osc, int envelope,
				   int index, const QPointF &point)
{
      if (!gKickApi) {
      return;
    }

    qDebug() << "GKickApi::updateOscEvelopePoint";
    geonkick_osc_envelope_update_point(gKickApi, osc, envelope,
  				     index, point.x(), point.y());  
}


