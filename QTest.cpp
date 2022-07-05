#include <iostream>
#include <QApplication>
#include <QFileDialog>
#include <QFile>
#include <QDebug>
#include <QPushButton>

using namespace std;

// g++ -Wall $(pkg-config --cflags --libs Qt6Widgets) -fPIC  test.cpp && ./a.out

class QFileDialogTester : public QWidget {
    public:
        QString openFile () {
            QString filename = QFileDialog::getOpenFileName (
                this,
                "Open Document",
                QDir::currentPath(),
                "All files (*.*) ;; Document files (*.doc *.rtf);; PNG files (*.png)");

                return filename;
        }
};

vector<QStringList> getdata  (QString path){

    QFile file (path);
    vector<QStringList> csv;
    //file.setFileName (path);
    file.open (QIODevice::ReadOnly);

    while (!file.atEnd()) {
        QString str = file.readLine();
        QStringList oneString = str.split(' ');

        csv.push_back(oneString);
    }

    file.close();
    return csv;
}

int main (int argc, char **argv) {

    QApplication app (argc, argv);

    QFileDialogTester test;


     QWidget window;
     window.setFixedSize(1000, 500);

     QPushButton *button = new QPushButton("Hello World", &window);
     button->setGeometry(100, 100, 80, 30);

     window.show();

    /*
    QString filename = test.openFile();


    vector<QStringList> data = getdata (filename);

    for (auto &token : data) {
        cout << token[0].toStdString() << endl;
    }
    string file2 = filename.toStdString();
    // Either this if you use UTF-8 anywhere
    std::string utf8_text = qs.toUtf8().constData();

    // or this if you're on Windows :-)
    std::string current_locale_text = qs.toLocal8Bit().constData();
    */


    return app.exec();
}
