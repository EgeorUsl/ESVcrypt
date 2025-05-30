#include "utils.hpp"
#include <QByteArray>
#include <QCryptographicHash>
#include <QDebug>
#include <QFile>
#include <QScopedPointer>
#include <QString>
#include <openssl/err.h>
#include <openssl/evp.h>

#define iterations 100000

bool encryptFile(const QString &inputPath, const QString &outputPath,
                 const QByteArray &key, const QByteArray &iv,
                 const QByteArray &salt) {
  QFile inFile(inputPath);
  if (!inFile.open(QIODevice::ReadOnly)) {
    qWarning() << "Input file error:" << inFile.errorString();
    return false;
  }

  QFile outFile(outputPath);
  if (!outFile.open(QIODevice::WriteOnly)) {
    qWarning() << "Output file error:" << outFile.errorString();
    return false;
  }

  // Write salt and IV in at the begining of the file
  outFile.write(salt);
  outFile.write(iv);

  // Settings OpenSSL
  EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
  EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL,
                     reinterpret_cast<const unsigned char *>(key.constData()),
                     reinterpret_cast<const unsigned char *>(iv.constData()));

  constexpr int bufferSize = 4096;
  unsigned char inBuf[bufferSize], outBuf[bufferSize + EVP_MAX_BLOCK_LENGTH];
  int outLen = 0;

  while (!inFile.atEnd()) {
    const qint64 bytesRead =
        inFile.read(reinterpret_cast<char *>(inBuf), bufferSize);

    if (!EVP_EncryptUpdate(ctx, outBuf, &outLen, inBuf, bytesRead)) {
      qWarning() << "Encryption failed";
      EVP_CIPHER_CTX_free(ctx);
      return false;
    }

    outFile.write(reinterpret_cast<char *>(outBuf), outLen);
  }

  // Final
  if (!EVP_EncryptFinal_ex(ctx, outBuf, &outLen)) {
    qWarning() << "Finalization failed";
    EVP_CIPHER_CTX_free(ctx);
    return false;
  }
  outFile.write(reinterpret_cast<char *>(outBuf), outLen);

  EVP_CIPHER_CTX_free(ctx);
  qDebug() << "File" << inputPath << "is encrypted!";
  return true;
}

struct EvpCipherCtxDeleter {
  static inline void cleanup(EVP_CIPHER_CTX *p) noexcept {
    if (p)
      EVP_CIPHER_CTX_free(p);
  }
};

bool decryptFile(const QString &inputPath, const QString &outputPath,
                 const QByteArray &password) {

  QFile inFile(inputPath);
  QFile outFile(outputPath);

  // Use custom deliter
  QScopedPointer<EVP_CIPHER_CTX, EvpCipherCtxDeleter> ctx(EVP_CIPHER_CTX_new());

  if (!inFile.open(QIODevice::ReadOnly) ||
      !outFile.open(QIODevice::WriteOnly) || !ctx) {
    qWarning() << "Error opening files or creating EVP context.";
    return false;
  }

  // Read salt and IV from header file
  QByteArray readSalt = inFile.read(16);
  QByteArray readIV = inFile.read(16);

  QByteArray key =
      QCryptographicHash::hash(password + readSalt, QCryptographicHash::Sha256);
  for (int i = 1; i < iterations; i++) {
    key = QCryptographicHash::hash(key + password + readSalt,
                                   QCryptographicHash::Sha256);
  }

  if (readSalt.size() != 16 || readIV.size() != 16) {
    qWarning() << "Invalid file format: cannot read salt/IV";
    return false;
  }

  if (!EVP_DecryptInit_ex(
          ctx.data(), EVP_aes_256_cbc(), nullptr,
          reinterpret_cast<const unsigned char *>(key.constData()),
          reinterpret_cast<const unsigned char *>(readIV.constData()))) {
    qWarning() << "EVP_DecryptInit_ex failed:"
               << ERR_error_string(ERR_get_error(), nullptr);
    return false;
  }

  constexpr int bufferSize = 4096;
  unsigned char inBuf[bufferSize];
  unsigned char
      outBuf[bufferSize + EVP_MAX_BLOCK_LENGTH]; // Больше буфер для дешифровки
  int outLen = 0;

  while (!inFile.atEnd()) {
    qint64 bytesRead = inFile.read(reinterpret_cast<char *>(inBuf), bufferSize);
    if (bytesRead <= 0) {
      if (!inFile.atEnd()) {
        qWarning() << "Error reading input file.";
        return false;
      }
      break;
    }

    if (!EVP_DecryptUpdate(ctx.data(), outBuf, &outLen, inBuf, bytesRead)) {
      qWarning() << "EVP_DecryptUpdate failed:"
                 << ERR_error_string(ERR_get_error(), nullptr);
      return false;
    }

    if (outLen > 0) {
      outFile.write(reinterpret_cast<char *>(outBuf), outLen);
    }
  }

  // Финализация дешифровки
  if (!EVP_DecryptFinal_ex(ctx.data(), outBuf, &outLen)) {
    qWarning() << "EVP_DecryptFinal_ex failed:"
               << ERR_error_string(ERR_get_error(), nullptr);
    qWarning() << "Possible causes: wrong password, corrupted file, or invalid "
                  "padding";
    return false;
  }

  if (outLen > 0) {
    outFile.write(reinterpret_cast<char *>(outBuf), outLen);
  }

  qDebug() << "File" << inputPath << "is decrypted!";
  return true;
}
