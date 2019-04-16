#include "stdafx.h"



void CHexaedro::ProcessTilePixels(const CHexaData& data, const int16& plane, const int32& verTile, const int32& HorizTile)
{
	uint8* outpixel = (uint8*)gFilterRecord->outData;

	VRect tileRect = GetOutRect();

	int32 rectWidth = tileRect.right - tileRect.left;
	int32 rectHeight = tileRect.bottom - tileRect.top;

	int32 areaWidth = tileRect.right - tileRect.left;
	int32 areaHeight = tileRect.bottom - tileRect.top;

	int32  pixelRowBytes = gFilterRecord->outRowBytes;

	int32 pixelsoutsofbounds = 0;

	LogProcessTilePixels1(data, plane, verTile, HorizTile);

	uint64 index = 0;

	if (outpixel)
	{
		for (uint32 i = 0; i < data.cnt; i++)
		{
			index = i + data.idx;
			VPoint pTransformed;
			pTransformed.h = gData->pPointPixel[index].p.h;
			pTransformed.v = gData->pPointPixel[index].p.v;
			uint8  Color = gData->pPointPixel[index].color[plane];
			
			uint8* Pixel = outpixel;

			uint32 deltaH = pTransformed.h - tileRect.left;
			uint32 deltaV = pTransformed.v - tileRect.top;
			uint32 delta = (pixelRowBytes - rectWidth)* deltaV + pixelRowBytes * deltaV + deltaH;

			bool bPixelInRect = (pTransformed.h >= tileRect.left) && (pTransformed.h <= tileRect.right) && (pTransformed.v >= tileRect.top) && (pTransformed.v <= tileRect.bottom);

			if (bPixelInRect)
			{
				Pixel += delta;
				
					*Pixel = Color;
					//*Pixel = 50;
				
	
			}
			else
			{
				pixelsoutsofbounds++;
				//LogProcessTilePixels11(pTransformed);
			}

		}

		LogProcessTilePixels2(pixelsoutsofbounds, pixelRowBytes, rectWidth, data);

	}

}


void CHexaedro::ProcessImpactedArea(const CHexaData& dataTile, const int32& verTile, const int32& horizTile)
{

	//aqui voy pidiendo Tiles de 1024*1024 luego veremos si se puede optimizar

	int32 tileHeight = gFilterRecord->outTileHeight;
	int32 tileWidth = gFilterRecord->outTileWidth;

	if (tileWidth == 0 || tileHeight == 0)
	{
		*gResult = filterBadParameters;
		return;
	}

	//gestionar bien la Tile que le pides a gRecordFactor

	VRect inRect = { 0,0,0,0 };

	inRect.top = horizTile * tileHeight;
	inRect.left = verTile * tileWidth;
	inRect.bottom = inRect.top + tileHeight;
	inRect.right = inRect.left + tileWidth;

	SetInRect(inRect);
	SetOutRect(inRect);
	SetMaskRect(inRect);

	for (int16 plane = 0; plane < gFilterRecord->planes; plane++)
	{
		// we want one plane at a time, small memory foot print is good
		gFilterRecord->outLoPlane = gFilterRecord->inLoPlane = plane;
		gFilterRecord->outHiPlane = gFilterRecord->inHiPlane = plane;

		// update the gFilterRecord with our latest request
		*gResult = gFilterRecord->advanceState();

		LogProcessImpactedArea1();
		LogProcessImpactedArea2(plane);

		if (*gResult != noErr) return;

		ProcessTilePixels(dataTile, plane, verTile, horizTile);
	}

}

void CHexaedro::processByTiles()
{
	// iterate through active data faces:

	for (int vertTile = 0; vertTile < m_tilesVert; vertTile++)
		for (int horizTile = 0; horizTile < m_tilesHoriz; horizTile++)
		{
			if (m_data[vertTile][horizTile].cnt != 0) //impacted Tile
			{
				LogProcessByTiles1();
				LogProcessByTiles2(vertTile, horizTile, (int32 )m_data[vertTile][horizTile].cnt);
				ProcessImpactedArea(m_data[vertTile][horizTile], vertTile, horizTile); //m_data stores the Tiles in the other way!!
			}
		}
}

