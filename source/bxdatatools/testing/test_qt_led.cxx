// Standard library:
#include <iostream>

// This project:
#include <datatools/datatools.h>
#include <datatools/datatools_config.h>
#include <bayeux/bayeux.h>

#if DATATOOLS_WITH_QT_GUI == 1
#include <QApplication>
#include <QVBoxLayout>
#include <QPushButton>
#include <QWidget>
#include <QDialog>
#include <datatools/qt/led.h>
#endif // DATATOOLS_WITH_QT_GUI == 1

void test_qt_led(bool gui_ = false);

int main(int argc_, char * argv_[])
{
  bayeux::initialize(argc_, argv_);

  bool gui = false;
  int iarg =  1;
  while (iarg < argc_) {
    std::string arg = argv_[iarg];
    if ((arg == "-g") || (arg == "--gui")) gui = true;
    iarg++;
  }

  test_qt_led(gui);

  bayeux::terminate();
  return 0;
}

void test_qt_led(bool gui_)
{
  if (gui_) {
    int argc = 1;
    // char qtitle[100];
    // qtitle = { "test" };
#if DATATOOLS_WITH_QT_GUI == 1
    const char * argv[] = { "test" };
    QApplication app(argc, (char **) argv);
    QWidget window;
    QVBoxLayout * layout = new QVBoxLayout;
    QPushButton * my_button = new QPushButton("Toggle");
    datatools::qt::led  * my_led = new datatools::qt::led(datatools::qt::led::Triangle,
                                                          datatools::qt::led::Green,
                                                          datatools::qt::led::Red);
    my_led->set_value(true);
    datatools::qt::led * my_led2 = new datatools::qt::led;
    my_led2->set_value(false);
    my_led2->set_on_color(datatools::qt::led::Orange);
    QObject::connect(my_button, SIGNAL(clicked()),my_led,SLOT(toggle_value()));
    QObject::connect(my_button, SIGNAL(clicked()),my_led2,SLOT(toggle_value()));
    layout->addWidget(my_led);
    layout->addWidget(my_led2);
    layout->addWidget(my_button);
    window.setLayout(layout);
    window.show();
    int ret = app.exec();
    if (ret == QDialog::Rejected) {
      DT_LOG_NOTICE(datatools::logger::PRIO_ALWAYS, "datatools::qt::led test GUI dialog was rejected!");
    }
#else
    std::cerr << "test_qt_led::No support for Qt based GUI\n";
#endif // DATATOOLS_WITH_QT_GUI == 1
  }
  return;
}
