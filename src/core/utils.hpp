#pragma once

#include <QByteArray>
#include <QString>

bool encryptFile(const QString &, const QString &, const QByteArray &,
                 const QByteArray &, const QByteArray &);

bool decryptFile(const QString &, const QString &, const QByteArray &);
