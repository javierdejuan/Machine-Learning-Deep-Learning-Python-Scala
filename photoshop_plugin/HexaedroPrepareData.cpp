#include "stdafx.h"



void CHexaedro::CalcImpactedPixels(int vertTile,int horizTile)
{
	LogCalcImpactedPixels0();

	if(!CalRectMetrics())
		return;

	//LogCalcImpactedPixels1();

	CLogInfo LogInfo;
	
	// log info

	for (int32 vertTile = 0; vertTile < m_tilesVert; vertTile++)
	{
		for (int32 horizTile = 0; horizTile < m_tilesHoriz; horizTile++)
		{

			for (short face = 0; face < (short)efaces::MAX + 1; face++)
			{
				LogInfo.facesImpacted[vertTile][horizTile][face] = efaces::none;
				LogInfo.badPoint[vertTile][horizTile][face] = 0;
			}
		}
	}

	// end log info

	// main loop process
	// strategy: Loop through the image by intescted Tiles with selection
	// for each tile, loop for the differentes planes
	// then check if the pixel is in the selecion (i.e it is not in the mask)
	// then check if the pixel is not in the isometric transformation of the selection
	// then transform the pixel, to a projected face
	// store orginal color of the pixel and transformed pixel


		//VRect inRect = GetInRect();

		VRect inRect = m_tiles[vertTile][horizTile].rect;

		inRect.top		= m_tiles[vertTile][horizTile].rect.top;
		inRect.left		= m_tiles[vertTile][horizTile].rect.left;
		inRect.bottom	= m_tiles[vertTile][horizTile].rect.bottom;
		inRect.right	= m_tiles[vertTile][horizTile].rect.right;


		if (inRect.bottom > m_size.v)
		inRect.bottom = m_size.v;
		if (inRect.right > m_size.h)
		inRect.right = m_size.h;

		SetInRect(inRect);

		// duplicate what's in the inData with the outData
		SetOutRect(inRect);

		// Fixed numbers are 16.16 values 
		// the first 16 bits represent the whole number
		// the last 16 bits represent the fraction
		gFilterRecord->inputRate = (int32)1 << 16;
		gFilterRecord->maskRate = (int32)1 << 16;

		// get the maskRect if the user has given us a selection
		if (gFilterRecord->haveMask)
		{
			SetMaskRect(inRect);
		}

		// loop through planes

		for (int16 plane = 0; plane < gFilterRecord->planes; plane++)
		{
			// we want one plane at a time, small memory foot print is good
			gFilterRecord->outLoPlane = gFilterRecord->inLoPlane = plane;
			gFilterRecord->outHiPlane = gFilterRecord->inHiPlane = plane;

			// update the gFilterRecord with our latest request
			*gResult = gFilterRecord->advanceState();


			if (*gResult != noErr)
			{
				LogCalcImpactedPixels2(plane, vertTile, horizTile);

				return;
			}

			uint8* pixel = (uint8*)gFilterRecord->inData;
			uint8* outpixel = (uint8*)gFilterRecord->outData;
			void*  mask = gFilterRecord->maskData;
			int32  maskRowBytes = gFilterRecord->maskRowBytes;
			int32  pixelRowBytes = gFilterRecord->inRowBytes;

			uint8* maskPixel = (uint8*)mask;

			VRect tileRect = GetOutRect();

			int32 rectWidth = tileRect.right - tileRect.left;
			int32 rectHeight = tileRect.bottom - tileRect.top;

			uint32 IndexPointInSubSelectionToProcess = 0;
			uint8 backgroundcolor = gFilterRecord->backColor[plane];

			VPoint floatCoord = GetFloatCoord();

			if (floatCoord.h != 0 && floatCoord.v != 0)
			{

				LogCalcSelection3(floatCoord);
			}

			// loop through pixels in the tile

			// log Info
				
			if (plane == 0)
				LogSectionRect11(vertTile, horizTile);
				
			// End log info

			bool bFirstPointInSelection = true;

			for (int32 pixelY = tileRect.top; pixelY < tileRect.bottom; pixelY++)
			{
				for (int32 pixelX = tileRect.left; pixelX < tileRect.right; pixelX++)
				{

					bool leaveItAlone = false;
					if ((maskPixel != NULL) && !(*maskPixel))
						leaveItAlone = true;

					if (!leaveItAlone) // pixel in selection
					{

						VPoint P;

						P.h = pixelX;
						P.v = pixelY;

						// check if P belongs to process face, in that case, do nothing

						if (!(m_faces[m_faceToProcess].processPixelIntersection(P.h, P.v))) // chek if the pixel is not in the isometric transformation of the face to process
						{
							VPoint PixelTransformed;
							efaces face_impacted = efaces::none;
							IndexPointInSubSelectionToProcess++;

							processPixel(P.h, P.v, PixelTransformed, face_impacted); // calculate T, and store it in face impacted with the pixel.

							

							int32 OutTileVert = -1;
							int32 OutTileHoriz = -1;

							OutTileVert = (int32)floor(PixelTransformed.h / m_tileWidth);
							OutTileHoriz = (int32)floor(PixelTransformed.v / m_tileHeight);


							bool bGoodPoint = ((OutTileVert >= 0) && (OutTileVert < m_tilesVert)) && ((OutTileHoriz >= 0) && (OutTileHoriz < m_tilesHoriz));

							if (bGoodPoint) // Point is in the correct projected tile
							{

								if (plane == 0)
									LogInfo.facesImpacted[OutTileVert][OutTileHoriz][face_impacted] = face_impacted;
									
								uint64 MemAllocated = -1;

								MemAllocated =m_data[OutTileVert][OutTileHoriz].Push(plane,PixelTransformed, *pixel, IndexPointInSubSelectionToProcess - 1); // store
									
								*outpixel = 0; //erase the source image 
									
								if(MemAllocated!=0)
								{
									LogCalcImpactedPixels31(vertTile, horizTile, pixelX, pixelY);
									LogCalcImpactedPixels32((efaces)face_impacted,plane, OutTileHoriz, OutTileVert, (int32)MemAllocated);
								}

							}
							else // store for log purposes
							{
								LogInfo.badPoint[vertTile][horizTile][plane]++;
							}
						} // endif pixel to transform

					} // endif loop in selection

					// move pixels horizontaly

				pixel++;
				outpixel++;

				if (maskPixel != NULL)
					maskPixel++;
				} // loop pixelX

				// move pixels vertically

			if (maskPixel != NULL)
				maskPixel += (maskRowBytes - rectWidth);
			pixel += (pixelRowBytes - rectWidth);
			outpixel += (pixelRowBytes - rectWidth);

			} // loop pixelY
	} // loop plane
	
	// log info

	LogCalcSelection4(LogInfo);
	
}
bool CHexaedro::CalRectMetrics()
{
	bool retval = false;

	m_tileHeight = gFilterRecord->outTileHeight;
	m_tileWidth = gFilterRecord->outTileWidth;

	if (m_tileHeight == 0 || m_tileWidth == 0)
	{
		*gResult = filterBadParameters;
		return retval;
	}

	// round up to the nearest horizontal and vertical tile count
	m_tilesHoriz = (m_tileHeight - 1 + m_size.v) / m_tileHeight;
	m_tilesVert = (m_tileWidth - 1 + m_size.h) / m_tileWidth;


	// Fixed numbers are 16.16 values 
	// the first 16 bits represent the whole number
	// the last 16 bits represent the fraction
	gFilterRecord->inputRate = (int32)1 << 16;
	gFilterRecord->maskRate = (int32)1 << 16;
	retval = true;

	return retval;
}

