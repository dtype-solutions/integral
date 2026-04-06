/*
 * Copyright (C) 1996-2026 D-Type Solutions
 * Web Site: http://www.d-type.com
 * E-mail: info@d-type.com
 */


// D-Type header file (level 3)
#include "dtype_l3.h"
// D-Type's platform independent window support
#include "dtwindow.h"
// D-Type's extra system utility functions
#include "dtxsystem.h"

// D-Type CPP Engine
#include "dtengine.cpp"


#define DV_SURFACE_BPP 4
#define DV_SURFACE_FORMAT (DV_SURFACE_BPP * 8)


class CDTApplet : public CDTWindow
{
public:

	CDTApplet(CDTEngineV10& engine, CDTDisplay& display, DT_FLOAT zoom = 1.0) : CDTWindow(&display), Engine(engine)
	{
		Init();
		Zoom *= zoom;
	}

	CDTApplet(CDTEngineV10& engine, CDTDisplay& display, const DT_CHAR* title, DT_SLONG w, DT_SLONG h, DT_ULONG flags = 0, DT_FLOAT zoom = 1.0) : CDTWindow(&display, title, w, h, flags), Engine(engine)
	{
		Init();
		Zoom *= zoom;
	}

	virtual ~CDTApplet()
	{
		Exit();
	}

	void DrawFrame()
	{
		DrawMain(); SetPixels(MemorySurface.m, 1);
	}

	bool Redraw;

private:

	bool Init();
	void Exit();

	void Reset()
	{
		Redraw = true;

		LeftMouseButton = false;
		MouseX = MouseY = 0;
		X = Y = 0;
		Zoom = GetDisplay()->GetScale();
		Page = 0;
	}

	bool InitDoc();

	bool DrawMain();
	void DrawBitmap(const DT_BMP& bmp, DT_SLONG x, DT_SLONG y, DT_UBYTE alpha);
	void LoadBitmap(DT_BMP& bmp, DT_SLONG w, DT_SLONG h, const DT_STREAM_DESC& sd);

private: // window callbacks

	void Event_KeyDown(DT_SLONG key);
	void Event_MouseButtonDown(DT_SLONG button, DT_SLONG x, DT_SLONG y);
	void Event_MouseMove(DT_SLONG x, DT_SLONG y);
	void Event_MouseButtonUp(DT_SLONG button, DT_SLONG x, DT_SLONG y);
	void Event_Resize(DT_SLONG w, DT_SLONG h);

private:

	bool LeftMouseButton;
	DT_SLONG MouseX, MouseY;
	DT_SLONG X, Y;
	DT_FLOAT Zoom;
	DT_SWORD Page;

	CDTDocV10 Doc;

	CDTEngineV10& Engine;

	DT_MDC MemorySurface;

	DT_BMP BitmapA;
	DT_BMP BitmapB;
};


bool CDTApplet::Init()
{
	Reset();

	MemorySurface.m = DV_NULL; MemorySurface.l = 0; MemorySurface.w = MemorySurface.h = 0;

	DT_STREAM_FILE(sa, "../../files/raw/base_180x165_8bpp.raw");  LoadBitmap(BitmapA, 180, 165, sa);
	DT_STREAM_FILE(sb, "../../files/raw/menu2_180x165_8bpp.raw"); LoadBitmap(BitmapB, 180, 165, sb);

	return InitDoc();
}


void CDTApplet::Exit()
{
	dtMemFree(MemorySurface.m);
	dtMemFree(BitmapA.m);
	dtMemFree(BitmapB.m);
}


