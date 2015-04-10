
#ifndef __ZOOMSCRL_H__
#define __ZOOMSCRL_H__

//	 CZoomScrollImpl : zooming implementation on top of CScrollImpl
//
//
// Written by Alain Rist (ar@navpoch.com)
// Updated by Alain Rist for WTL 7.5.4291
// Copyright (c) 2004 Alain Rist.
//
// This file is NOT a part of the Windows Template Library.
// The code and information is provided "as-is" without
// warranty of any kind, either expressed or implied.
//
// This code may be used in compiled form in any way you desire. This
// file may be redistributed by any means PROVIDING it is 
// not sold for profit without the author written consent, and 
// providing that this notice and the author name is included. 
//
// Beware of bugs.
//

#pragma once

#ifndef __ATLMISC_H__
#error zoomscrl.h requires atlmisc.h to be included first.
#else
#ifdef _WTL_NO_WTYPES
#error CSize class support requires _WTL_NO_WTYPES to be undefined.
#endif // _WTL_NO_WTYPES
#endif // __ATLMISC_H__

#ifndef __ATLSCRL_H__
#error zoomscrl.h requires atlscrl.h to be included first.
#endif // __ATLSCRL_H__

//#define _WTL_NO_SIZE_SCALAR // remove this line when WTL supports CSize scalar operators

namespace WTL
{
	//  CSize scalar operators
	//  Should be part of CSize definition in WTL 7.5 release
#if (_WTL_VER <= 0x0710) || defined(_WTL_NO_SIZE_SCALAR)

	template < class Num >
	inline CSize operator * ( tagSIZE s, Num n) 
	{
		return CSize( (int)(s.cx * n), (int)(s.cy * n));
	};

	template < class Num >
	inline void operator *= ( tagSIZE & s, Num n)
	{
		s = s * n;
	};	

	template < class Num >
	inline CSize operator / ( tagSIZE s, Num n) 
	{
		return CSize( (int)(s.cx / n), (int)(s.cy / n));
	};

	template < class Num >
	inline void operator /= ( tagSIZE & s, Num n)
	{
		s = s / n;
	};	

#endif // (_WTL_VER <= 0x0710) || defined(_WTL_NO_SIZE_SCALAR)

	//////////////////////////////////

	//	Class CZoomScrollImpl

	template < class T >
	class  CZoomScrollImpl: public CScrollImpl<T>
	{
	public:
		// Creation
		CZoomScrollImpl() : m_sizeTrue( 0 ) , m_fzoom( 1. ) {} 

		// Data members
		CSize m_sizeTrue;
		double	m_fzoom;

		// Zoom operations and access
		void SetZScrollSize( CSize sizeTrue, double fzoom = 1., BOOL bRedraw = TRUE )
		{
			ATLASSERT( fzoom > 0. );
			m_sizeTrue = sizeTrue; 
			m_fzoom = fzoom;

			CScrollImpl<T>::SetScrollSize( sizeTrue / fzoom, bRedraw );
		}

		void SetZScrollSize( int cx, int cy, double fzoom=1., BOOL bRedraw = TRUE )
		{
			SetZScrollSize( CSize( cx, cy ), fzoom, bRedraw );
		}

		void SetZoom( double fzoom, BOOL bRedraw = TRUE )
		{
			CPoint ptCenter = WndtoTrue( m_sizeClient / 2 );
			CSize sizePage = GetScrollPage();
			CSize sizeLine = GetScrollLine();

			SetZScrollSize( GetScrollSize(), fzoom, bRedraw );

			SetScrollLine( sizeLine );
			SetScrollPage( sizePage );
			CPoint ptOffset = ptCenter - m_sizeClient * fzoom / 2;
			SetScrollOffset( ptOffset, bRedraw );
		}

		double GetZoom()
		{
			return m_fzoom;
		}
		// CScrollImpl overrides 

