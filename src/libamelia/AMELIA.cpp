;
#include "AMELIA.h"

#include <QDir>
#include <QDebug>
#include <QPluginLoader>
#include <QRegExp>
#include <QDomDocument>

#if defined(Q_WS_X11)
QString AMELIA::libExtension="so";
#elif defined(Q_WS_MAC)
QString AMELIA::libExtension="dylib";
#elif defined(Q_WS_WIN)
QString AMELIA::libExtension="dll";
#endif

AMELIA* AMELIA::global=0;

AMELIA::AMELIA(QObject *parent):QObject(parent)
{
  global=this;
}

APlugin* AMELIA::plugin(QString plugin)
{
  return plugins[plugin];
}

bool AMELIA::isLoaded(QString name)
{
  return (plugins.keys().contains(name));
}

bool AMELIA::loadPlugin(QString name)
{
  if(isLoaded(name)) return true;

  //OS dependent library name
#ifdef Q_WS_WIN
  QString file=name+"."+libExtension;
#else
  QString file="lib"+name+"."+libExtension;
#endif

  QPluginLoader loader("plugins:"+file,this);
  loader.setLoadHints(QLibrary::ExportExternalSymbolsHint);

  QDomDocument doc("plugindoc");
  QFile pfile(":/"+name.toLower()+"/plugin.xml");
  if (pfile.open(QIODevice::ReadOnly) && doc.setContent(&pfile)) {
    QDomElement root=doc.documentElement();
    QDomNodeList depsNodes=root.elementsByTagName("dependencies");

    for(int i=0;i<depsNodes.size();i++)
      {
	QDomElement depsNode=depsNodes.at(i).toElement();
	QDomNodeList pluginDepNodes=depsNode.elementsByTagName("plugin");

	for(int j=0;j<pluginDepNodes.size();j++)
	  {
	    QDomElement pluginDep=pluginDepNodes.at(j).toElement();
	    QString depName=pluginDep.attribute("name");
	    bool optional=(pluginDep.attribute("optional","false")=="true");
	    qDebug() << "Name " << depName << " Optional " << optional;
	    if(!loadPlugin(depName) && !optional) return false;
	  }
      }
  }
  pfile.close();

  qDebug() << "Loading " << name;
  APlugin *plugin=qobject_cast<APlugin *>(loader.instance());
  if(!plugin)
    {
      qDebug() << plugin << " " << loader.errorString();
      return false;
    }

  plugins[name]=plugin;
  plugin->setPluginBase(this);
  plugin->load();

  emit pluginLoaded(name);
  return true;
}

struct APluginInfoTmp
{
  QString name;
  int priority;
};

void AMELIA::loadAllPlugins()
{
  QList<APluginInfoTmp> pluginOrder;

  for(int i=0;i<searchPaths.size();i++)
    {
      QDir path(searchPaths[i]);
      QStringList plugins=path.entryList(QDir::Files);
      for(int j=0;j<plugins.size();j++)
	{
	  QString file=plugins[j];
#ifdef Q_WS_WIN
	  QRegExp re("(.*)\\."+libExtension+"$");
#else
	  QRegExp re("lib(.*)\\."+libExtension);
#endif
	  if(re.indexIn(file)>=0)
	    {
	      qDebug() << "Found plugin " << file;

	      QString plugin=re.cap(1);

	      QPluginLoader loader("plugins:"+file,this);
	      loader.setLoadHints(QLibrary::ExportExternalSymbolsHint);
	      loader.load();

	      QDomDocument doc("plugindoc");
	      APluginInfoTmp info;
	      info.name=plugin;
	      info.priority=5;


	      QFile file(":/"+plugin.toLower()+"/plugin.xml");
	      if (file.open(QIODevice::ReadOnly) && doc.setContent(&file)) {
		QDomElement root=doc.documentElement();
		QString priority=root.attribute("priority","5");

		info.priority=priority.toInt();
	      }
	      file.close();
	      qDebug() << "\tPriority " << info.priority;

	      pluginOrder.push_back(info);
	    }
	}

      for(int i=0;i<pluginOrder.size()-1;i++)
	{
	  for(int j=i+1;j<pluginOrder.size();j++)
	    {
	      if(pluginOrder[i].priority>=pluginOrder[j].priority)
		{
		  pluginOrder.swap(i,j);
		}
	    }
	}

      qDebug() << "-- Ordered --";
      for(int j=0;j<pluginOrder.size();j++)
	{
	  loadPlugin(pluginOrder[j].name);
	}
    }

  emit allPluginsLoaded();
}

void AMELIA::addSearchPath(QString path)
{
  searchPaths.push_back(path);
  QDir::addSearchPath("plugins",path);
}
