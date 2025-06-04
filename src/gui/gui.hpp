// gui.hpp
#ifndef GUI_HPP
#define GUI_HPP

#include <QFileDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);

private slots:
  void onEncryptClicked();
  void onDecryptClicked();
  void onBrowseInputClicked();
  void onBrowseOutputClicked();

private:
  QLineEdit *inputFileEdit;
  QLineEdit *outputFileEdit;
  QLineEdit *passwordEdit;
  QLineEdit *confirmPasswordEdit;
  QPushButton *encryptButton;
  QPushButton *decryptButton;
  QPushButton *browseInputButton;
  QPushButton *browseOutputButton;

  void setupUi();
  bool validateInput();
};

#endif
