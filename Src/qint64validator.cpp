/*
Token from here : 
https://github.com/korrav/osc/blob/master/dialogues.h
*/
#include "qint64validator.h"
#include <QValidator>

QInt64Validator::QInt64Validator(QObject* parent, qint64 minimum, qint64 maximum): QValidator(parent), min_(minimum), max_(maximum)
{

}

void QInt64Validator::setTop(qint64 value)
{
    max_ = value;
}

void QInt64Validator::setBottom(qint64 value)
{
    min_ = value;
}

void QInt64Validator::setRange(qint64 bottom, qint64 top)
{
    min_ = bottom;
    max_ = top;
}

qint64 QInt64Validator::top() const
{
    return max_;
}

qint64 QInt64Validator::bottom() const
{
    return min_;
}

QValidator::State QInt64Validator::validate(QString &input, int&) const
{
    bool status;
    if(input.isEmpty() || input == "-" || input == "+")
        return  Intermediate;
    qint64 value = input.toLongLong(&status);
    if(!status)
        return Invalid;
    //    if(value < min_ || value > max_)
    //        return Invalid;
    if(value < min_)
        return Intermediate;
    else if(value > max_)
        return Invalid;
    else
        return Acceptable;
}

void QInt64Validator::fixup(QString &input) const
{
    qint64 value = input.toLongLong();
    if(value < min_) {
        value = min_;
        input = QString::number(value);
    }
}
