#include "stdafx.h"

#include <ctime>

using namespace std;

#ifndef FACESLIST
const char* FACES[8] = { "none", "right", "left", "top", "front", "back", "bottom", "MAX" };
#define FACESLIST
#endif


void CHexaedro::LogProcessFace1()
{

	char buffer[256];
	time_t now = time(0);
	time_t rawtime;
	struct tm * timeinfo;

	time(&rawtime);
	timeinfo = localtime(&rawtime);
	sprintf(buffer, "*\t\t\t© Vr Illustrations Ltd  %s", asctime(timeinfo));

	CSingletonInit::getInstance().getLog()->Write(" ", true);
	CSingletonInit::getInstance().getLog()->Write("*********************************************************************************************", true);
	CSingletonInit::getInstance().getLog()->Write("*\tPlugin: Hexaedrum", true);
	CSingletonInit::getInstance().getLog()->Write("* ", true);
	CSingletonInit::getInstance().getLog()->Write(buffer, false);
	CSingletonInit::getInstance().getLog()->Write("* ", true);
	CSingletonInit::getInstance().getLog()->Write("*********************************************************************************************", true);

	CSingletonInit::getInstance().getLog()->Write("*********************************************************************************************", true);
	CSingletonInit::getInstance().getLog()->Write("*\tFunction: ProcessFace", true);
	CSingletonInit::getInstance().getLog()->Write("*********************************************************************************************", true);

	CSingletonInit::getInstance().getLog()->Write(" ", true);

	CSingletonInit::getInstance().getLog()->Write("\t\t\tImageSize:", false);
	CSingletonInit::getInstance().getLog()->Write(" height:", false);
	CSingletonInit::getInstance().getLog()->Write(m_size.h, false);
	CSingletonInit::getInstance().getLog()->Write(", width:", false);
	CSingletonInit::getInstance().getLog()->Write(m_size.v, true);

	double megapixels = m_size.h * m_size.v / 1e6;
	sprintf(buffer, "\t\t\tTotal %5.3f MegaPixels", megapixels);

	CSingletonInit::getInstance().getLog()->Write(buffer, true);

	CSingletonInit::getInstance().getLog()->Write("\t\t\tHexaedrum side:", false);
	CSingletonInit::getInstance().getLog()->Write((int32)getSide(), true);

	CSingletonInit::getInstance().getLog()->Write("\t\t\tFace to fold:", false);
	CSingletonInit::getInstance().getLog()->Write(FACES[(short)(m_faceToProcess + 1)], true);
	CSingletonInit::getInstance().getLog()->Write("\t\t\tInitialized data:", false);
	CSingletonInit::getInstance().getLog()->Write("gData->pPontPixel:", false);
	CSingletonInit::getInstance().getLog()->Write(gData->pPointPixel == NULL ? "NULL" : "No", true);


	

	switch (gFilterRecord->filterCase)
	{
		case filterCaseUnsupported:
			sprintf(buffer, "filterCaseUnsupported");
			break;
		case filterCaseFlatImageNoSelection:
			sprintf(buffer, "filterCaseFlatImageNoSelection");
			break;
		case filterCaseFlatImageWithSelection:
			sprintf(buffer, "filterCaseFlatImageWithSelection");
			break;
		case filterCaseFloatingSelection:
			sprintf(buffer, "filterCaseFlatImageWithSelection");
			break;
		case filterCaseEditableTransparencyNoSelection:
			sprintf(buffer, "filterCaseEditableTransparencyNoSelection");
			break;
		case filterCaseEditableTransparencyWithSelection:
			sprintf(buffer, "filterCaseEditableTransparencyWithSelection");
			break;
		case filterCaseProtectedTransparencyNoSelection:
			sprintf(buffer, "filterCaseProtectedTransparencyNoSelection");
			break;
		case filterCaseProtectedTransparencyWithSelection:
			sprintf(buffer, "filterCaseProtectedTransparencyWithSelection");
			break;
	};

	CSingletonInit::getInstance().getLog()->Write("\t\t\tPhotoshop Internal FilterCase: ", false);
	CSingletonInit::getInstance().getLog()->Write(buffer, true);

}


