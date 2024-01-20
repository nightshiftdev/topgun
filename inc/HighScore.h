/*
* ============================================================================
*  Name     : CHighScoreItem from HighScore.h
*  Part of  : ARFUN
*  Created  : 12/5/2004 by pawel.kijowski@gmail.com
*  Implementation notes:
*     Holds list item data.
*  Version  : 1.0 Beta
*  Copyright: ilPadrino Group
* ============================================================================
*/

#ifndef __HIGH_SCORE_ITEM_H__
#define __HIGH_SCORE_ITEM_H__

/*! 
  @class CHighScoreItem
  
  @discussion An instance of CHighScoreItem represents a single item on the list.
  It has a name, as an HBufC, and a set of flags, including a flag to indicate
  whether the item is marked or not.
  */
class CHighScoreItem : public CBase
	{

public:

/*!
  @function NewL
   
  @discussion Create a CHighScoreItem object
  @result a pointer to the created instance of CHighScoreItem
*/
    static CHighScoreItem* NewL();

/*!
  @function NewLC
   
  @discussion Create a CHighScoreItem object
  @result a pointer to the created instance of CHighScoreItem
*/
    static CHighScoreItem* NewLC();

/*!
  @function	~CHighScoreItem

  @discussion Destroy the object
  */

	~CHighScoreItem();

public:

 static TInt CompareTop(const CHighScoreItem& aFirst, const CHighScoreItem& aSecond);

/*!
  @function	SetPlayerNameL

  @discussion Set the Player Name.
  @param aValue the value to set the name to
  */
	void SetPlayerNameL( HBufC* aPlayerName);

/*!
  @function	PlayerName

  @discussion Get the Player Name.
  @result the Player Name
  */
	HBufC* PlayerName() const;

/*!
  @function	HighScore

  @discussion Get the high score of the item.
  @result the high score of the player
  */
	TUint HighScore() const;

/*!
  @function	SetHighScore

  @discussion Set the index of the item to 'aIndex'.
  @param aIndex the value to set the index to
  */
	void SetHighScore(TUint aIndex);


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
  @function CHighScoreItem

  @discussion Perform any first stage construction.
  */
	CHighScoreItem();

/*!
  @function	ConstructL

  @discussion Perform any second stage construction.
  */
	void ConstructL();

private:

	/*! @var iName the name of the item */
	HBufC* iPlayerName;

	/*! @var iScore holds the list type */
	TUint iScore;
	};

#endif //__HIGH_SCORE_ITEM_H__