		void SetScrollOffset( int x, int y, BOOL bRedraw = TRUE )
		{
			CSize sizeMax = CSize(m_sizeAll) - m_sizeClient;
			x = max (min( (int)( x / m_fzoom), sizeMax.cx), 0 );
			y = max (min( (int)( y / m_fzoom), sizeMax.cy), 0 );

			CScrollImpl<T>::SetScrollOffset( x, y, bRedraw );
		}

		void SetScrollOffset( POINT ptOffset, BOOL bRedraw = TRUE )
		{
			SetScrollOffset( ptOffset.x, ptOffset.y , bRedraw );
		}

		void GetScrollOffset( POINT& ptOffset ) 
		{
			ptOffset = m_ptOffset * m_fzoom;
		}

		void SetScrollSize( int cx, int cy, BOOL bRedraw = TRUE )
		{
			SetZScrollSize( cx, cy, GetZoom(), bRedraw );
		}

		void SetScrollSize( SIZE sizeTrue, BOOL bRedraw = TRUE )
		{
			SetZScrollSize( sizeTrue, GetZoom(), bRedraw );
		}

		void GetScrollSize( SIZE& sizeTrue ) const
		{
			sizeTrue = m_sizeTrue;
		}

		void SetScrollPage(int cxPage, int cyPage)
		{
			SetScrollPage( CSize( cxPage, cyPage ));
		}

		void SetScrollPage(SIZE sizePage)
		{
			CScrollImpl<T>::SetScrollPage( sizePage / m_fzoom );
		}

		void GetScrollPage(SIZE& sizePage) const
		{
			sizePage = m_sizePage * m_fzoom;
		}

		void SetScrollLine(int cxLine, int cyLine)
		{
			SetScrollLine( CSize( cxLine, cyLine ));
		}

		void SetScrollLine(SIZE sizeLine)
		{
			CScrollImpl<T>::SetScrollLine( sizeLine / m_fzoom );
		}

		void GetScrollLine(SIZE& sizeLine) const
		{
			sizeLine = m_sizeLine * m_fzoom;
		}
		// Data access complements

		CSize GetScrollSize( ) 
		{
			return m_sizeTrue;
		}

		CSize GetScrollPage( ) 
		{
			return m_sizePage * m_fzoom;
		}

		CSize GetScrollLine( ) 
		{
			return m_sizeLine * m_fzoom;
		}

		CPoint GetScrollOffset() 
		{
			return (CSize)m_ptOffset * m_fzoom;
		}
		//	Helper coordinate functions

		CPoint WndtoTrue( CPoint ptW )
		{
			return (CSize)ptW * GetZoom() + GetScrollOffset();
		}

		void WndtoTrue( LPPOINT aptW, int nPts ) // in place coord transformation
		{
			for ( int i = 0 ; i < nPts ; i++ )
				aptW[i] = WndtoTrue( aptW[i] );
		}

		void WndtoTrue( LPRECT prectW ) // in place coord transformation
		{
			WndtoTrue( (LPPOINT)prectW, 2);
		}

		CPoint TruetoWnd( CPoint ptT )
		{
			return ( ptT - GetScrollOffset()) / GetZoom();
		}

		void TruetoWnd( LPPOINT aptT, int nPts ) // in place coord transformation
		{
			for ( int i = 0 ; i < nPts ; i++ )
				aptT[i] = TruetoWnd( aptT[i] );
		}

		void TruetoWnd( LPRECT prectT ) // in place coord transformation
		{
			TruetoWnd( (LPPOINT)prectT, 2);
		}
		// Drawing operations : assume adequate setting of data members 

		void DrawBitmap( HDC hdestDC, HBITMAP hbm)
		{// deprecated : params are (src, dest, ROP) everywhere
			DrawBitmap( hbm, hdestDC); 
		}

		void DrawBitmap( HBITMAP hbm, HDC hdestDC, DWORD dwROP = SRCCOPY)
		{
			CDC memDC = CreateCompatibleDC( hdestDC);
			CBitmapHandle bmpOld = memDC.SelectBitmap( hbm);
			DrawDC( memDC, hdestDC, dwROP);
			memDC.SelectBitmap( bmpOld);
		}

