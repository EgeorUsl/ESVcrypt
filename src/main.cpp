#include "core/utils.hpp"
#include "gui/gui.hpp"
#include <QApplication>
#include <QCommandLineParser>
#include <QCoreApplication>
#include <QCryptographicHash>
#include <QDebug>
#include <QFile>
#include <QRandomGenerator>
#include <QString>
#include <iostream>

#define iterations 100000
#define keyLength 32 // AES-256

int runCliMode(int argc, char *argv[]) {
  QCoreApplication app(argc, argv);
  QCoreApplication::setApplicationName("ESVcrypt");
  QCoreApplication::setApplicationVersion("0.1.1");

  QCommandLineParser parser;
  parser.setApplicationDescription("");
  parser.addHelpOption();
  parser.addVersionOption();

  // Arguments
  QCommandLineOption inputOption(QStringList() << "i" << "input", "Input file",
                                 "file");
  QCommandLineOption outputOption(QStringList() << "o" << "output",
                                  "Output file", "file");

  parser.addOption(inputOption);
  parser.addOption(outputOption);

  parser.process(app);

  QString inputFile = parser.value(inputOption);
  QString outputFile = parser.value(outputOption);

  // Check if we have positional arguments
  QStringList positionalArgs = parser.positionalArguments();
  if (positionalArgs.isEmpty()) {
    qWarning() << "Mode not specified. Use 'encrypt' or 'decrypt'.";
    parser.showHelp(1);
    return 1;
  }

  QString mode = positionalArgs.at(0).toLower();

  if ((mode != "encrypt" && mode != "decrypt") &&
      (mode[0] != 'e' && mode[0] != 'd')) {
    qWarning() << "Invalid mode. Use: encrypt, decrypt";
    return 1;
  }

  // Enter the password
  std::string inputPassword;
  std::string input2Password;
  QByteArray password;

  if (mode == "decrypt" || mode[0] == 'd') {
    std::cout << "Password: ";
    std::getline(std::cin, inputPassword);
    password = QByteArray::fromStdString(inputPassword);
  }

  if (mode == "encrypt" || mode[0] == 'e') {
    while (1) {
      std::cout << "Password: ";
      std::getline(std::cin, inputPassword);
      std::cout << "Repeat password: ";
      std::getline(std::cin, input2Password);
      if (inputPassword == input2Password) {
        break;
      } else {
        qDebug() << "Passwords do not match. Please, enter the password again";
      }
    }
    password = QByteArray::fromStdString(inputPassword);
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
  QByteArray key;
  if (mode == "encrypt" || mode[0] == 'e') {
    key = QCryptographicHash::hash(password + salt, QCryptographicHash::Sha256);
    for (int i = 1; i < iterations; i++) {
      key = QCryptographicHash::hash(key + password + salt,
                                     QCryptographicHash::Sha256);
    }
  }

  // Files error
  if (inputFile.isEmpty() || outputFile.isEmpty()) {
    qWarning() << "You must specify the input and output files.";
    parser.showHelp(1);
    return 1;
  }

  // Encryption
  if (mode == "encrypt" || mode[0] == 'e') {
    if (!encryptFile(inputFile, outputFile, key, iv, salt)) {
      qWarning() << "Encryption error!";
      return 2;
    }
  }

  // Decryption
  if (mode == "decrypt" || mode[0] == 'd') {
    if (!decryptFile(inputFile, outputFile, password)) {
      qWarning() << "Decryption error!";
      return 2;
    }
  }
  return 0;
}

int main(int argc, char *argv[]) {
  // Check if we have command line arguments
  if (argc > 1) {
    return runCliMode(argc, argv);
  }

  // Run GUI mode
  QApplication app(argc, argv);
  QCoreApplication::setApplicationName("ESVcrypt");
  QCoreApplication::setApplicationVersion("0.1.1");

  MainWindow window;
  window.show();

  return app.exec();
}
