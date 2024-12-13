// Copyright (c) 2011-2020 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef NULLION_QT_NULLIONADDRESSVALIDATOR_H
#define NULLION_QT_NULLIONADDRESSVALIDATOR_H

#include <QValidator>

/** Base58 entry widget validator, checks for valid characters and
 * removes some whitespace.
 */
class NullionAddressEntryValidator : public QValidator
{
    Q_OBJECT

public:
    explicit NullionAddressEntryValidator(QObject *parent);

    State validate(QString &input, int &pos) const override;
};

/** Nullion address widget validator, checks for a valid nullion address.
 */
class NullionAddressCheckValidator : public QValidator
{
    Q_OBJECT

public:
    explicit NullionAddressCheckValidator(QObject *parent);

    State validate(QString &input, int &pos) const override;
};

#endif // NULLION_QT_NULLIONADDRESSVALIDATOR_H
