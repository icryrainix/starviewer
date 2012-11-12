#include "screenlayout.h"

#include <QDesktopWidget>
#include <QApplication>

namespace udg {

const int ScreenLayout::SamePositionThreshold = 5;

ScreenLayout::ScreenLayout()
{
}

ScreenLayout::~ScreenLayout()
{
}

bool ScreenLayout::addScreen(const Screen &screen)
{
    if (screen.getID() <= Screen::NullScreenID)
    {
        return false;
    }
    
    bool success = true;
    int numberOfScreens = getNumberOfScreens();
    int i = 0;
    while (i < numberOfScreens && success)
    {
        success = m_screens.at(i).getID() != screen.getID() && !(m_screens.at(i).isPrimary() && screen.isPrimary());
        ++i;
    }

    if (success)
    {
        m_screens << screen;
    }
    
    return success;
}

int ScreenLayout::getNumberOfScreens() const
{
    return m_screens.count();
}

void ScreenLayout::clear()
{
    m_screens.clear();
}

Screen ScreenLayout::getScreen(int screenID) const
{
    Screen screen;
    
    int index = getIndexOfScreen(screenID);
    if (index > -1)
    {
        screen = m_screens.at(index);
    }

    return screen;
}

int ScreenLayout::getPrimaryScreenID() const
{
    int id = -1;
    bool found = false;
    int numberOfScreens = getNumberOfScreens();
    int i = 0;
    while (i < numberOfScreens && !found)
    {
        if (m_screens.at(i).isPrimary())
        {
            id = m_screens.at(i).getID();
            found = true;
        }
        ++i;
    }

    return id;
}

int ScreenLayout::getScreenOnTheRightOf(int screenID) const
{
    int rightScreen = Screen::NullScreenID;
    // Buscar una pantalla a la dreta i a la mateixa altura + o -
    for (int i = 0; i < getNumberOfScreens(); ++i)
    {
        // Si est� a la dreta, per� no est� completament per sobre ni per sota
        if (isOnRight(i, screenID) && !isOver(i, screenID) && !isUnder(i, screenID))
        {
            // Si encara no hem trobat cap pantalla
            if (rightScreen == Screen::NullScreenID)
            {
                rightScreen = i;
            }
            // De les pantalles de la dreta, volem la m�s pr�xima
            // Si la pantalla que hem trobat est� m�s a l'esquerra que la que tenim
            else if (isOnLeft(i, rightScreen))
            {
                rightScreen = i;
            }
        }
    }

    return rightScreen;
}

int ScreenLayout::getScreenOnTheLeftOf(int screenID) const
{
    int leftScreen = Screen::NullScreenID;
    // Buscar una pantalla a l'esquera i a la mateixa altura + o -
    for (int i = 0; i < getNumberOfScreens(); ++i)
    {
        // Si est� a l'esquera, per� no est� completament per sobre ni per sota
        if (isOnLeft(i, screenID) && !isOver(i, screenID) && !isUnder(i, screenID))
        {
            // Si encara no hem trobat cap pantalla
            if (leftScreen == Screen::NullScreenID)
            {
                leftScreen = i;
            }
            // De les pantalles de l'esquera, volem la m�s pr�xima
            // Si la pantalla que hem trobat est� m�s a la dreta que la que tenim
            else if (isOnRight(i, leftScreen))
            {
                leftScreen = i;
            }
        }
    }

    return leftScreen;
}

int ScreenLayout::getPreviousScreenOf(int screenID) const
{
    if (getIndexOfScreen(screenID) < 0)
    {
        return -1;
    }
    
    Screen currentScreen = getScreen(screenID);

    int previousScreenID = getScreenOnTheLeftOf(screenID);
    // Si no hi ha cap pantalla a l'esquerra, llavors busquem la de m�s a la dreta que est� per sobre d'aquesta
    if (previousScreenID == -1)
    {
        Screen screen;
        for (int i = 0; i < getNumberOfScreens(); i++)
        {
            screen = getScreen(i);
            if (screen.isHigher(currentScreen))
            {
                // Si encara no hem trobat cap pantalla
                if (previousScreenID == -1)
                {
                    previousScreenID = i;
                }
                // De les pantalles de sobre, volem la m�s a la dreta
                else if (isOnRight(i, previousScreenID))
                {
                    previousScreenID = i;
                }
            }
        }
    }

    // Si no hi ha cap pantalla per sobre de la actual, agafarem la de m�s avall a la dreta
    if (previousScreenID == -1)
    {
        Screen screen;
        // Amb aix� assegurem que mai arribar� al moveToDesktop valent -1
        previousScreenID = 0;
        for (int i = 1; i < getNumberOfScreens(); i++)
        {
            screen = getScreen(i);
            // Si est� per sota de l'actual ens la quedem
            if (isUnder(i, previousScreenID))
            {
                previousScreenID = i;
            }
            // Si no, si no est� per sobre, l'agafem si est� m�s a la dreta que l'actual
            else
            {
                Screen previousScreen = getScreen(previousScreenID);
                if (!isOver(i, previousScreenID) && screen.isMoreToTheRight(previousScreen))
                {
                    previousScreenID = i;
                }
            }
        }
    }

    return previousScreenID;
}

int ScreenLayout::getNextScreenOf(int screenID) const
{
    if (getIndexOfScreen(screenID) < 0)
    {
        return -1;
    }
    
    Screen currentScreen = getScreen(screenID);

    // Buscar una pantalla a la dreta i a la mateixa altura + o -
    int nextScreenID = getScreenOnTheRightOf(screenID);
    
    // Si no hi ha cap pantalla a la dreta, llavors busquem la de m�s a l'esquerra que est� per sota d'aquesta
    if (nextScreenID == -1)
    {
        Screen screen;
        for (int i = 0; i < getNumberOfScreens(); i++)
        {
            screen = getScreen(i);
            if (screen.isLower(currentScreen))
            {
                // Si encara no hem trobat cap pantalla
                if (nextScreenID == -1)
                {
                    nextScreenID = i;
                }
                // De les pantalles de sota, volem la m�s a l'esquerra
                else if (isOnLeft(i, nextScreenID))
                {
                    nextScreenID = i;
                }
            }
        }
    }

    // Si no hi ha cap patalla per sota de la actual, agafarem la de m�s amunt a l'esquerra
    if (nextScreenID == -1)
    {
        Screen screen;
        // Amb aix� assegurem que mai arribar� al moveToDesktop valent -1
        nextScreenID = 0;
        for (int i = 1; i < getNumberOfScreens(); i++)
        {
            screen = getScreen(i);
            // Si est� per sobre de l'actual ens la quedem
            if (isOver(i, nextScreenID))
            {
                nextScreenID = i;
            }
            // Si no, si no est� per sota, l'agafem si est� m�s a l'esquerra que l'actual
            else
            {
                Screen nextScreen = getScreen(nextScreenID);
                if (!isUnder(i, nextScreenID) && screen.isMoreToTheLeft(nextScreen))
                {
                    nextScreenID = i;
                }
            }
        }
    }

    return nextScreenID;
}

bool ScreenLayout::isOver(int screen1, int screen2) const
{
    if (getIndexOfScreen(screen1) == Screen::NullScreenID || getIndexOfScreen(screen2) == Screen::NullScreenID)
    {
        return false;
    }
    
    QRect screen1Geometry = getScreen(screen1).getGeometry();
    QRect screen2Geometry = getScreen(screen2).getGeometry();
    if (screen1Geometry.bottom() <= screen2Geometry.top())
    {
        return true;
    }
    return false;
}

bool ScreenLayout::isUnder(int screen1, int screen2) const
{
    if (getIndexOfScreen(screen1) == Screen::NullScreenID || getIndexOfScreen(screen2) == Screen::NullScreenID)
    {
        return false;
    }

    QRect screen1Geometry = getScreen(screen1).getGeometry();
    QRect screen2Geometry = getScreen(screen2).getGeometry();
    if (screen1Geometry.top() >= screen2Geometry.bottom())
    {
        return true;
    }
    return false;
}

bool ScreenLayout::isOnLeft(int screen1, int screen2) const
{
    if (getIndexOfScreen(screen1) == Screen::NullScreenID || getIndexOfScreen(screen2) == Screen::NullScreenID)
    {
        return false;
    }

    QRect screen1Geometry = getScreen(screen1).getGeometry();
    QRect screen2Geometry = getScreen(screen2).getGeometry();
    if (screen1Geometry.right() <= screen2Geometry.left())
    {
        return true;
    }
    return false;
}

bool ScreenLayout::isOnRight(int screen1, int screen2) const
{
    if (getIndexOfScreen(screen1) == Screen::NullScreenID || getIndexOfScreen(screen2) == Screen::NullScreenID)
    {
        return false;
    }
    
    QRect screen1Geometry = getScreen(screen1).getGeometry();
    QRect screen2Geometry = getScreen(screen2).getGeometry();
    if (screen1Geometry.left() >= screen2Geometry.right())
    {
        return true;
    }
    return false;
}

int ScreenLayout::getIndexOfScreen(int screenID) const
{
    int i = 0;
    int numberOfScreens = getNumberOfScreens();
    while (i < numberOfScreens)
    {
        if (m_screens.at(i).getID() == screenID)
        {
            return i;
        }
        ++i;
    }

    return -1;
}

} // End namespace udg
