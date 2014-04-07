#include "LTaskButton.h"

LTaskButton::LTaskButton(QWidget *parent) : LTBWidget(parent){
  actMenu = new QMenu(this);
  winMenu = new QMenu(this);
  UpdateMenus();
  
  this->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
  this->setAutoRaise(false); //make sure these always look like buttons
  this->setContextMenuPolicy(Qt::CustomContextMenu);
  winMenu->setContextMenuPolicy(Qt::CustomContextMenu);
  connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(openActionMenu()) );
  connect(winMenu, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(openActionMenu()) );
  connect(winMenu, SIGNAL(triggered(QAction*)), this, SLOT(winClicked(QAction*)) );
}

LTaskButton::~LTaskButton(){
	
}

//===========
//      PUBLIC
//===========
QList<LWinInfo> LTaskButton::windows(){
  return WINLIST;
}

void LTaskButton::addWindow(LWinInfo win){
  WINLIST << win;
  UpdateButton();
}

void LTaskButton::rmWindow(LWinInfo win){
  for(int i=0; i<WINLIST.length(); i++){
    if(WINLIST[i].windowID() == win.windowID()){
      WINLIST.removeAt(i);
      break;
    }
  }
  UpdateButton();
}

//==========
//    PRIVATE
//==========
LWinInfo LTaskButton::currentWindow(){
  if(WINLIST.length() == 1 || cWin.windowID()==0){
    return WINLIST[0]; //only 1 window - this must be it
  }else{
    return cWin;
  }
}

//=============
//   PUBLIC SLOTS
//=============
void LTaskButton::UpdateButton(){
  winMenu->clear();
  Lumina::STATES showstate = Lumina::NOSHOW;
  for(int i=0; i<WINLIST.length(); i++){
    if(WINLIST[i].windowID() == 0){
      WINLIST.removeAt(i);
      i--;
      continue;
    }
    if(i==0){
      //Update the button visuals from the first window
      this->setIcon(WINLIST[i].icon());
      this->setText(WINLIST[i].Class());
    }
    winMenu->addAction( WINLIST[i].icon(), WINLIST[i].text() );
    Lumina::STATES stat = WINLIST[i].status();
    if(showstate!=Lumina::NOTIFICATION){ 
	if(showstate!=Lumina::VISIBLE && stat!=Lumina::NOSHOW){ showstate = stat; } //this is alwasy an improvement
	else if(stat ==Lumina::NOTIFICATION){ showstate = stat; } //only notification is higher than visible
    }
  }
  this->setState(showstate);
  //Now setup the button appropriately
  // - visibility
  if(showstate == Lumina::NOSHOW || WINLIST.length() < 1){ this->setVisible(false); }
  else{ this->setVisible(true); }
  // - functionality
  disconnect(this, SIGNAL(clicked())); //remove some button connections for a moment
  if(WINLIST.length() == 1){
    //single window
    this->setPopupMode(QToolButton::MenuButtonPopup);
    this->setMenu(actMenu);
    //this->setText("");
    //this->setToolButtonStyle(Qt::ToolButtonIconOnly);
    connect(this, SIGNAL(clicked()), this, SLOT(triggerWindow()) );
  }else if(WINLIST.length() > 1){
    //multiple windows
    this->setPopupMode(QToolButton::InstantPopup);
    this->setMenu(winMenu);
    this->setText(QString::number(WINLIST.length()) );
    //this->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
  }
}

void LTaskButton::UpdateMenus(){
  //Action menu is very simple right now - can expand it later
  actMenu->clear();
  actMenu->addAction( LXDG::findIcon("window-close",""), tr("Close Window"), this, SLOT(closeWindow()) );
}

//=============
//   PRIVATE SLOTS
//=============
void LTaskButton::closeWindow(){
  LWinInfo win = currentWindow();
  LX11::CloseWindow(win.windowID());
  cWin = LWinInfo(); //clear the current
}

void LTaskButton::triggerWindow(){
  LWinInfo win = currentWindow();
  //Check which state the window is currently in and flip it to the other
  if(LX11::GetWindowState(win.windowID()) == LX11::VISIBLE ){
    LX11::IconifyWindow(win.windowID());
  }else{
    LX11::RestoreWindow(win.windowID());
  }
  cWin = LWinInfo(); //clear the current
}

void LTaskButton::winClicked(QAction* act){
  //Get the window from the action
  QString txt = act->text();
  for(int i=0; i<WINLIST.length(); i++){
    if(WINLIST[i].text() == txt){ cWin = WINLIST[i]; }
  }
  //Now trigger the window
  triggerWindow();
}

void LTaskButton::openActionMenu(){
  //Get the Window the mouse is currently over
  QAction *act = winMenu->actionAt(QCursor::pos());
  if( act != 0 && winMenu->isVisible() ){
    //get the window from the action
    QString txt = act->text();
    for(int i=0; i<WINLIST.length(); i++){
      if(WINLIST[i].text() == txt){ cWin = WINLIST[i]; }
    }
  }
  //Now show the action menu
  actMenu->popup(QCursor::pos());
}
