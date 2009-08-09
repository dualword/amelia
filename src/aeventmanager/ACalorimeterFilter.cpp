#include "ACalorimeterFilter.h"
#include <QDebug>
#include "AShower.h"

ACalorimeterFilter::ACalorimeterFilter(AEventObjectFilter *next)
  :AEventObjectFilter(next)
{ }

bool ACalorimeterFilter::check(AEventObject* object)
{
  if(object->type()==AEventObject::eShower)
    {
      AShower *shower=(AShower*)object;
      
      //Currently support only these two calorimeters...
      if(shower->calorimeter()!="LAr" && shower->calorimeter()!="TILE")
	return false;
      
      //Noise filter, should we put this into the event loader?
      if(shower->energy()<1)
	return false;
    }
  
  return AEventObjectFilter::check(object);
}

