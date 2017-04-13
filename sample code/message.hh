#ifndef P2PAPP_MESSAGE_HH
#define P2PAPP_MESSAGE_HH

#include <QVariant>
#include <QDebug>
#include <QDataStream>
#include <QByteArray>
#include <QUuid>
#include <QMap>

enum {
  RUMOR_MSG = 0,
  STATUS_MSG,
  EMPTY_MSG
};

class Message {
public:
  friend QDataStream &operator<<(QDataStream &out, const Message &msg);
  friend QDataStream &operator>>(QDataStream &in, Message &msg);
  friend QDebug &operator<<(QDebug &out, const Message &msg);
  Message();
  Message(const QString &msg, const QString &origin, const quint32 &seqNum);
  Message(const QMap<QString, QVariant> &status);
  Message(const Message& msg);
  Message(const QByteArray& data);
  QByteArray serialize() const;
  QString printType() const;
  QString printMessage() const;
  QString printOrigin() const;
  quint32 getSeq() const;
  QMap<QString, QVariant> getStatus() const;
  bool isFrom(const QUuid &) const;
  bool isRumor() const;

private:
  int type;
  QMap<QString, QVariant> content;
};


#endif
