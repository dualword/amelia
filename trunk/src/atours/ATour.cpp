#include "ATour.h"

#include <QFile>
#include <QDebug>

ATour::ATour(QString path)
  :_location(path)
{
  loadTour();
}

QString ATour::title()
{
  return _title;
}

QString ATour::description()
{
  return _description;
}

ATourBlock* ATour::block(int idx)
{
  return blocks[idx];
}

int ATour::blockCount()
{
  return blocks.size();
}

void ATour::loadTour()
{
  QFile fh(_location);
  QDomDocument doc("tour");
  if (!fh.open(QIODevice::ReadOnly))
    return;
  if (!doc.setContent(&fh))
    {
      fh.close();
      return;
    }
  fh.close();
  
  QDomElement amtourElement=doc.namedItem("amtour").toElement();
  
  _title=amtourElement.attribute("title");
  _description=amtourElement.attribute("description");
  
  QDomElement blockNode=amtourElement.firstChildElement("block");

  while(!blockNode.isNull())
    {
      loadBlock(blockNode);
      blockNode=blockNode.nextSiblingElement("block");
    }
}

void ATour::loadBlock(QDomElement blockElement)
{
    ATourBlock* block = new ATourBlock();
    block->loadBlock(blockElement);
    blocks.push_back( block );
}
