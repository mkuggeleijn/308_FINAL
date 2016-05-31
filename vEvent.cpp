#include "vEvent.h"

// vEvent
vEvent::vEvent(vVertexPoint* vpoint, bool vplaceEvent) {
	point = vpoint;
	// yValue = vpoint.y;
	placeEvent = vplaceEvent;
	arc = 0;
}



vEvent::~vEvent() {}