bool CDTApplet::InitDoc()
{
	if (!Doc.Init(Engine)) return false;

	const DT_CHAR* integral_prog = R"/*_INTEGRAL_*/(

::my.include("integral/includes/integral/doc-base.i");
::my.include("integral/includes/integral/doc-list.i");
::my.include("integral/includes/integral/doc-pict.i");

function CDTDocumentBasic::Make(page_w, page_h)
{
	UpdateBaseFormat({FontSize = 100, Align = DE_ALIGN_JUSTIFY_A_LEFT});

	local t = Run()

	+ Pict({Src = "integral/assets/integral-hor.png", SizeScale = 1000, RunFormat = {Align = DE_ALIGN_CENTER /*, SkewHor = -12*/}, NewlineFlag = 2, BaseAlign = DE_BASEALIGN_MIDDLE})

	+ "Jim Olsen\rIntegral Systems Inc.\r810 Albert Lane\rToronto, Ontario, M3G 1T2"

	+ Gap({SpacingFactor = 1024})

	+ "January 12, 2019"

	+ Gap({SpacingFactor = 1024})

	+ "Patricia Smith\rPixel Solutions Ltd.\r128 Sheppard Street West, Suite 301\rVancouver, British Columbia, V5K 2C1"

	+ Gap({SpacingFactor = 1024})

	+ "Dear Patricia,"

	+ Gap()

	+ "It’s my pleasure to provide this letter of recommendation for Raul Payette as he seeks employment in software sales with your company. As Raul’s direct supervisor at Integral Systems, I’ve worked closely with him over the past four years. Raul showed excellent communication skills and consistently exceeded company quotas. On a personal level, Raul is charismatic and well-spoken. Both of these qualities have served him well in his role as software sales professional."

	+ Gap()

	+ "Due to his wife’s cross-country transfer, Raul is seeking employment in Vancouver. All of us at Integral Systems will be sad to see such a valuable and energetic individual go. Allow me to point out three examples of Raul’s strengths:"

	+ Gap()

	+ Bullet({Type = DE_BULLET_CIRC}) + {MarginLeft = 200, Text = "First, Raul brought great value to Integral Systems by consistently exceeding quotas and selling in the top 15% of his peers. Raul has an understanding of market trends and instincts for identifying and pursuing qualified leads. Not only does he provide stellar service to existing clients, but he’s established a number of new business-to-business and business-to-customer accounts during his time at Integral Systems."}

	+ Gap()

	+ Bullet({Type = DE_BULLET_CIRC}) + {MarginLeft = 200, Text = "Second, Raul has a very strong grasp of technology. He knows software inside and out and helps relay technical information to customers in an accessible way. He explains our products clearly to customers and makes the most of technological avenues to gain new leads and customize his sales pitch to clients. During his first year at Integral Systems, I observed him discuss in detail with a prospective client the specific ways that our software could boost the sales of their print business. By monitoring clients’ presence across our website and blog, he provides each one with personalized service to meet their needs. Raul makes the most of technology to find new prospects, provide modern solutions, and drive results."}

	+ Gap()

	+ Bullet({Type = DE_BULLET_CIRC}) + {MarginLeft = 200, Text = "Finally, Raul has proven himself to be a strong cultural fit here at Integral Systems. He’s always ready to lend a helping hand to his teammates and contributes during brainstorming sessions and meetings between the sales and product development teams. Last month, Raul co-led an enlightening meeting on content marketing and account management. Outside of official events, Raul also takes turns leading weekly cycling classes at our in-house gym. Raul is an active member of the Integral Systems team and supports his colleagues professionally and personally."}

	+ Gap()

	+ "In closing, I’d like to restate my strong support for Raul’s application to the role of software sales professional with Pixel Solutions. I’m confident that he will surpass your expectations in this new role. Raul is driven, self-confident, proactively helpful, and resourceful. I know he will continue to find success in sales. Please feel free to contact me with any other questions."

	+ Gap({SpacingFactor = 748})

	+ "Sincerely Yours,"

	+ Gap({SpacingFactor = 480})

	+ Pict({Src = "integral/assets/signature-jo.png", SizeScale = 1500, NewlineFlag = 2, BaseAlign = DE_BASEALIGN_MIDDLE}) + "Jim Olsen\rIntegral Systems Inc.";

	/* text flow on two pages */

	TextFlow(0, 2, 1, 0, 300, 300, page_w - 600, page_h - 600, t);

	return t;
}

local page_w = 5100, page_h = 6600;
local doc = CDTDocumentBasic("integral/fontmaps/stix-1.inf");
doc.Make(page_w, page_h);
doc.Cleanup();

)/*_INTEGRAL_*/";

	DT_SCRIPT_INTERPRETER ix;

	ixInstIni(&ix, Doc.GetPDDoc(), 0, 0); if (ix == DV_NULL) return false;
	ixInstExecViaMemory(ix, integral_prog, 0);
	ixInstExt(ix);

	return true;
}


