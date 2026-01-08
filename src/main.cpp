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
    QApplication::setApplicationName("PolybiusTools");
    QApplication::setApplicationVersion("0.1.0");
    QApplication::setOrganizationName("Arzyel");
    QApplication::setOrganizationDomain("https://github.com/Arzyel/PolybiusTools");
    app.setWindowIcon(QIcon("GFX/icon/PolybiusToolsLogo.png"));


    app.setStyle(new InstantMenuStyle(QStyleFactory::create("")));
    
    FilePathHandler* filePathHandler = nullptr;

    StartupDialog startupBox = StartupDialog(filePathHandler);
    if (startupBox.exec() != QDialog::Accepted) {
        delete filePathHandler;
        return 0;
    }

    MainApp mainApp = MainApp(app, filePathHandler);

    delete filePathHandler;
    return app.exec();
}
