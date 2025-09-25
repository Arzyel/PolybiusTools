//#pragma once
//#include <iostream>
//#include <vector>
//
//class TextWrapper {
//public :
//    bool toggle = 0;
//    const char* text = { "ABCD\0" };
//    std::vector<char*> texts = { "ABCD\0" , "FOOL\0" };
//
//    const char* toggleText() {
//        toggle = (toggle) ? 0 : 1;
//        return texts[toggle];
//    }
//
//    void oldmain() {
//        const char* str1 = { "ABCD\0" };
//        const char* str2 = { "WOAH\0" };
//        const char* str3 = { "FINAL\0" };
//
//        str3 = str2;
//
//
//        QApplication app(argc, argv);
//
//        QWidget window;
//        window.setWindowTitle("My first cpp QT");
//
//        // Create a button
//        QPushButton button(str3);
//        button.resize(200, 60);
//
//        //create second button
//        QPushButton buttonClose("Close Window");
//        buttonClose.resize(200, 60);
//
//        // Connect button click to a lambda function
//        QObject::connect(&button, &QPushButton::clicked, [&]() {
//            QMessageBox::information(nullptr, "Hello", "You clicked the button!");
//            button.setText(str1);
//            });
//
//        QObject::connect(&buttonClose, &QPushButton::clicked,
//            [&]() {
//                QCoreApplication::quit();
//            });
//
//        QVBoxLayout* layout = new QVBoxLayout();
//        layout->addWidget(&button);
//        layout->addWidget(&buttonClose);
//
//        window.setLayout(layout);
//        window.show();
//
//
//        return app.exec();
//    }
//};