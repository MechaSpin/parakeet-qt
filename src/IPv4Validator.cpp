/*
	Copyright 2021 OpenJAUS, LLC (dba MechaSpin). Subject to the MIT license.
*/

#include "IPv4Validator.h"

const QString ipaddressAcceptedCharacters = ".locahst";
const int MAX_IPADDRESS_LENGTH = 15;
const int MAX_PORT_LENGTH = 5;

void IPv4Validator::ensureValidPortCharacters(QPlainTextEdit* textBox)
{
    checkTextboxCharacterRequirements(textBox, MAX_PORT_LENGTH, &IPv4Validator::isAllowedPortCharacter);
}

void IPv4Validator::ensureValidAddressCharacters(QPlainTextEdit* textBox)
{
    checkTextboxCharacterRequirements(textBox, MAX_IPADDRESS_LENGTH, &IPv4Validator::isAllowedIPAddressCharacter);
}

void IPv4Validator::checkTextboxCharacterRequirements(QPlainTextEdit* textBox, short maxStringLength, std::function<bool(QChar)> characterRequirementFunction)
{
    bool requiresUpdate = false;
    QString newText = textBox->toPlainText();

    for(int i = newText.length() - 1; i >= 0; i--)
    {
        if(!characterRequirementFunction(newText[i]))
        {
            newText.remove(i, 1);
            requiresUpdate = true;
        }
    }

    if(newText.length() > maxStringLength)
    {
        newText = newText.remove(maxStringLength, newText.length() - maxStringLength);
        requiresUpdate = true;
    }

    if(requiresUpdate)
    {
        textBox->setPlainText(newText);
        textBox->moveCursor(QTextCursor::MoveOperation::End);
    }
}

bool IPv4Validator::isAddressValid(const QString& address)
{
    //Addresses follow A.B.C.D or localhost
    //where A/B/C/D are 1-3 digits in length

    if(address == "localhost")
    {
        return true;
    }
    else
    {
        QStringList periodDelimited = address.split('.');

        if(periodDelimited.size() != 4)
        {
            return false;
        }

        for(int i = 0; i < periodDelimited.size(); i++)
        {
            auto sectionLength = periodDelimited[i].length();
            if(sectionLength < 1 || sectionLength > 3)
            {
                return false;
            }

            for(int j = 0; j < sectionLength; j++)
            {
                if(!periodDelimited[i][j].isDigit())
                {
                    return false;
                }
            }
        }

        return true;
    }
}

bool IPv4Validator::isAllowedIPAddressCharacter(QChar c)
{
    return c.isDigit() || ipaddressAcceptedCharacters.contains(c);
}

bool IPv4Validator::isAllowedPortCharacter(QChar c)
{
    return c.isDigit();
}
