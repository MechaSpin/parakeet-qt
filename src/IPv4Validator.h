/*
	Copyright 2021 OpenJAUS, LLC (dba MechaSpin). Subject to the MIT license.
*/

#pragma once

#include <QChar>
#include <QPlainTextEdit>

#include <vector>

class IPv4Validator
{
public:
    static bool isAllowedIPAddressCharacter(QChar c);
    static bool isAllowedPortCharacter(QChar c);

    static bool isAddressValid(const QString& address);

    static void ensureValidPortCharacters(QPlainTextEdit* textBox);
    static void ensureValidAddressCharacters(QPlainTextEdit* textBox);

private:
    static void checkTextboxCharacterRequirements(QPlainTextEdit* textBox, short maxStringLength, std::function<bool(QChar)> characterRequirementFunction);
};
