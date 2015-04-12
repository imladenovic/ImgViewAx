// imgx.cpp : Implementation of Cimgx
#include "stdafx.h"
#include "imgx.h"
#include "atldlgs.h"
#include "atlstr.h"

// Cimgx

LRESULT Cimgx::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// TODO: Add your message handler code here and/or call default
    CREATESTRUCT *pCreateStruct  =   reinterpret_cast< CREATESTRUCT* >( lParam );

	RECT rcContent = { 0, 0, pCreateStruct->cx -1, pCreateStruct->cy - 1 };
	//::GetClientRect(m_hWndCD,&RECT);
    //rcContent.bottom = rcContent.bottom - 2;
	//rcContent.right = rcContent.right - 2;
	m_BitmapView.Create(m_hWndCD,rcContent,NULL,WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL | WS_CLIPSIBLINGS );
   /* CBitmap bmp;
	bmp.LoadBitmap(") */
	return 0;
}


STDMETHODIMP Cimgx::OpenImage(void)
{
	// TODO: Add your implementation code here
	CFileDialog dlg(TRUE, _T("bmp"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("Bitmap Files (*.bmp)\0*.bmp\0All Files (*.*)\0*.*\0"), m_hWnd);
	if(dlg.DoModal() == IDOK)
	{
#ifndef _WIN32_WCE
		/*if(m_bPrintPreview)
			TogglePrintPreview(); */
		bitmapFile = dlg.m_szFileName;

		//Gdi+ magic
		Bitmap mBitmap(bitmapFile,false);
		HBITMAP hBmp;
		mBitmap.GetHBITMAP(0x00000000, &hBmp);

		//HBITMAP hBmp = (HBITMAP)::LoadImage(NULL, dlg.m_szFileName, IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR | LR_LOADFROMFILE);
#else
		// Using alternate image load routines here since LR_LOADFROMFILE isn't supported.
#ifdef WIN32_PLATFORM_PSPC
		HBITMAP hBmp = (HBITMAP)::SHLoadImageFile(dlg.m_szFileName);
#else	// SHLoadDIBitmap (below) is for CE, but only loads .bmp; SHLoadImageFile (above) loads .bmp, .gif, .jpg, .png
		HBITMAP hBmp = (HBITMAP)::SHLoadDIBitmap(dlg.m_szFileName);
#endif	// WIN32_PLATFORM_PSPC
#endif // _WIN32_WCE
		if(hBmp != NULL)
		{
			m_BitmapView.SetBitmap(hBmp);
		
/*#ifndef _WIN32_WCE
			m_mru.AddToList(dlg.m_ofn.lpstrFile);
			m_mru.WriteToRegistry(g_lpcstrMRURegKey);
#endif // _WIN32_WCE */
#ifndef WIN32_PLATFORM_PSPC
			/*UpdateTitleBar(dlg.m_szFileTitle); */
#endif // WIN32_PLATFORM_PSPC
			/*lstrcpy(m_szFilePath, dlg.m_szFileName);*/
		}
		else
		{
			CString strMsg = _T("Can't load image from:\n");
			strMsg += dlg.m_szFileName; 
			/*MessageBox(strMsg, g_lpcstrApp, MB_OK | MB_ICONERROR);*/
			/*AtlMessageBox(this->m_hWndCD,_T("Can't load image from:\n"),NULL);*/
		}
	}
	return S_OK;
}

LRESULT Cimgx::OnSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
{
	// TODO: Add your message handler code here and/or call default
	int sizex , sizey;
	sizex = GET_X_LPARAM(lParam);
	sizey = GET_Y_LPARAM(lParam);
	SIZE size;
	size.cx = sizex - 2;
    size.cy = sizey - 2;
	//m_BitmapView.m_size = size;
	//LPARAM newsize;
	::MoveWindow((HWND)m_BitmapView,1,1,sizex-2,sizey-2,TRUE);
	
	

	m_BitmapView.SetScrollOffset(0, 0, TRUE);
	m_BitmapView.SetScrollSize(m_BitmapView.m_size,TRUE);
	
	return 0;
}

STDMETHODIMP Cimgx::print(void)
{
	if (bitmapFile.GetLength() == 0)
	{
		AtlMessageBox(m_hWnd, _T("No image loaded!")); 
		return 0;
	}
	// TODO: Add your implementation code here
	CPageSetupDialog printsetup;
	printsetup.DoModal();
	CDC printer;
	//if (! printer.Attach(printsetup.CreatePrinterDC())) {
	//	AtlMessageBox(m_hWnd ,_T("No printer found!")); return 0;
	//} 
    
	//printer.m_bPrinting = TRUE;
	
	printer.Attach(printsetup.CreatePrinterDC());
	DOCINFO di;    
	// Initialise print document details
	::ZeroMemory (&di, sizeof (DOCINFO));
	di.cbSize = sizeof (DOCINFO);
	di.lpszDocName = bitmapFile; 
	BOOL bPrintingOK = printer.StartDoc(&di); // Begin a new print job 
	// Get the printing extents
	// and store in the m_rectDraw field of a 
	// CPrintInfo object
	//CPrintInfo info;
	//Info.SetMaxPage(1); // just one page 
	int maxw = printer.GetDeviceCaps(HORZRES);
	int maxh = printer.GetDeviceCaps(VERTRES); 
	//Info.m_rectDraw.SetRect(0, 0, maxw, maxh); 
	//for (UINT page = Info.GetMinPage(); page <= 
	//	Info.GetMaxPage() && bPrintingOK; page++) {
			printer.StartPage();    // begin new page
	//		Info.m_nCurPage = page;
			CBitmap bitmap;
			// LoadImage does the trick here, it creates a DIB section
			// You can also use a resource here
			// by using MAKEINTRESOURCE() ... etc. 

			Bitmap mBitmap(bitmapFile, false);
			HBITMAP hBmp;
			mBitmap.GetHBITMAP(0x00000000, &hBmp);

			//HBITMAP hBmp = (HBITMAP)::LoadImage(NULL,bitmapFile, IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR | LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE);
			if(!bitmap.IsNull())
				bitmap.DeleteObject();

			bitmap = hBmp;

			/*if(!bmpitmap.IsNull())
				m_bmp.GetSize(m_size);
			else
				m_size.cx = m_size.cy = 1; */
		/*	if(!bitmap.Attach(::LoadImage(
				::GetModuleHandle(NULL), bitmapFile, IMAGE_BITMAP, 0, 0, 
				LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE))) {
					AtlMessageBox(m_hWnd,_T("Error loading bitmap!")); return 0;
			}  */
			BITMAP bm;
			bitmap.GetBitmap(&bm);
			int w = bm.bmWidth; 
			int h = bm.bmHeight; 
			// create memory device context
			CDC memDC; 
			
			memDC.CreateCompatibleDC(printer.m_hDC);
			HBITMAP pbmp = memDC.SelectBitmap(bitmap);
			memDC.SetMapMode(printer.GetMapMode());
			printer.SetStretchBltMode(HALFTONE);
			// now stretchblt to maximum width on page
			printer.StretchBlt(0, 0, maxw, maxh, memDC, 0, 0, w, h, SRCCOPY); 
			// clean up
			memDC.SelectBitmap(pbmp);
			bPrintingOK = (printer.EndPage() > 0);   // end page
	//} 
	if (bPrintingOK)
		printer.EndDoc(); // end a print job
	else printer.AbortDoc();           // abort job. 

	return S_OK;
}

STDMETHODIMP Cimgx::print2(void)
{
	if (bitmapFile.GetLength() == 0)
	{
		AtlMessageBox(m_hWnd, _T("No image loaded!"));
		return 0;
	}
	// TODO: Add your implementation code here
	CPageSetupDialog printsetup;
	printsetup.DoModal();
	CDC printer;
	
	printer.Attach(printsetup.CreatePrinterDC());
	DOCINFO di;    
	// Initialise print document details
	::ZeroMemory (&di, sizeof (DOCINFO));
	di.cbSize = sizeof (DOCINFO);
	di.lpszDocName = bitmapFile; 
	BOOL bPrintingOK = printer.StartDoc(&di); // Begin a new print job 
	
	// get printer Width and heght
	int maxw = printer.GetDeviceCaps(HORZRES);
	int maxh = printer.GetDeviceCaps(VERTRES); 
	//Info.m_rectDraw.SetRect(0, 0, maxw, maxh); 
	//for (UINT page = Info.GetMinPage(); page <= 
	//	Info.GetMaxPage() && bPrintingOK; page++) {
	printer.StartPage();    // begin new page
	//		Info.m_nCurPage = page;
	CBitmap bitmap;
	// LoadImage does the trick here, it creates a DIB section
	// You can also use a resource here
	// by using MAKEINTRESOURCE() ... etc. 
	HBITMAP hBmp = (HBITMAP)::LoadImage(NULL,bitmapFile, IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR | LR_LOADFROMFILE);
	if(!bitmap.IsNull())
		bitmap.DeleteObject();

	bitmap = hBmp;

	/*if(!bmpitmap.IsNull())
	m_bmp.GetSize(m_size);
	else
	m_size.cx = m_size.cy = 1; */
	/*	if(!bitmap.Attach(::LoadImage(
	::GetModuleHandle(NULL), bitmapFile, IMAGE_BITMAP, 0, 0, 
	LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE))) {
	AtlMessageBox(m_hWnd,_T("Error loading bitmap!")); return 0;
	}  */
	BITMAP bm;
	bitmap.GetBitmap(&bm);
	int w = bm.bmWidth; 
	int h = bm.bmHeight; 
	// create memory device context
	CDC memDC; 

	memDC.CreateCompatibleDC(printer.m_hDC);
	HBITMAP pbmp = memDC.SelectBitmap(bitmap);
	memDC.SetMapMode(printer.GetMapMode());
	printer.SetStretchBltMode(HALFTONE);
	// now stretchblt to maximum width on page
	printer.StretchBlt(0, 0, maxw, maxh, memDC, 0, 0, w, h, SRCCOPY); 
	// clean up
	memDC.SelectBitmap(pbmp);
	bPrintingOK = (printer.EndPage() > 0);   // end page
	//} 
	if (bPrintingOK)
		printer.EndDoc(); // end a print job
	else printer.AbortDoc();           // abort job. 

	return S_OK;
}
