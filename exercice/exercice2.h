#include <QtWidgets\qapplication.h>
#include <QtWidgets\qpushbutton.h>
#include <QtWidgets\qmessagebox.h>
#include <QtWidgets\qboxlayout.h>
#include <iostream>
#include <string>



int exercice2(int argc, char* argv[]) {
    QApplication app(argc, argv);

    QWidget window;
    window.setWindowTitle("Calculator");

    // Create widgets
    std::vector<std::string> buttonTexts = { "/", "0", ",", "=","1", "2", "3", "+", "4", "5", "6", "-", "7", "8", "9", "x" };
    std::string answerBuffer = "";

    std::vector<QPushButton*> buttons;


    for (size_t i = 0; i < buttonTexts.size(); i++)
    {
        std::string temp = std::to_string(i);
        buttons.push_back(new QPushButton(buttonTexts[i].c_str()));
        buttons[i]->resize(50, 50);
        if (buttonTexts[i] == "/" or buttonTexts[i] == "," or buttonTexts[i] == "=" or buttonTexts[i] == "+" or buttonTexts[i] == "-" or buttonTexts[i] == "x") {

        }//doesnt work with switch on str

        else {
            QObject::connect(buttons[i], &QPushButton::clicked,
                [&, i]() {
                    std::cout << buttonTexts[i] << std::endl;
                });
        }
    }







    // Create layout
    QHBoxLayout* vBoxe1 = new QHBoxLayout();
    QHBoxLayout* vBoxe2 = new QHBoxLayout();
    QHBoxLayout* vBoxe3 = new QHBoxLayout();
    QHBoxLayout* vBoxe4 = new QHBoxLayout();
    QVBoxLayout* mainBox = new QVBoxLayout();

    // Add Widgets to layout
    for (size_t i = 0; i < 4; i++)
    {
        vBoxe1->addWidget(buttons[i]);
    }
    for (size_t i = 4; i < 8; i++)
    {
        vBoxe2->addWidget(buttons[i]);
    }
    for (size_t i = 8; i < 12; i++)
    {
        vBoxe3->addWidget(buttons[i]);
    }
    for (size_t i = 12; i < 16; i++)
    {
        vBoxe4->addWidget(buttons[i]);
    }




    mainBox->addLayout(vBoxe4);
    mainBox->addLayout(vBoxe3);
    mainBox->addLayout(vBoxe2);
    mainBox->addLayout(vBoxe1);

    window.setLayout(mainBox);
    window.show();

    return app.exec();

}