void CHexaedro::LogProcessTilePixels2(const int32 &pixelsoutsofbounds, const int32 &pixelRowBytes, const int32 &rectWidth, const CHexaData & data)
{
	CSingletonInit::getInstance().getLog()->Write("\t\t\t\tPixels Out of bounds: ", false);
	CSingletonInit::getInstance().getLog()->Write(pixelsoutsofbounds, true);
	CSingletonInit::getInstance().getLog()->Write("\t\t\t\tpixelRowBytes - rectWidth: ", false);
	CSingletonInit::getInstance().getLog()->Write((int32)(pixelRowBytes - rectWidth), true);
	CSingletonInit::getInstance().getLog()->Write("\t\t\t\tPixels Printed: ", false);
	CSingletonInit::getInstance().getLog()->Write((int32)(data.cnt - pixelsoutsofbounds), true);
	CSingletonInit::getInstance().getLog()->Write("\t\t\t\tPixels processed: ", false);
	CSingletonInit::getInstance().getLog()->Write((int32)data.cnt, true);
}

void CHexaedro::LogProcessTilePixels1(const CHexaData& data, const int16& plane, const int32& verTile, const int32& horizTile)
{

	CSingletonInit::getInstance().getLog()->Write(" ", true);
	CSingletonInit::getInstance().getLog()->Write("\t\t\t*********************************************************************************************", true);
	CSingletonInit::getInstance().getLog()->Write("\t\t\t*\tFunction: ProcessTilePixels", true);
	CSingletonInit::getInstance().getLog()->Write("\t\t\t*********************************************************************************************", true);
	CSingletonInit::getInstance().getLog()->Write(" ", true);


	VRect Rect = GetOutRect();

	CSingletonInit::getInstance().getLog()->Write("\t\t\t\tRect:", false);
	CSingletonInit::getInstance().getLog()->Write("Left Top:(", false);
	CSingletonInit::getInstance().getLog()->Write(Rect.left, false);
	CSingletonInit::getInstance().getLog()->Write(",", false);
	CSingletonInit::getInstance().getLog()->Write(Rect.top, false);
	CSingletonInit::getInstance().getLog()->Write("),", false);

	CSingletonInit::getInstance().getLog()->Write("Right Bottom:(", false);
	CSingletonInit::getInstance().getLog()->Write(Rect.right, false);
	CSingletonInit::getInstance().getLog()->Write(",", false);
	CSingletonInit::getInstance().getLog()->Write(Rect.bottom, false);
	CSingletonInit::getInstance().getLog()->Write(")", true);
	

}
void CHexaedro::LogProcessTilePixels11(VPoint &pTransformed)
{
	CSingletonInit::getInstance().getLog()->Write("\t\t\t\tERROR Pixel Out of Bounds:(", false);
	CSingletonInit::getInstance().getLog()->Write(pTransformed.h, false);
	CSingletonInit::getInstance().getLog()->Write(",", false);
	CSingletonInit::getInstance().getLog()->Write(pTransformed.v, false);
	CSingletonInit::getInstance().getLog()->Write(")", true);
}


void CHexaedro::LogProcessImpactedArea2(const int16 &plane)
{
	CSingletonInit::getInstance().getLog()->Write("\t\t\t\tProcessing plane:", false);
	CSingletonInit::getInstance().getLog()->Write(plane, true);
	CSingletonInit::getInstance().getLog()->Write("\t\t\t\tCall to Phostoshop (AdvanceState) Returned:", false);
	CSingletonInit::getInstance().getLog()->Write(*gResult != noErr ? 0 : 1, true);
}

void CHexaedro::LogProcessImpactedArea1()
{

	CSingletonInit::getInstance().getLog()->Write(" ", true);
	CSingletonInit::getInstance().getLog()->Write("\t\t*********************************************************************************************", true);
	CSingletonInit::getInstance().getLog()->Write("\t\t*\tFunction: ProcessImpactedArea", true);
	CSingletonInit::getInstance().getLog()->Write("\t\t*********************************************************************************************", true);
	CSingletonInit::getInstance().getLog()->Write(" ", true);
}

void CHexaedro::LogProcessByTiles1()
{
	CSingletonInit::getInstance().getLog()->Write(" ", true);
	CSingletonInit::getInstance().getLog()->Write("\t*********************************************************************************************", true);
	CSingletonInit::getInstance().getLog()->Write("\t*\tFunction: processByTiles", true);
	CSingletonInit::getInstance().getLog()->Write("\t*********************************************************************************************", true);

	CSingletonInit::getInstance().getLog()->Write(" ", true);

}

void CHexaedro::LogProcessByTiles2(int vertTile, int horizTile, const int32& cnt)
{
	CSingletonInit::getInstance().getLog()->Write("\t\tProcessing Tile:(", false);
	CSingletonInit::getInstance().getLog()->Write(horizTile, false);
	CSingletonInit::getInstance().getLog()->Write(",", false);
	CSingletonInit::getInstance().getLog()->Write(vertTile, false);
	CSingletonInit::getInstance().getLog()->Write(")", true);
	CSingletonInit::getInstance().getLog()->Write("\t\tTotal points to process:", false);
	CSingletonInit::getInstance().getLog()->Write(cnt,true);

}


