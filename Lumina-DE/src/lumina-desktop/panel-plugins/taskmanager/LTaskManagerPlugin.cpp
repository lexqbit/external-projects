#include "LTaskManagerPlugin.h"

LTaskManagerPlugin::LTaskManagerPlugin(QWidget *parent) : LPPlugin(parent, "taskmanager"){
  timer = new QTimer(this);
	timer->setInterval(250); // 1/4 second
	connect(timer, SIGNAL(timeout()), this, SLOT(UpdateButtons()) ); 
  connect(LSession::instance(), SIGNAL(WindowListEvent()), this, SLOT(checkWindows()) );
  this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
  this->layout()->setAlignment(Qt::AlignLeft);
}

LTaskManagerPlugin::~LTaskManagerPlugin(){
	
}

//==============
//    PRIVATE SLOTS
//==============
void LTaskManagerPlugin::UpdateButtons(){
  //Get the current window list
  QList<WId> winlist = LX11::WindowList();
  qDebug() << "Update Buttons:" << winlist;
  //Now go through all the current buttons first
  for(int i=0; i<BUTTONS.length(); i++){
    //Get the windows managed in this button
    QList<LWinInfo> WI = BUTTONS[i]->windows();
    bool updated=false;
    for(int w=0; w<WI.length(); w++){
      if( winlist.contains( WI[w].windowID() ) ){
        //Still current window - update it later
	winlist.removeAll(WI[w].windowID()); //remove this window from the list since it is done
      }else{
	//Window was closed - remove it
	if(WI.length()==1){
	  //Remove the entire button
	  this->layout()->takeAt(i); //remove from the layout
	  delete BUTTONS.takeAt(i);
	  i--;
	  updated = true; //prevent updating a removed button
	  break; //break out of the button->window loop
	}else{
	  BUTTONS[i]->rmWindow(WI[w]); // one of the multiple windows for the button
	  WI.removeAt(w); //remove this window from the list
	  w--;
	}
	updated=true; //button already changed
      }
    }
    if(!updated){
      QTimer::singleShot(1,BUTTONS[i], SLOT(UpdateButton()) ); //keep moving on
    }
  }
  //Now go through the remaining windows
  for(int i=0; i<winlist.length(); i++){
    //New windows, create buttons for each (add grouping later)
    LTaskButton *but = new LTaskButton(this);
      but->addWindow( LWinInfo(winlist[i]) );
    this->layout()->addWidget(but);
    BUTTONS << but;
  }
}

void LTaskManagerPlugin::checkWindows(){
  timer->start();
}