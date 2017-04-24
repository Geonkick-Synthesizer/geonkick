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

std::vector<std::unique_ptr<GKickOscillator>> GKickApi::getOscillators(void)
{
  std::vector<std::unique_ptr<GKickOscillator>> oscillators;
  
  int n = geonkick_get_oscillators_number(gKickApi);
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

  qDebug() << "GKickApi::getOscEvelopePoints(int osc, int envelope)";
  geonkick_osc_envelope_get_points(gKickApi, osc, envelope, &buf, &npoints);

  for (size_t i = 0; i < npoints; i += 2) {
    points << QPointF(buf[i], buf[i+1]);
  }
  free(buf);

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


