#ifndef _LUMINA_OPEN_FILE_DIALOG_H
#define _LUMINA_OPEN_FILE_DIALOG_H

#include <QSettings>
#include <QDialog>
#include <QString>
#include <QStringList>
#include <QIcon>
#include <QFileDialog>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QSettings>
#include <QTextStream>
#include <QTreeWidgetItem>

#include "LuminaXDG.h" //From libLuminaUtils

namespace Ui{
	class LFileDialog;
};

class LFileDialog : public QDialog{
	Q_OBJECT
public:
	LFileDialog(QWidget *parent = 0);
	~LFileDialog();
	
	//inputs
	void setFileInfo(QString filename, QString extension);
	
	//outputs
	bool appSelected, setDefault;
	QString appExec;
	QString appPath;
	QString appFile;
	
	//static functions
	static QString getDefaultApp(QString extension);
	static void setDefaultApp(QString extension, QString appFile);
	
private:
	Ui::LFileDialog *ui;
	QString fileEXT;
	QSettings *settings;
	
	//DB set/get
	QStringList getPreferredApplications();
	void setPreferredApplication(QString);
	
	QString translateCat(QString);
	
private slots:
	void updateUI();
	void generateAppList();
	//Internal UI slots
	void on_group_binary_toggled(bool checked);
	void on_tool_ok_clicked();
	void on_tool_cancel_clicked();
	void on_tool_findBin_clicked();
	void on_line_bin_textChanged();
	
};

#endif
