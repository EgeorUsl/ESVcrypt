#include "gui.hpp"
#include "../core/utils.hpp"
#include <QCryptographicHash>
#include <QRandomGenerator>

#define iterations 100000
#define keyLength 32 // AES-256

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
  setupUi();
  setWindowTitle("ESVcrypt");
}

void MainWindow::setupUi() {
  QWidget *centralWidget = new QWidget(this);
  setCentralWidget(centralWidget);

  QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

  // Input file
  QHBoxLayout *inputLayout = new QHBoxLayout();
  inputFileEdit = new QLineEdit(this);
  browseInputButton = new QPushButton("Browse", this);
  inputLayout->addWidget(new QLabel("Input File:", this));
  inputLayout->addWidget(inputFileEdit);
  inputLayout->addWidget(browseInputButton);

  // Output file
  QHBoxLayout *outputLayout = new QHBoxLayout();
  outputFileEdit = new QLineEdit(this);
  browseOutputButton = new QPushButton("Browse", this);
  outputLayout->addWidget(new QLabel("Output File:", this));
  outputLayout->addWidget(outputFileEdit);
  outputLayout->addWidget(browseOutputButton);

  // Password
  QHBoxLayout *passwordLayout = new QHBoxLayout();
  passwordEdit = new QLineEdit(this);
  passwordEdit->setEchoMode(QLineEdit::Password);
  passwordLayout->addWidget(new QLabel("Password:", this));
  passwordLayout->addWidget(passwordEdit);

  // Confirm password
  QHBoxLayout *confirmPasswordLayout = new QHBoxLayout();
  confirmPasswordEdit = new QLineEdit(this);
  confirmPasswordEdit->setEchoMode(QLineEdit::Password);
  confirmPasswordLayout->addWidget(new QLabel("Confirm Password:", this));
  confirmPasswordLayout->addWidget(confirmPasswordEdit);

  // Buttons
  QHBoxLayout *buttonLayout = new QHBoxLayout();
  encryptButton = new QPushButton("Encrypt", this);
  decryptButton = new QPushButton("Decrypt", this);
  buttonLayout->addWidget(encryptButton);
  buttonLayout->addWidget(decryptButton);

  // Add all layouts to main layout
  mainLayout->addLayout(inputLayout);
  mainLayout->addLayout(outputLayout);
  mainLayout->addLayout(passwordLayout);
  mainLayout->addLayout(confirmPasswordLayout);
  mainLayout->addLayout(buttonLayout);

  // Connect signals
  connect(browseInputButton, &QPushButton::clicked, this,
          &MainWindow::onBrowseInputClicked);
  connect(browseOutputButton, &QPushButton::clicked, this,
          &MainWindow::onBrowseOutputClicked);
  connect(encryptButton, &QPushButton::clicked, this,
          &MainWindow::onEncryptClicked);
  connect(decryptButton, &QPushButton::clicked, this,
          &MainWindow::onDecryptClicked);

  // Set window properties
  resize(500, 250);
}

bool MainWindow::validateInput() {
  if (inputFileEdit->text().isEmpty() || outputFileEdit->text().isEmpty()) {
    QMessageBox::warning(this, "Error",
                         "Please specify both input and output files.");
    return false;
  }
  if (passwordEdit->text().isEmpty()) {
    QMessageBox::warning(this, "Error", "Please enter a password.");
    return false;
  }
  return true;
}

void MainWindow::onBrowseInputClicked() {
  QString fileName = QFileDialog::getOpenFileName(this, "Select Input File");
  if (!fileName.isEmpty()) {
    inputFileEdit->setText(fileName);
  }
}

void MainWindow::onBrowseOutputClicked() {
  QString fileName = QFileDialog::getSaveFileName(this, "Select Output File");
  if (!fileName.isEmpty()) {
    outputFileEdit->setText(fileName);
  }
}

void MainWindow::onEncryptClicked() {
  if (!validateInput())
    return;

  if (passwordEdit->text() != confirmPasswordEdit->text()) {
    QMessageBox::warning(this, "Error", "Passwords do not match.");
    return;
  }

  // Generate salt (16 bytes)
  QByteArray salt(16, '\0');
  QRandomGenerator::global()->fillRange(
      reinterpret_cast<quint32 *>(salt.data()), salt.size() / sizeof(quint32));

  // Generate IV (16 bytes)
  QByteArray iv(16, '\0');
  QRandomGenerator::global()->fillRange(reinterpret_cast<quint32 *>(iv.data()),
                                        iv.size() / sizeof(quint32));

  // Generate key
  QByteArray password = passwordEdit->text().toUtf8();
  QByteArray key =
      QCryptographicHash::hash(password + salt, QCryptographicHash::Sha256);
  for (int i = 1; i < iterations; i++) {
    key = QCryptographicHash::hash(key + password + salt,
                                   QCryptographicHash::Sha256);
  }

  if (encryptFile(inputFileEdit->text(), outputFileEdit->text(), key, iv,
                  salt)) {
    QMessageBox::information(this, "Success", "File encrypted successfully!");
  } else {
    QMessageBox::critical(this, "Error", "Encryption failed!");
  }
}

void MainWindow::onDecryptClicked() {
  if (!validateInput())
    return;

  QByteArray password = passwordEdit->text().toUtf8();
  if (decryptFile(inputFileEdit->text(), outputFileEdit->text(), password)) {
    QMessageBox::information(this, "Success", "File decrypted successfully!");
  } else {
    QMessageBox::critical(this, "Error", "Decryption failed!");
  }
}
