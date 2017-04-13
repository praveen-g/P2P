#include "message.hh"

Message::Message() : type(EMPTY_MSG) {}

Message::Message(const QString &msg,
                 const QString &origin,
                 const quint32 &seqNum) : type(RUMOR_MSG) {
  QMap<QString, QVariant> wrapper;
  wrapper["ChatText"] = msg;
  wrapper["Origin"] = origin;
  wrapper["SeqNo"] = seqNum;
  content = wrapper;
}

Message::Message(const QMap<QString, QVariant> &status) : type(STATUS_MSG) {
  QMap<QString, QVariant> wrapper;
  wrapper["Want"] = QVariant(status);
  content = wrapper;
}

Message::Message(const QByteArray& data) {
  QDataStream m(data);
  qDebug() << "deserialize bytearray with size: " << data.size();
  m >> *this;
}

Message::Message(const Message& msg) : type(msg.type),
                                       content(msg.content) {}

QByteArray Message::serialize() const {
  QByteArray buffer;
  QDataStream m(&buffer, QIODevice::WriteOnly);
  m << *this;
  qDebug() << "serialize as size: " << buffer.size();
  return buffer;
}

QString Message::printMessage() const {
  if (type == RUMOR_MSG) {
    return content["ChatText"].toString();
  } else {
    qDebug() << "Message is not of type Rumor";
    return "";
  }
}

QString Message::printOrigin() const {
  if (type == RUMOR_MSG) {
    return content["Origin"].toString();
  } else {
    qDebug() << "Message is not of type Rumor";
    return "";
  }
}

quint32 Message::getSeq() const {
  if (type == RUMOR_MSG) {
    return content["SeqNo"].toUInt();
  } else {
    qDebug() << "Message is not of type Rumor";
    return 0;
  }
}

QMap<QString, QVariant> Message::getStatus() const {
  return content["Want"].toMap();
}

bool Message::isRumor() const {
  return type == RUMOR_MSG;
}

bool Message::isFrom(const QUuid &uuid) const {
  return isRumor() && content["Origin"].toString() == uuid.toString();
}

QDebug &operator<<(QDebug &out, const Message &msg) {
  if (msg.type == RUMOR_MSG) {
    return out << "<type: RUMOR, text:" << msg.printMessage()
               << ", origin:" << msg.content["Origin"].toString()
               << ", seqNum:" << msg.content["SeqNo"].toUInt() << " >";
  } else if (msg.type == STATUS_MSG) {
    out << "<type: STATUS, < ";
    QMap<QString, QVariant>::iterator i;
    QMap<QString, QVariant> m = msg.content["Want"].toMap();
    for (i = m.begin(); i != m.end(); i++) {
      out << "< " << i.key() << ", " << i.value() << " >, ";
      return out;
    }
  } else {
    return out << "<type: EMPTY>";
  }
  return out;
}

QDataStream &operator<<(QDataStream &out, const Message &msg) {
  QMap<QString, QVariant> wrapper;
  wrapper["type"] = msg.type;
  wrapper["content"] = msg.content;
  return out << wrapper;
}

QDataStream &operator>>(QDataStream &in, Message &msg) {
  QMap<QString, QVariant> wrapper;
  in >> wrapper;
  msg.type = wrapper["type"].toInt();
  msg.content = wrapper["content"].toMap();
  return in;
}
