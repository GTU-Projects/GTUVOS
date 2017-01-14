#include "helpcmd.h"
#include "command.h"
#include "QApplication"
#include "gtuvos.h"

#include <iostream>
using namespace  std;


HelpCMD::~HelpCMD(){
}

HelpCMD::HelpCMD(QStringList params):ICommand(params)
{
    cout<<"HelpCmd constructed!!"<<endl;
}

void HelpCMD::execute(Ui::MainWindow *mainWindow){
    cout<<"Help execute started."<<endl;

    if(mParams.size()!=1){
        printTerm(mainWindow,"Invalid help parameter","red");
        printTerm(mainWindow,"Help command no use parameter","DeepSkyBlue");
        return;
    }

    // Information for running the command
    printTerm(mainWindow,"<p style=\"white-space:pre;\">--- Help Manual ---</p>","lime");
    printTerm(mainWindow,"<p style=\"white-space:pre;\">The available commands are:</p>","lime");
    printTerm(mainWindow,"<p style=\"white-space:pre;\"> - help<br>"
                         " - cp file_source file_destination behavior(optional)<br>"
                         " - mail list<br>"
                         " - mail send \"to\" \"title\" \"message\"<br>"
                         " - ls - path (path is optional)<br>"
                         " - touch path<br>"
                         " - mkdir path<br>"
                         " - man command<br>"
                         " - clear<br>"
                         " - exit<br>"
                         " - ftp user:pass@ipadd/filepath sendFilePath</p><br>","DeepSkyBlue");
}