void CHexaedro::CalcSelectionRectangle()
{
	// necesito calcular la interseccion de la seleccion con las caras
	// voy recorriendo la seleccion tile a tile, me hace falta un solo plano
	// si es el primer punto -> es el top left del rect
	// para los puntos siguientesm guardo bottom right

	if (!CalRectMetrics())
		return;
	bool FirstPointInSelection = false;

	for (int32 vertTile = 0; vertTile < m_tilesVert; vertTile++)
	{
		for (int32 horizTile = 0; horizTile < m_tilesHoriz; horizTile++)
		{

			VRect inRect = GetInRect();

			inRect.top = horizTile * m_tileHeight;
			inRect.left = vertTile * m_tileWidth;
			inRect.bottom = inRect.top + m_tileHeight;
			inRect.right = inRect.left + m_tileWidth;

			if (inRect.bottom > m_size.v)
				inRect.bottom = m_size.v;
			if (inRect.right > m_size.h)
				inRect.right = m_size.h;

			SetInRect(inRect);

			// duplicate what's in the inData with the outData
			SetOutRect(inRect);

			// Fixed numbers are 16.16 values 
			// the first 16 bits represent the whole number
			// the last 16 bits represent the fraction
			gFilterRecord->inputRate = (int32)1 << 16;
			gFilterRecord->maskRate = (int32)1 << 16;

			// get the maskRect if the user has given us a selection
			if (gFilterRecord->haveMask)
			{
				SetMaskRect(inRect);
			}
			
			gFilterRecord->outLoPlane = gFilterRecord->inLoPlane = 1;
			gFilterRecord->outHiPlane = gFilterRecord->inHiPlane = 1;

			// update the gFilterRecord with our latest request
			*gResult = gFilterRecord->advanceState();


			if (*gResult != noErr)
			{
				//LogCalcImpactedPixels2(plane, vertTile, horizTile);

				return;
			}

			uint8* pixel = (uint8*)gFilterRecord->inData;
			uint8* outpixel = (uint8*)gFilterRecord->outData;
			void*  mask = gFilterRecord->maskData;
			int32  maskRowBytes = gFilterRecord->maskRowBytes;
			int32  pixelRowBytes = gFilterRecord->inRowBytes;

			uint8* maskPixel = (uint8*)mask;

			VRect tileRect = GetOutRect();

			int32 rectWidth = tileRect.right - tileRect.left;
			int32 rectHeight = tileRect.bottom - tileRect.top;

			uint32 IndexPointInSubSelectionToProcess = 0;

			for (int32 pixelY = tileRect.top; pixelY < tileRect.bottom; pixelY++)
			{
				for (int32 pixelX = tileRect.left; pixelX < tileRect.right; pixelX++)
				{

					bool PixelInMask = false;
					if ((maskPixel != NULL) && !(*maskPixel))
						PixelInMask = true;

					if (!PixelInMask) // pixel in selection
					{
						if(pixelX>m_area.right)
							m_area.right = pixelX;
						if(pixelY>m_area.bottom)
							m_area.bottom = pixelY;
						if (pixelX<m_area.left)
							m_area.left = pixelX;
						if (pixelY<m_area.top)
							m_area.top = pixelY;

						if (!FirstPointInSelection)
						{
							m_area.left = pixelX;
							m_area.top = pixelY;
							m_area.right = pixelX;
							m_area.left = pixelY;
							FirstPointInSelection = true;
						}
						
						

					}
					// move pixel horizontaly
					pixel++;
					outpixel++;

					if (maskPixel != NULL)
						maskPixel++;
				}// PixelX loop

				// move pixel verticaly
				if (maskPixel != NULL)
					maskPixel += (maskRowBytes - rectWidth);
				pixel += (pixelRowBytes - rectWidth);
				outpixel += (pixelRowBytes - rectWidth);

			}// PixelY loop

		}// HorizTile Loop
	}// VertTile loop

	LogCalcSelectionRectangle();


}


