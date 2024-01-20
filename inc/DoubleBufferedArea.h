/**
 * This class provides all the components for creating offscreen areas for
 * double buffered drawing
 */

#ifndef DOUBLEBUFFEREDAREA_H
#define DOUBLEBUFFEREDAREA_H

#include <e32base.h>
#include <gdi.h>

class CFbsBitmap;
class CFbsBitmapDevice;
class CFbsBitGc;

class CDoubleBufferedArea : public CBase
    {
    public:
        static CDoubleBufferedArea* NewL(TSize aSize, TDisplayMode aDisplayMode);
        static CDoubleBufferedArea* NewLC(TSize aSize, TDisplayMode aDisplayMode);
        ~CDoubleBufferedArea();

        const CFbsBitmap& GetDoubleBufferedAreaBitmap() const;
        CFbsBitGc& GetDoubleBufferedAreaContext() const;
	    void ClearBufferedArea();

    private:
        CDoubleBufferedArea();
        void ConstructL(TSize aSize, TDisplayMode aDisplayMode);
        CFbsBitmap*       iAreaBitmap;
        CFbsBitmapDevice* iAreaBitmapDevice;
        CFbsBitGc*        iAreaBitmapContext;
    };
#endif //DOUBLEBUFFEREDAREA_H