void CHexaedro::LogCalcImpactedPixels0()
{
	CSingletonInit::getInstance().getLog()->Write(" ", true);
	CSingletonInit::getInstance().getLog()->Write("\t*********************************************************************************************", true);
	CSingletonInit::getInstance().getLog()->Write("\t*\tFunction: CalcImpactedPixels", true);
	CSingletonInit::getInstance().getLog()->Write("\t*********************************************************************************************", true);
	CSingletonInit::getInstance().getLog()->Write(" ", true);
}

void CHexaedro::LogCalcSelection4(const CLogInfo& logInfo)
{

	CSingletonInit::getInstance().getLog()->Write(" ", true);
	CSingletonInit::getInstance().getLog()->Write("\t\t---------------------------------------------------------------------------", true);
	CSingletonInit::getInstance().getLog()->Write("\t\tImpact Report:", true);
	CSingletonInit::getInstance().getLog()->Write("\t\t---------------------------------------------------------------------------", true);

	for (int tilever = 0; tilever<m_tilesVert; tilever++)
		for (int tileHori = 0; tileHori<m_tilesHoriz; tileHori++)
			for (int planes = 0; planes < 5; planes++)
			{
				if (logInfo.badPoint[tilever][tileHori][planes] != 0)
				{
					CSingletonInit::getInstance().getLog()->Write("\t\t\tBadPoints:", false);
					CSingletonInit::getInstance().getLog()->Write(logInfo.badPoint[tilever][tileHori][planes], true);
					CSingletonInit::getInstance().getLog()->Write("\t\t\ttile vertical  impacted:", false);
					CSingletonInit::getInstance().getLog()->Write(tilever, true);
					CSingletonInit::getInstance().getLog()->Write("\t\t\ttile horizontal impacted:", false);
					CSingletonInit::getInstance().getLog()->Write(tileHori, true);
					CSingletonInit::getInstance().getLog()->Write("\t\t\tPlane:", false);
					CSingletonInit::getInstance().getLog()->Write(planes, true);

				}

				if (planes == 0)
				{

					for (int idface = 0; idface < 6; idface++)
					{

						efaces face = logInfo.facesImpacted[tilever][tileHori][(efaces)idface];

						if (face != efaces::none)
						{

							CSingletonInit::getInstance().getLog()->Write(" ", true);
							CSingletonInit::getInstance().getLog()->Write("\t\ttile impacted:(", false);
							CSingletonInit::getInstance().getLog()->Write(tileHori, false);
							CSingletonInit::getInstance().getLog()->Write(",", false);
							CSingletonInit::getInstance().getLog()->Write(tilever, false);
							CSingletonInit::getInstance().getLog()->Write(")", true);
							CSingletonInit::getInstance().getLog()->Write("\t\tFaceImpacted:", false);
							CSingletonInit::getInstance().getLog()->Write(FACES[(short)face + 1], true);
							CSingletonInit::getInstance().getLog()->Write("\t\tNumber of impacts:", false);
							CSingletonInit::getInstance().getLog()->Write((int32)m_data[tilever][tileHori].cnt, true);

						}
					}
				}
			}
}

void CHexaedro::LogCalcSelection3(VPoint &floatCoord)
{
	CSingletonInit::getInstance().getLog()->Write("\t\t\tBad Selection:", true);
	CSingletonInit::getInstance().getLog()->Write("\t\t\tSelection layer seems to be translated above image layer -> Translation Vector:(", false);
	CSingletonInit::getInstance().getLog()->Write(floatCoord.h, false);
	CSingletonInit::getInstance().getLog()->Write(",", false);
	CSingletonInit::getInstance().getLog()->Write(floatCoord.v, false);
	CSingletonInit::getInstance().getLog()->Write(")", true);
	CSingletonInit::getInstance().getLog()->Write("\t\t\tRestart again and DO NOT MOVE selection layer. Thanks.", true);
}