void CHexaedro::CalcTilesIntersection()
{
	LogCalcTilesIntersection();

	for (int32 vertTile = 0; vertTile < m_tilesVert; vertTile++)
	{
		for (int32 horizTile = 0; horizTile < m_tilesHoriz; horizTile++)
		{

			VRect tile;

			tile.top = horizTile * m_tileHeight;
			tile.left = vertTile * m_tileWidth;
			tile.bottom = tile.top + m_tileHeight;
			tile.right = tile.left + m_tileWidth;

			m_tiles[vertTile][horizTile].bintersect = CalcRectIntersection(&m_area, &tile, &m_tiles[vertTile][horizTile].rect);

			if(m_tiles[vertTile][horizTile].bintersect)
			{
				LogCalcTilesIntersection2(vertTile,horizTile,&tile, &m_tiles[vertTile][horizTile].rect);
			}
			
		}
	}
}


void CHexaedro::processPixel(const uint16& pixelX, const uint16& pixelY, VPoint& PixelTransformed, efaces& face_impacted)
{
	double vector[3] = { 0. };
	double intersect_vector[3] = { 0. };

	getXYZ(pixelX, pixelY, vector);								 // get x,y,x coordinates from pixelX,PixelY
	CubeIntersection(vector, intersect_vector, face_impacted);   // calculates the intersection with the visual Cube, (x,y,x) coordinates
	getUV(intersect_vector, face_impacted, PixelTransformed);    // get Pixel coordinates back 

}


void CHexaedro::InitializeData()
{
	
	m_mem = 0;
	m_pixelcnt = 0;
	
	m_tileHeight = 0;
	m_tileWidth = 0;
	
	m_tilesHoriz = 0;
	m_tilesVert = 0;

	m_area.bottom = 0;
	m_area.left = 0;
	m_area.right = 0;
	m_area.top = 0;
	
	DeleteBuffer();
	CreateBuffer();

	for (int i = 0; i < MAXTILES; i++)
		for (int j = 0; j < MAXTILES; j++)
		{
			m_data[i][j].Create(this);
		}
}


int32 CHexaedro::GetMem()
{
	return m_mem;
}


void CHexaedro::CreateBuffer()
{
	BufferProcs *bufferProcs = gFilterRecord->bufferProcs;
	bufferProcs->allocateProc(KILOPIXEL * KILOPIXEL * sizeof(PointPixel), &gData->pointPixelID);
	gData->pPointPixel = (PointPixel*) bufferProcs->lockProc(gData->pointPixelID, true);
	m_mem = KILOPIXEL * KILOPIXEL;

}




void CHexaedro::DeleteBuffer()
{
	BufferProcs *bufferProcs = gFilterRecord->bufferProcs;

	if(gData->pointPixelID!=NULL)
	{
		gFilterRecord->bufferProcs->unlockProc(gData->pointPixelID);
		gFilterRecord->bufferProcs->freeProc(gData->pointPixelID);
		gData->pointPixelID= NULL;
		gData->pPointPixel = NULL;
		m_mem = 0;
	}

}


