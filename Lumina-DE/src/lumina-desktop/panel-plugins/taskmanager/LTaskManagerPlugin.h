#ifndef _LUMINA_DESKTOP_TASK_MANAGER_PLUGIN_H
#define _LUMINA_DESKTOP_TASK_MANAGER_PLUGIN_H

// Qt includes
#include <QWidget>
#include <QList>
#include <QString>
#include <QDebug>
#include <QTimer>
#include <QEvent>

// libLumina includes
#include <LuminaX11.h>

// Local includes
#include "LTaskButton.h"
#include "LWinInfo.h"
#include "../LPPlugin.h"
#include "../../LSession.h" //keep this last

class LTaskManagerPlugin : public LPPlugin{
	Q_OBJECT
public:
	LTaskManagerPlugin(QWidget *parent=0);
	~LTaskManagerPlugin();

private:
	QList<LTaskButton*> BUTTONS; //to keep track of the current buttons
	QTimer *timer;

private slots:
	void UpdateButtons();
	void checkWindows();

public slots:
	void LocaleChange(){
	  UpdateButtons();
	}
	void ThemeChange(){
	  UpdateButtons();
	}

};
#endif