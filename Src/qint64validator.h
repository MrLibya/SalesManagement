/*
Token from here : 
https://github.com/korrav/osc/blob/master/dialogues.h
*/
#ifndef QINT64VALIDATOR_H
#define QINT64VALIDATOR_H
#include <QValidator>

class QInt64Validator: public QValidator {
    qint64 min_;
    qint64 max_;
public:
    QInt64Validator(QObject* parent, qint64 minimum = std::numeric_limits<qint64>::min(), qint64 maximum = std::numeric_limits<qint64>::max());
    void setTop(qint64 value);
    void setBottom(qint64 value);
    void setRange(qint64 bottom, qint64 top);
    qint64 top(void) const;
    qint64 bottom(void) const;
    virtual QValidator::State validate(QString& input, int&) const;
    virtual void fixup(QString &input) const;
};


#endif // QINT64VALIDATOR_H
