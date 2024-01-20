/*
* ============================================================================
*  Name     : CGameSettings from GameSettings.h
*  Part of  : ARFUN
*  Created  : 12/5/2004 by pawel.kijowski@gmail.com
*  Implementation notes:
*     Holds list item data.
*  Version  : 1.0 Beta
*  Copyright: ilPadrino Group
* ============================================================================
*/

#ifndef __GAME_SETTINGS_H__
#define __GAME_SETTINGS_H__

/*! 
  @class CGameSettings
  
  @discussion An instance of CGameSettings represents a single item on the list.
  It has a name, as an HBufC, and a set of flags, including a flag to indicate
  whether the item is marked or not.
  */
class CGameSettings : public CBase
	{

public:

/*!
  @function NewL
   
  @discussion Create a CGameSettings object
  @result a pointer to the created instance of CGameSettings
*/
    static CGameSettings* NewL();

/*!
  @function NewLC
   
  @discussion Create a CGameSettings object
  @result a pointer to the created instance of CGameSettings
*/
    static CGameSettings* NewLC();

/*!
  @function	~CGameSettings

  @discussion Destroy the object
  */

	~CGameSettings();

public:

 static TInt CompareTop(const CGameSettings& aFirst, const CGameSettings& aSecond);

/*!
  @function	HighScore

  @discussion Get the high score of the item.
  @result the high score of the player
  */
	TInt AudioVol() const;

/*!
  @function	SetHighScore

  @discussion Set the index of the item to 'aIndex'.
  @param aIndex the value to set the index to
  */
	void SetAudioVol(TInt aAudioVol);
	
	/*!
  @function	HighScore

  @discussion Get the high score of the item.
  @result the high score of the player
  */
	TInt SfxVol() const;

/*!
  @function	SetHighScore

  @discussion Set the index of the item to 'aIndex'.
  @param aIndex the value to set the index to
  */
	void SetSfxVol(TInt aSfxVol);


/*!
  @function	ExternalizeL

  @discussion Write the shopping item to aStream.
  @param aStream the stream to write the item to.
  */
	void ExternalizeL(RWriteStream& aStream) const;

/*!
  @function	InternalizeL

  @discussion Read the shopping item from aStream.
  @param aStream the stream to read the item from.
  */
	void InternalizeL(RReadStream& aStream);

private:

/*!
  @function CGameSettings

  @discussion Perform any first stage construction.
  */
	CGameSettings();

/*!
  @function	ConstructL

  @discussion Perform any second stage construction.
  */
	void ConstructL();

private:
  
	TInt iAudioVol;
	TInt iSfxVol;
	};

#endif //__GAME_SETTINGS_H__