void CHexaedro::LogCalcImpactedPixels2(const int16 &plane, const int32 &vertTile, const int32 &horizTile)
{
	CSingletonInit::getInstance().getLog()->Write("\t\tFunction: CalcImpactedPixels", true);
	CSingletonInit::getInstance().getLog()->Write("\t\tERROR->AdvanceState Returned:", false);
	CSingletonInit::getInstance().getLog()->Write(*gResult != noErr ? 0 : 1, true);
	CSingletonInit::getInstance().getLog()->Write("\t\ttProcessing Plane:", false);
	CSingletonInit::getInstance().getLog()->Write(plane, true);
	CSingletonInit::getInstance().getLog()->Write("\t\tProcessing Vertical tile:", false);
	CSingletonInit::getInstance().getLog()->Write(vertTile, true);

	CSingletonInit::getInstance().getLog()->Write("\t\tProcessing Horizontal tile:", false);
	CSingletonInit::getInstance().getLog()->Write(horizTile, true);
}

void CHexaedro::LogCalcImpactedPixels1()
{
	int32 numTiles = m_tilesHoriz * m_tilesVert;

	CSingletonInit::getInstance().getLog()->Write("\t\tTiles to process: ", false);
	CSingletonInit::getInstance().getLog()->Write(numTiles, false);
	CSingletonInit::getInstance().getLog()->Write(" (", false);
	CSingletonInit::getInstance().getLog()->Write(m_tilesHoriz, false);
	CSingletonInit::getInstance().getLog()->Write(",", false);
	CSingletonInit::getInstance().getLog()->Write(m_tilesVert, false);
	CSingletonInit::getInstance().getLog()->Write(")", true);

	CSingletonInit::getInstance().getLog()->Write("\t\tsize of tile:(", false);
	CSingletonInit::getInstance().getLog()->Write(m_tileWidth, false);
	CSingletonInit::getInstance().getLog()->Write(",", false);
	CSingletonInit::getInstance().getLog()->Write(m_tileHeight, false);
	CSingletonInit::getInstance().getLog()->Write(")", true);

}

void CHexaedro::LogCalcImpactedPixels31(const int32& tileVert, const int32& tileHoriz, const int32& PixelX, const int32& PixelY)
{

	CSingletonInit::getInstance().getLog()->Write("\t\tPoint In Selection:(", false);
	CSingletonInit::getInstance().getLog()->Write(PixelX, false);
	CSingletonInit::getInstance().getLog()->Write(",", false);
	CSingletonInit::getInstance().getLog()->Write(PixelY, false);
	CSingletonInit::getInstance().getLog()->Write(")", true);

}

void CHexaedro::LogCalcImpactedPixels32(const efaces& impactedFace,const int16 &plane, const int32 &horizTile, const int32 &vertTile, const uint32 &MemAllocated)
{
	CSingletonInit::getInstance().getLog()->Write("\t\t\t\tProcessing Plane: ", false);
	CSingletonInit::getInstance().getLog()->Write((int32)plane, true);
	CSingletonInit::getInstance().getLog()->Write("\t\t\t\tDetected impact in tile:( ", false);
	CSingletonInit::getInstance().getLog()->Write((int32)horizTile, false);
	CSingletonInit::getInstance().getLog()->Write(",", false);
	CSingletonInit::getInstance().getLog()->Write((int32)vertTile, false);
	CSingletonInit::getInstance().getLog()->Write(") in face:", false);
	CSingletonInit::getInstance().getLog()->Write(FACES[short(impactedFace)+1], true);


	CSingletonInit::getInstance().getLog()->Write("\t\t\t\tFunction Push: Allocate pPoint with SUCCESS", true);
	CSingletonInit::getInstance().getLog()->Write("\t\t\t\ttotal Memory: ", false);
	CSingletonInit::getInstance().getLog()->Write((int32)MemAllocated, true);
}

void CHexaedro::LogSectionRect11(const int32& tileVert, const int32& tileHoriz, const int32& PixelX, const int32& PixelY)
{

	CSingletonInit::getInstance().getLog()->Write(" ", true);
	CSingletonInit::getInstance().getLog()->Write("\t\tprocessing Tile:(", false);
	CSingletonInit::getInstance().getLog()->Write(tileHoriz, false);
	CSingletonInit::getInstance().getLog()->Write(",", false);
	CSingletonInit::getInstance().getLog()->Write(tileVert, false);
	CSingletonInit::getInstance().getLog()->Write(")", true);

	VRect Rect = GetInRect();
	CSingletonInit::getInstance().getLog()->Write("\t\tRect:", false);
	CSingletonInit::getInstance().getLog()->Write("Left Top:(", false);
	CSingletonInit::getInstance().getLog()->Write(Rect.left, false);
	CSingletonInit::getInstance().getLog()->Write(",", false);
	CSingletonInit::getInstance().getLog()->Write(Rect.top, false);
	CSingletonInit::getInstance().getLog()->Write("),", false);

	CSingletonInit::getInstance().getLog()->Write("Right Bottom:(", false);
	CSingletonInit::getInstance().getLog()->Write(Rect.right, false);
	CSingletonInit::getInstance().getLog()->Write(",", false);
	CSingletonInit::getInstance().getLog()->Write(Rect.bottom, false);
	CSingletonInit::getInstance().getLog()->Write(")", true);

}

