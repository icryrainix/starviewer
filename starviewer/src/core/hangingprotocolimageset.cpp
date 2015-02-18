/*************************************************************************************
  Copyright (C) 2014 Laboratori de Gràfics i Imatge, Universitat de Girona &
  Institut de Diagnòstic per la Imatge.
  Girona 2014. All rights reserved.
  http://starviewer.udg.edu

  This file is part of the Starviewer (Medical Imaging Software) open source project.
  It is subject to the license terms in the LICENSE file found in the top-level
  directory of this distribution and at http://starviewer.udg.edu/license. No part of
  the Starviewer (Medical Imaging Software) open source project, including this file,
  may be copied, modified, propagated, or distributed except according to the
  terms contained in the LICENSE file.
 *************************************************************************************/

#include "hangingprotocolimageset.h"
#include "logging.h"
#include "series.h"
#include "study.h"
#include "hangingprotocol.h"
#include "hangingprotocolimagesetrestriction.h"

namespace udg {

HangingProtocolImageSet::HangingProtocolImageSet(QObject *parent)
 : QObject(parent)
{
    m_hangingProtocol = NULL;
    m_previousStudyToDisplay = NULL;
    m_serieToDisplay = NULL;
    m_isPreviousStudy = false;
    m_downloaded = true;
    m_imageNumberInPatientModality = -1;
}

HangingProtocolImageSet::~HangingProtocolImageSet()
{
}

void HangingProtocolImageSet::setIdentifier(int identifier)
{
    m_identifier = identifier;
}

int HangingProtocolImageSet::getIdentifier() const
{
    return m_identifier;
}

void HangingProtocolImageSet::setHangingProtocol(HangingProtocol *hangingProtocol)
{
    m_hangingProtocol = hangingProtocol;
}

HangingProtocol* HangingProtocolImageSet::getHangingProtocol() const
{
    return m_hangingProtocol;
}

const HangingProtocolImageSetRestrictionExpression& HangingProtocolImageSet::getRestrictionExpression() const
{
    return m_restrictionExpression;
}

void HangingProtocolImageSet::setRestrictionExpression(const HangingProtocolImageSetRestrictionExpression &restrictionExpression)
{
    m_restrictionExpression = restrictionExpression;
}

void HangingProtocolImageSet::setTypeOfItem(QString type)
{
    m_typeOfItem = type;
}

QString HangingProtocolImageSet::getTypeOfItem() const
{
    return m_typeOfItem;
}

void HangingProtocolImageSet::setImageToDisplay(int imageNumber)
{
    m_imageToDisplay = imageNumber;
}

int HangingProtocolImageSet::getImageToDisplay() const
{
    return m_imageToDisplay;
}

void HangingProtocolImageSet::setSeriesToDisplay(Series *series)
{
    m_serieToDisplay = series;
}

Series* HangingProtocolImageSet::getSeriesToDisplay() const
{
    return m_serieToDisplay;
}

void HangingProtocolImageSet::show()
{
    DEBUG_LOG(QString("    Identifier %1\n").arg(m_identifier));
}

void HangingProtocolImageSet::setIsPreviousStudy(bool hasPreviousStudy)
{
    m_isPreviousStudy = hasPreviousStudy;
}

bool HangingProtocolImageSet::isPreviousStudy()
{
    return m_isPreviousStudy;
}

void HangingProtocolImageSet::setDownloaded(bool option)
{
    m_downloaded = option;
}

bool HangingProtocolImageSet::isDownloaded()
{
    return m_downloaded;
}

void HangingProtocolImageSet::setPreviousStudyToDisplay(Study *study)
{
    m_previousStudyToDisplay = study;
}

Study* HangingProtocolImageSet::getPreviousStudyToDisplay()
{
    return m_previousStudyToDisplay;
}

void HangingProtocolImageSet::setPreviousImageSetReference(int imageSetNumber)
{
    m_previousImageSetReference = imageSetNumber;
}

int HangingProtocolImageSet::getPreviousImageSetReference()
{
    return m_previousImageSetReference;
}

int HangingProtocolImageSet::getImageNumberInPatientModality()
{
    return m_imageNumberInPatientModality;
}

void HangingProtocolImageSet::setImageNumberInPatientModality(int imageNumberInPatientModality)
{
    m_imageNumberInPatientModality = imageNumberInPatientModality;
}

}
