#include <QCommandLineParser>
#include <QCoreApplication>
#include <QDebug>
#include <QFile>

// Заготовка для функции шифрования
bool encryptFile(const QString &inputPath, const QString &outputPath) {
  // Тут будет логика шифрования
  qDebug() << "Шифрование файла:" << inputPath << "в" << outputPath;
  return true;
}

int main(int argc, char *argv[]) {
  QCoreApplication app(argc, argv);
  QCoreApplication::setApplicationName("FileEncryptor");
  QCoreApplication::setApplicationVersion("1.0");

  QCommandLineParser parser;
  parser.setApplicationDescription("CLI шифровальщик файлов");
  parser.addHelpOption();
  parser.addVersionOption();

  // Параметры
  QCommandLineOption inputOption(QStringList() << "i" << "input",
                                 "Входной файл", "file");
  QCommandLineOption outputOption(QStringList() << "o" << "output",
                                  "Выходной файл", "file");

  parser.addOption(inputOption);
  parser.addOption(outputOption);

  parser.process(app);

  QString inputFile = parser.value(inputOption);
  QString outputFile = parser.value(outputOption);

  if (inputFile.isEmpty() || outputFile.isEmpty()) {
    qWarning() << "Необходимо указать входной и выходной файлы.";
    return 1;
  }

  if (!encryptFile(inputFile, outputFile)) {
    qWarning() << "Ошибка при шифровании.";
    return 2;
  }

  qDebug() << "Файл успешно зашифрован!";
  return 0;
}