void CHexaedro::LogCalcSelectionRectangle()
{

	VRect Rect = m_area;

	CSingletonInit::getInstance().getLog()->Write(" ", true);
	CSingletonInit::getInstance().getLog()->Write("\t*********************************************************************************************", true);
	CSingletonInit::getInstance().getLog()->Write("\t*\tFunction: CalcSelectionRectangle", true);
	CSingletonInit::getInstance().getLog()->Write("\t*********************************************************************************************", true);
	CSingletonInit::getInstance().getLog()->Write(" ", true);
	
	CSingletonInit::getInstance().getLog()->Write("\t\tIntersection Rectangle:", false);
	CSingletonInit::getInstance().getLog()->Write("Left Top:(", false);
	CSingletonInit::getInstance().getLog()->Write(Rect.left, false);
	CSingletonInit::getInstance().getLog()->Write(",", false);
	CSingletonInit::getInstance().getLog()->Write(Rect.top, false);
	CSingletonInit::getInstance().getLog()->Write("),", false);

	CSingletonInit::getInstance().getLog()->Write("Right Bottom:(", false);
	CSingletonInit::getInstance().getLog()->Write(Rect.right, false);
	CSingletonInit::getInstance().getLog()->Write(",", false);
	CSingletonInit::getInstance().getLog()->Write(Rect.bottom, false);
	CSingletonInit::getInstance().getLog()->Write(")", true);
}

void CHexaedro::LogCalcTilesIntersection()
{
	CSingletonInit::getInstance().getLog()->Write(" ", true);
	CSingletonInit::getInstance().getLog()->Write("\t*********************************************************************************************", true);
	CSingletonInit::getInstance().getLog()->Write("\t*\tFunction: CalcTilesIntersection", true);
	CSingletonInit::getInstance().getLog()->Write("\t*********************************************************************************************", true);
	CSingletonInit::getInstance().getLog()->Write(" ", true);
}


void CHexaedro::LogCalcTilesIntersection2(int vertile,int horiztile,VRect* tile, VRect *interSect)
{
	
	CSingletonInit::getInstance().getLog()->Write("\t\tTile (", false);
	CSingletonInit::getInstance().getLog()->Write(horiztile, false);
	CSingletonInit::getInstance().getLog()->Write(",", false);
	CSingletonInit::getInstance().getLog()->Write(vertile, false);
	CSingletonInit::getInstance().getLog()->Write(") ", false);

	CSingletonInit::getInstance().getLog()->Write("Left Top:(", false);
	CSingletonInit::getInstance().getLog()->Write(tile->left, false);
	CSingletonInit::getInstance().getLog()->Write(",", false);
	CSingletonInit::getInstance().getLog()->Write(tile->top, false);
	CSingletonInit::getInstance().getLog()->Write("),", false);

	CSingletonInit::getInstance().getLog()->Write("Right Bottom:(", false);
	CSingletonInit::getInstance().getLog()->Write(tile->right, false);
	CSingletonInit::getInstance().getLog()->Write(",", false);
	CSingletonInit::getInstance().getLog()->Write(tile->bottom, false);
	CSingletonInit::getInstance().getLog()->Write(")", true);

	
	CSingletonInit::getInstance().getLog()->Write("\t\tIntersects with Selection in the Rectangle:", false);
	CSingletonInit::getInstance().getLog()->Write("Left Top:(", false);
	CSingletonInit::getInstance().getLog()->Write(interSect->left, false);
	CSingletonInit::getInstance().getLog()->Write(",", false);
	CSingletonInit::getInstance().getLog()->Write(interSect->top, false);
	CSingletonInit::getInstance().getLog()->Write("),", false);

	CSingletonInit::getInstance().getLog()->Write("Right Bottom:(", false);
	CSingletonInit::getInstance().getLog()->Write(interSect->right, false);
	CSingletonInit::getInstance().getLog()->Write(",", false);
	CSingletonInit::getInstance().getLog()->Write(interSect->bottom, false);
	CSingletonInit::getInstance().getLog()->Write(")", true);
}
