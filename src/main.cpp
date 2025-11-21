#include <QtWidgets\qapplication.h>
#include <QtWidgets\qstylefactory.h>
#include <QtWidgets\qdialog.h>


#include <InstantMenuStyle.h>
#include "StartupDialog.h"
#include "FilePathHandler.h"
#include "FileManager.h"
#include "MainApp.h"

#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>
int main(int argc, char* argv[]) {
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    QApplication app(argc, argv);
    app.setStyle(new InstantMenuStyle(QStyleFactory::create("")));
    
    FilePathHandler* filePathHandler = nullptr;
    DM::FileManager* fileManager = new DM::FileManager();

    StartupDialog startupBox = StartupDialog(filePathHandler);
    if (startupBox.exec() != QDialog::Accepted) {
        delete filePathHandler;
        return 0;
    }

    MainApp mainApp = MainApp(app, filePathHandler, fileManager);

    delete filePathHandler;
    delete fileManager;
    return app.exec();

   
}
