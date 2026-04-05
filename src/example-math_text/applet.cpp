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

	CDTApplet(CDTEngine& engine, CDTDisplay& display, DT_FLOAT zoom = 1.0) : CDTWindow(&display), Engine(engine)
	{
		Init();
		Zoom *= zoom;
	}

	CDTApplet(CDTEngine& engine, CDTDisplay& display, const DT_CHAR* title, DT_SLONG w, DT_SLONG h, DT_ULONG flags = 0, DT_FLOAT zoom = 1.0) : CDTWindow(&display, title, w, h, flags), Engine(engine)
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

	CDTDoc Doc;

	CDTEngine& Engine;

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
::my.include("integral/includes/integral/doc-table.i");
::my.include("integral/includes/integral/doc-math.i");

function CDTDocumentBasic::Formula_Pi(label)
{
	/* formula example from http://functions.wolfram.com/Constants/Pi/09/0014 */

	//local old_base_format = GetBaseFormat(1);

	local old_base_format = UpdateBaseFormat({Align = DE_ALIGN_CENTER}, -1 /* RETURN CLONED FORMAT BEFORE UPDATE */);

	local r = Run() + label + "\r" + Pi + Eq + LimitInf("n") + Sp + Frac(Sup(2, "n" + Sum + 1), Run() + 2 + Diff + Sub("b", 1)) + Mul + Brack(/* Run() WAS NEEDED BEFORE TO ENSURE FONT SIZE IS 100 + */ Frac(Sub("b", "n"), 2) + Mul + Sqrt(Run() + 2 + Sum + Sub("b", "n" + Diff + 1) + Mul + Sqrt(Run() + 2 + Sum + Sub("b", "n" + Diff + 2) + Mul + Sqrt(Run() + 2 + Sum + Ellipsis + Sum + Sub("b", 2) + Mul + Sqrt(Run() + 2 + Sum + "sin" + Brack(Frac(Run() + Pi + Mul + Sub("b", 1), 4))))))) + Sp + "/;" + Br() + Sub("b", "n") + Eq + 1 + And + Sub("b", "n" + Diff + 1) + Eq + -1 + And + "(" + Sub("b", "k") + Eq + 1 + And + 2 + LTEq + "k" + LTEq + "n" + Diff + 2 + And + "k" + Element + Integers + ")" + And + Sub("b", 1) + Element + Reals + And + -2 + LTEq + Sub("b", 1) + LTEq + 2;

	RestoreBaseFormat(old_base_format);

	//BaseFormat.clear();
	//UpdateBaseFormat(old_base_format);

	//UpdateBaseFormat({Align = DE_ALIGN_LEFT});

	return r;

	//return r * {Align = DE_ALIGN_CENTER};
}

function CDTDocumentBasic::Make(page_w, page_h)
{
	local label1 = "Formula A: ";
	local label2 = "Formula B: ";

	SetMathParams({Autoformat = 1, Placeholders = 0});
	UpdateBaseFormat({FontSize = 122});

	local t = Run()

	+ "Hello, here is my formula:"

	+ Gap()

	+ Formula_Pi(label1)

	+ Gap()

	+ "And here is my formula once again:"

	+ Gap()

	+ Formula_Pi(label2)

	+ Gap()

	+ "Mathematical notation includes " + {Text = "letters from various alphabets", Bold = 1} + ", as well as " + {Text = "special mathematical and scientific symbols", Underline = 1} + ". Letters in various fonts often have specific, fixed meanings in particular areas of mathematics. In mathematics, a formula is an entity constructed using the symbols and formation rules of a given logical language. A mathematical formula such as " + Root(3, "x + y") + " contains two variables, x and y. Other formulas may be created to solve a particular problem: for example, using the equation of a sine curve to model the movement of the tides in a bay: sin" + Brack(Frac("a - b", "x + y")) + ". In all cases, however, formulas form the basis for calculations. Expressions are distinct from formulas in that they cannot contain an equals sign. When comparing formulas to grammatical sentences, expressions are more like phrases."

	+ Gap()

	+ "In a general context, mathematical formulas are applied to provide a mathematical solution for real world problems. Some formulas may be rather complex to typeset, for example:"

	+ Gap()

	+ Frac(Root(3, "u + v"), Run() + Pi + Sum + Integral("cos(x) dx", "a", "b") + Diff + Root(4, Frac("a - b", "x + y")), {NewlineFlag = 0, RunFormat = {Align = DE_ALIGN_CENTER}})

	+ Gap()

	+ "The vast majority of computations with measurements are done in computer programs with no facility for retaining a symbolic computation of the units. Only the numerical quantity is used in the computation. This requires that the universal formula be converted to a formula that is intended to be used only with prescribed units, meaning the numerical quantity is implicitly assumed to be multiplying a particular unit. The requirements about the prescribed units must be given to users of the input and the output of the formula.";

	//t * {Align = DE_ALIGN_CENTER};
	//t * {SkewHor = 12};
	//t * {FontHeight = 200};
	//t * {FontSize = 70};

	/* text flow on two pages */

	TextFlow(0, 2, 1, 0, 100, 100, page_w - 200, page_h - 200, t);
}

local page_w = 5100, page_h = 3600;
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