bool CDTApplet::DrawMain()
{
	DT_UBYTE* m;
	DT_UBYTE clear_byte = 255;
	DT_SLONG w = GetW(); if (w < 1 || w > 16384) return false;
	DT_SLONG h = GetH(); if (h < 1 || h > 16384) return false;
	DT_SLONG l = DV_SURFACE_BPP * w * h;

	if (l <= MemorySurface.l) m = MemorySurface.m; else m = static_cast<DT_UBYTE*>(dtMemRealloc(MemorySurface.m, l));
	if (m == DV_NULL) return false;

	dtMemSet(m, clear_byte, l);

	MemorySurface.m = m;
	MemorySurface.l = l;
	MemorySurface.w = w;
#ifdef DEF_WIN32 /* flip the surface vertically */
	MemorySurface.h = h;
#else
	MemorySurface.h = -h;
#endif

	Doc.SetScale(Zoom);
	Doc.Draw(Page, X, Y, DV_SURFACE_FORMAT, 0, &MemorySurface, DV_NULL);

	DrawBitmap(BitmapA, w - BitmapA.w, h - BitmapA.h, 0);
	DrawBitmap(BitmapB, w - BitmapB.w, h - BitmapB.h, 255);

	return true;
}


void CDTApplet::DrawBitmap(const DT_BMP& bmp, DT_SLONG x, DT_SLONG y, DT_UBYTE alpha)
{
	DT_STYLE_EFFECTS style1 = {1, {0}, {alpha, alpha, alpha, 0}, DV_NULL};
	Engine.OutputSetStyleEffects(style1);
	Engine.BitmapDoOutput(bmp, x, y);
}


void CDTApplet::LoadBitmap(DT_BMP& bmp, DT_SLONG w, DT_SLONG h, const DT_STREAM_DESC& sd)
{
	DT_SLONG l = w * h;

	bmp.m = DV_NULL; bmp.l = 0; bmp.x = bmp.y = bmp.w = bmp.h = 0;
	bmp.m = static_cast<DT_UBYTE*>(dtMemAlloc(l)); if (bmp.m == DV_NULL) return;
	bmp.w = w;
	bmp.h = h;
	bmp.l = l;

	dtMemSet(bmp.m, 0, bmp.l);

	DT_DTSTREAM f = dtStreamOpen(&sd, DV_STREAM_OPEN_MUST); if (f == DV_NULL) return;
	dtStreamRead(f, bmp.m, bmp.l, 0);
	dtStreamClose(f, DV_STREAM_CLOSE_YES);
}


void CDTApplet::Event_KeyDown(DT_SLONG key)
{
	if (key == 109 || key == 108) /* - */
	{
		if (Zoom > 0.1) Zoom /= 1.2; else return;
	}
	else if (key == 107 || key == 61) /* + */
	{
		if (Zoom < 10.0) Zoom *= 1.2; else return;
	}
	else if (key == 37) /* LEFT */
	{
		X += 10;
	}
	else if (key == 39) /* RIGHT */
	{
		X -= 10;
	}
	else if (key == 38) /* UP */
	{
		Y += 10;
	}
	else if (key == 40) /* DOWN */
	{
		Y -= 10;
	}
	else if (key == 82) /* R */
	{
		Reset();
	}
	else if (key == 33) /* PageUp */
	{
		if (Page > 0) Page--; else return;
	}
	else if (key == 34) /* PageDown */
	{
		if (Page < 1) Page++; else return;
	}
	else return;

	Redraw = true;
}


void CDTApplet::Event_MouseButtonDown(DT_SLONG button, DT_SLONG x, DT_SLONG y)
{
	if (button == 0) LeftMouseButton = true;

	MouseX = x;
	MouseY = y;
}


void CDTApplet::Event_MouseMove(DT_SLONG x, DT_SLONG y)
{
	if (!LeftMouseButton) return;

	X += MouseX - x; MouseX = x;
	Y += MouseY - y; MouseY = y;

	Redraw = true;
}


void CDTApplet::Event_MouseButtonUp(DT_SLONG button, DT_SLONG x, DT_SLONG y)
{
	if (button == 0) LeftMouseButton = false;

	MouseX = x;
	MouseY = y;
}


void CDTApplet::Event_Resize(DT_SLONG /*w*/, DT_SLONG /*h*/)
{
	Redraw = true;
}