		void DrawDC( HDC hsourceDC, HDC hdestDC, DWORD dwROP = SRCCOPY)
		{
			CDCHandle destDC = hdestDC;
#ifndef _WIN32_WCE
			destDC.SetMapMode(MM_TEXT);
#endif // _WIN32_WCE
			destDC.SetViewportOrg(0,0);
			CPoint ptOffset = GetScrollOffset();
			CSize sizeZClient = m_sizeClient * GetZoom();
			destDC.StretchBlt( 0, 0, m_sizeClient.cx, m_sizeClient.cy, 
				hsourceDC, ptOffset.x, ptOffset.y, sizeZClient.cx, sizeZClient.cy, dwROP);
		}

		// Message map and handlers
		BEGIN_MSG_MAP(CZoomScrollImpl<T>)
			MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBkgnd)
#if _WTL_VER <= 0x0710
			MESSAGE_HANDLER(WM_SIZE, OnSize)
#endif //	_WTL_VER <= 0x0710
			CHAIN_MSG_MAP(CScrollImpl<T>)
		END_MSG_MAP()

		LRESULT OnEraseBkgnd(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled)
		{
			T* pT = static_cast<T*>(this);
			ATLASSERT( ::IsWindow( pT->m_hWnd ));
			if (( GetScrollExtendedStyle() & SCRL_ERASEBACKGROUND))
			{
				CRect rect;
				pT->GetClientRect(rect);
				CSize sizeClient=rect.Size();

				if ( m_sizeAll.cx < sizeClient.cx || m_sizeAll.cy < sizeClient.cy )
				{
					CDCHandle hdc = (HDC)wParam;
					HBRUSH hbr = GetSysColorBrush( (int)T::GetWndClassInfo().m_wc.hbrBackground - 1 );
					CRect rectBG( CPoint( 0 ), sizeClient );

					if ( m_sizeAll.cx < sizeClient.cx )
					{
						rectBG.left = m_sizeAll.cx;
						hdc.FillRect( rectBG, hbr );
					}
					if ( m_sizeAll.cy < sizeClient.cy ) 
					{
						rectBG.left = 0;
						rectBG.top = m_sizeAll.cy;
						hdc.FillRect( rectBG, hbr );
					}
				}
			}
			else 
				bHandled = FALSE;
			return 1;
		}

#if _WTL_VER <= 0x0710
		LRESULT OnSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
		{
			T* pT = static_cast<T*>(this);
			ATLASSERT( ::IsWindow(pT->m_hWnd));

			CSize sizeClient( GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) );
			CRect rect( CPoint(0), sizeClient);
			SCROLLINFO si = { sizeof(si), SIF_PAGE | SIF_POS };

			if  ( m_sizeClient.cx != sizeClient.cx )
			{
				rect.left = min( m_sizeClient.cx, sizeClient.cx);
				si.nPage = m_sizeClient.cx = sizeClient.cx ;
				si.nPos = m_ptOffset.x;
				pT->SetScrollInfo( SB_HORZ, &si, FALSE);
				pT->InvalidateRect( rect);
				rect.left = 0;
			}

			if  ( m_sizeClient.cy != sizeClient.cy )
			{
				rect.top = min( m_sizeClient.cy, sizeClient.cy);
				si.nPage = m_sizeClient.cy = sizeClient.cy ;
				si.nPos = m_ptOffset.y;
				pT->SetScrollInfo(SB_VERT, &si, FALSE);
				pT->InvalidateRect( rect);
			}

			CSize sizeMax = CSize(m_sizeAll) - m_sizeClient;
			int x = max (min( m_ptOffset.x, sizeMax.cx), 0 );
			int y = max (min( m_ptOffset.y, sizeMax.cy), 0 );

			if ( CPoint( x, y) != m_ptOffset )
			{
				pT->ScrollWindowEx(m_ptOffset.x - x, m_ptOffset.y - y, m_uScrollFlags);
				CScrollImpl<T>::SetScrollOffset(x, y, FALSE);
			}
			return 0;
		}
#endif //	_WTL_VER <= 0x0710
	};

}	// namespace WTL

#endif // __ZOOMSCRL_H__