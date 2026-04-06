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

function CDTDocumentBasic::Make()
{
	/* Lambda shortcuts */
	local o = @(label = "") CDTObj(this, label); /* to make object */
	local p = @(id, str, len = PD_DEFAULT) CDTProp(id, str, len); /* to add property */
	local r = @(id, obj, id2) CDTPropRef(id, obj, id2); /* to add property reference */
	local l = @(id, obj) CDTLink(id, obj); /* to add link */

	// Affine Transformation Matrix
	local transform = [1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0];

	// PART 1: Defining objects and their properties
	// 110 PowerDoc Object(s)
	local obj = array(110);

	obj[0] = o("LibShape");
	obj[1] = o("Line Width");
	obj[2] = o("LibShape");
	obj[3] = o("Color");
	obj[4] = o("TextLine Body Color");
	obj[5] = o("TextLine Typography");
	obj[6] = o("TextLine");
	obj[7] = o("Text Layout");
	obj[8] = o("Rect");
	obj[9] = o("RichTextArea");
	obj[10] = o("TextLine Body Color");
	obj[11] = o("Ellipse Outline");
	obj[12] = o("Ellipse");
	obj[13] = o("Line");
	obj[14] = o("Char Typography");
	obj[15] = o("Char");
	obj[16] = o("LibShape");
	obj[17] = o("Color");
	obj[18] = o("Rect");
	obj[19] = o("Ellipse");
	obj[20] = o("Ellipse");
	obj[21] = o("Ellipse Body Color");
	obj[22] = o("Ellipse");
	obj[23] = o("Ellipse");
	obj[24] = o("RichTextArea");
	obj[25] = o("Rect");
	obj[26] = o("Line Width");
	obj[27] = o("Line");
	obj[28] = o("Ellipse Outline");
	obj[29] = o("Line");
	obj[30] = o("LibShape");
	obj[31] = o("Line Width");
	obj[32] = o("Char Typography");
	obj[33] = o("Char");
	obj[34] = o("Rect");
	obj[35] = o("RichTextArea");
	obj[36] = o("Color");
	obj[37] = o("Ellipse Outline");
	obj[38] = o("Ellipse");
	obj[39] = o("Line Width");
	obj[40] = o("Line");
	obj[41] = o("Group");
	obj[42] = o("Group Member");
	obj[43] = o("Group Member");
	obj[44] = o("Group Member");
	obj[45] = o("Group");
	obj[46] = o("Group Member");
	obj[47] = o("Group Member");
	obj[48] = o("Group Member");
	obj[49] = o("Group");
	obj[50] = o("Group Member");
	obj[51] = o("Group Member");
	obj[52] = o("Group Member");
	obj[53] = o("Group Member");
	obj[54] = o("Group Member");
	obj[55] = o("Group");
	obj[56] = o("Group Member");
	obj[57] = o("Group Member");
	obj[58] = o("Group Member");
	obj[59] = o("Group");
	obj[60] = o("Group Member");
	obj[61] = o("Group Member");
	obj[62] = o("Group Member");
	obj[63] = o("Group");
	obj[64] = o("Group Member");
	obj[65] = o("Group Member");
	obj[66] = o("Group");
	obj[67] = o("Group Member");
	obj[68] = o("Group Member");
	obj[69] = o("Group Member");
	obj[70] = o("Group Member");
	obj[71] = o("Group Member");
	obj[72] = o("Arc Line Width");
	obj[73] = o("Poly");
	obj[74] = o("Group");
	obj[75] = o("Group Member");
	obj[76] = o("Group Member");
	obj[77] = o("TextLine Typography");
	obj[78] = o("TextLine Typography");
	obj[79] = o("TextLine");
	obj[80] = o("Text Layout");
	obj[81] = o("TextLine");
	obj[82] = o("RichTextArea Typography");
	obj[83] = o("RichTextArea Typography");
	obj[84] = o("RichTextArea Typography");
	obj[85] = o("Text Layout");
	obj[86] = o("Text Layout");
	obj[87] = o("Text Fragment");
	obj[88] = o("RichTextArea Typography");
	obj[89] = o("Text Fragment");
	obj[90] = o("RichTextArea Typography");
	obj[91] = o("Text Layout");
	obj[92] = o("Text Fragment");
	obj[93] = o("Text Fragment");
	obj[94] = o("Text Fragment");
	obj[95] = o("Text Fragment");
	obj[96] = o("Text Fragment");
	obj[97] = o("RichTextArea Typography");
	obj[98] = o("Text Fragment");
	obj[99] = o("Text Fragment");
	obj[100] = o("Text Fragment");
	obj[101] = o("RichTextArea Typography");
	obj[102] = o("Text Layout");
	obj[103] = o("Color");
	obj[104] = o("Ellipse Outline");
	obj[105] = o("Line");
	obj[106] = o("Group");
	obj[107] = o("Group Member");
	obj[108] = o("Group Member");
	obj[109] = o("Group Member");

	obj[0] + l(pdOutlinePtr, obj[1]) + p(pdDw, "300") + p(pdDh, "300") + p(pdID, "56");

	obj[1] + l(pdStylePtr, obj[10]) + p(pdLineThickness, "400");

	obj[2] + l(pdBodyPtr, obj[3]) + p(pdID, "56") + p(pdDw, "270") + p(pdDh, "270");

	obj[3] + p(pdRGBT, "FF004000");

	obj[4] + p(pdRGBT, "FFFFFF00");

	obj[5] + l(pdBodyPtr, obj[4]) + p(pdFontDw, "95") + p(pdFontDh, "130") + p(pdFontUniqueID, "F0010_HSANSS_cS0");

	obj[6] + l(pdTypographyPtr, obj[5]) + l(pdTextLayoutPtr, obj[7]) + p(pdAngleRotation, "0") + p(pdTextString_08, "STOP");

	obj[7] + p(pdKerning, "1");

	obj[8] + l(pdOutlinePtr, obj[1]) + p(pdDh, "300") + p(pdRoundX, "20") + p(pdDw, "260");

	obj[9] + l(pdTextFragmentPtr, obj[79]) + p(pdTextAreaDw, "260") + p(pdTextAreaDh, "320");

	obj[10] + p(pdRGBT, "00000000");

	obj[11] + l(pdStylePtr, obj[3]) + p(pdLineThickness, "2500");

	obj[12] + l(pdOutlinePtr, obj[11]) + p(pdDw, "300") + p(pdDh, "300");

	obj[13] + l(pdLinePtr, obj[11]) + p(pdDw, "200") + p(pdDh, "200");

	obj[14] + l(pdBodyPtr, obj[10]) + p(pdSkewHor, "0") + p(pdSkewVer, "0") + p(pdRotation, "0") + p(pdUnderline, "0") + p(pdFontDw, "270") + p(pdFontDh, "260") + p(pdFontUniqueID, "F0010_HSANSS_cS0");

	obj[15] + l(pdTypographyPtr, obj[14]) + p(pdID, "80");

	obj[16] + l(pdBodyPtr, obj[17]) + l(pdOutlinePtr, obj[1]) + p(pdDw, "300") + p(pdDh, "300") + p(pdID, "50");

	obj[17] + p(pdRGBT, "FFE80000");

	obj[18] + l(pdBodyPtr, obj[10]) + p(pdDw, "80") + p(pdDh, "205");

	obj[19] + l(pdBodyPtr, obj[3]) + p(pdDw, "60") + p(pdDh, "60");

	obj[20] + l(pdBodyPtr, obj[17]) + p(pdDw, "60") + p(pdDh, "60");

	obj[21] + p(pdRGBT, "00BB0000");

	obj[22] + l(pdBodyPtr, obj[21]) + p(pdDw, "60") + p(pdDh, "60");

	obj[23] + l(pdBodyPtr, obj[3]) + p(pdDw, "300") + p(pdDh, "300");

	obj[24] + l(pdTextFragmentPtr, obj[100]) + p(pdTextAreaDw, "300") + p(pdTextAreaDh, "200");

	obj[25] + l(pdBodyPtr, obj[4]) + p(pdDh, "40") + p(pdDw, "250");

	obj[26] + l(pdStylePtr, obj[10]) + p(pdLineStart, "100") + p(pdLineStartTheta, "38") + p(pdLineStartSize, "40") + p(pdLineThickness, "3000");

	obj[27] + l(pdLinePtr, obj[26]) + p(pdDw, "0") + p(pdDh, "180");

	obj[28] + l(pdStylePtr, obj[10]) + p(pdLineThickness, "2500");

	obj[29] + l(pdLinePtr, obj[28]) + p(pdDh, "70") + p(pdDw, "50");

	obj[30] + l(pdOutlinePtr, obj[31]) + p(pdID, "1") + p(pdDw, "350") + p(pdDh, "290");

	obj[31] + l(pdStylePtr, obj[3]) + p(pdLineThickness, "3400");

	obj[32] + l(pdBodyPtr, obj[10]) + p(pdSkewHor, "0") + p(pdSkewVer, "0") + p(pdRotation, "0") + p(pdUnderline, "0") + p(pdFontDw, "270") + p(pdFontDh, "260") + p(pdFontUniqueID, "F0011_TEX10__SS0");

	obj[33] + l(pdTypographyPtr, obj[32]) + p(pdID, "33");

	obj[34] + l(pdOutlinePtr, obj[1]) + p(pdDh, "300") + p(pdRoundX, "20") + p(pdDw, "240");

	obj[35] + l(pdTextFragmentPtr, obj[87]) + p(pdTextAreaDh, "290") + p(pdTextAreaDw, "240");

	obj[36] + p(pdRGBT, "00997700");

	obj[37] + l(pdStylePtr, obj[36]) + p(pdLineThickness, "2000");

	obj[38] + l(pdOutlinePtr, obj[37]) + p(pdDw, "150") + p(pdDh, "150");

	obj[39] + l(pdStylePtr, obj[10]) + p(pdLineStart, "100") + p(pdLineStartTheta, "38") + p(pdLineThickness, "900") + p(pdLineStartSize, "65");

	obj[40] + l(pdLinePtr, obj[39]) + p(pdDh, "0") + p(pdDw, "33");

	obj[41] + l(pdGroupMemberPtr, obj[42]);

	obj[42] + l(pdFnDrawObjectPtr, obj[0]) + l(pdGroupMemberPtr, obj[43]) + p(pdFnDraw, "18") + p(pdDy, "-10");

	obj[43] + l(pdFnDrawObjectPtr, obj[2]) + l(pdGroupMemberPtr, obj[44]) + p(pdFnDraw, "18") + p(pdDx, "15") + p(pdDy, "5");

	obj[44] + l(pdFnDrawObjectPtr, obj[6]) + p(pdFnDraw, "1") + p(pdDx, "30") + p(pdDy, "188");

	obj[45] + l(pdGroupMemberPtr, obj[46]);

	obj[46] + l(pdFnDrawObjectPtr, obj[8]) + l(pdGroupMemberPtr, obj[47]) + p(pdFnDraw, "6") + p(pdDy, "-10");

	obj[47] + l(pdFnDrawObjectPtr, obj[9]) + p(pdFnDraw, "10") + p(pdDy, "3");

	obj[48] + l(pdFnDrawObjectPtr, obj[13]) + p(pdFnDraw, "4") + p(pdDx, "35") + p(pdDy, "50");

	obj[49] + l(pdGroupMemberPtr, obj[50]);

	obj[50] + l(pdFnDrawObjectPtr, obj[34]) + l(pdGroupMemberPtr, obj[51]) + p(pdFnDraw, "6") + p(pdDy, "-10");

	obj[51] + l(pdFnDrawObjectPtr, obj[35]) + l(pdGroupMemberPtr, obj[52]) + p(pdFnDraw, "10") + p(pdDy, "17");

	obj[52] + l(pdFnDrawObjectPtr, obj[38]) + l(pdGroupMemberPtr, obj[53]) + p(pdFnDraw, "5") + p(pdDx, "45");

	obj[53] + l(pdFnDrawObjectPtr, obj[40]) + l(pdGroupMemberPtr, obj[54]) + p(pdFnDraw, "4") + p(pdDx, "22") + p(pdDy, "267");

	obj[54] + l(pdFnDrawObjectPtr, obj[40]) + p(pdFnDraw, "4") + p(pdTransformMatrix, "-262144, 0, 0, 262144") + p(pdDx, "218") + p(pdDy, "267");

	obj[55] + l(pdGroupMemberPtr, obj[56]);

	obj[56] + l(pdFnDrawObjectPtr, obj[23]) + l(pdGroupMemberPtr, obj[57]) + p(pdFnDraw, "5") + p(pdDy, "-10");

	obj[57] + l(pdFnDrawObjectPtr, obj[24]) + l(pdGroupMemberPtr, obj[58]) + p(pdFnDraw, "10") + p(pdDy, "61");

	obj[58] + l(pdFnDrawObjectPtr, obj[25]) + p(pdFnDraw, "6") + p(pdDx, "25") + p(pdDy, "120");

	obj[59] + l(pdGroupMemberPtr, obj[60]);

	obj[60] + l(pdFnDrawObjectPtr, obj[16]) + l(pdGroupMemberPtr, obj[61]) + p(pdFnDraw, "18") + p(pdDy, "-10");

	obj[61] + l(pdFnDrawObjectPtr, obj[27]) + l(pdGroupMemberPtr, obj[62]) + p(pdFnDraw, "4") + p(pdDx, "150") + p(pdDy, "56");

	obj[62] + l(pdFnDrawObjectPtr, obj[29]) + p(pdFnDraw, "4") + p(pdDx, "150") + p(pdDy, "147");

	obj[63] + l(pdGroupMemberPtr, obj[64]);

	obj[64] + l(pdFnDrawObjectPtr, obj[30]) + l(pdGroupMemberPtr, obj[65]) + p(pdFnDraw, "18") + p(pdDy, "-10");

	obj[65] + l(pdFnDrawObjectPtr, obj[33]) + p(pdFnDraw, "16") + p(pdDx, "107") + p(pdDy, "247");

	obj[66] + l(pdGroupMemberPtr, obj[67]);

	obj[67] + l(pdFnDrawObjectPtr, obj[16]) + l(pdGroupMemberPtr, obj[68]) + p(pdFnDraw, "18") + p(pdDy, "-10");

	obj[68] + l(pdFnDrawObjectPtr, obj[18]) + l(pdGroupMemberPtr, obj[69]) + p(pdFnDraw, "6") + p(pdDx, "110") + p(pdDy, "37");

	obj[69] + l(pdFnDrawObjectPtr, obj[19]) + l(pdGroupMemberPtr, obj[70]) + p(pdFnDraw, "5") + p(pdDx, "120") + p(pdDy, "45");

	obj[70] + l(pdFnDrawObjectPtr, obj[20]) + l(pdGroupMemberPtr, obj[71]) + p(pdFnDraw, "5") + p(pdDx, "120") + p(pdDy, "110");

	obj[71] + l(pdFnDrawObjectPtr, obj[22]) + p(pdFnDraw, "5") + p(pdDx, "120") + p(pdDy, "175");

	obj[72] + l(pdStylePtr, obj[10]) + p(pdLineThickness, "3500");

	obj[73] + l(pdLinePtr, obj[72]) + p(pdPolyArrI, "17, 20, 24, 0, 0, 20, 17, 20, 20") + p(pdPolyArrX_32, "106, 106, 106, 0, 0, 0, -40, 0, 40") + p(pdPolyArrY_32, "0, -140, -220, -220, -140, -20, -70, -20, -70");

	obj[74] + l(pdGroupMemberPtr, obj[75]);

	obj[75] + l(pdFnDrawObjectPtr, obj[12]) + l(pdGroupMemberPtr, obj[76]) + p(pdFnDraw, "5") + p(pdDy, "-10");

	obj[76] + l(pdFnDrawObjectPtr, obj[73]) + l(pdGroupMemberPtr, obj[48]) + p(pdFnDraw, "24") + p(pdDx, "98") + p(pdDy, "238");

	obj[77] + l(pdBodyPtr, obj[10]) + p(pdFontDw, "70") + p(pdFontDh, "70") + p(pdMMContributions, "320, 409, 0, 295") + p(pdFontUniqueID, "F0010_HSANSS_cS0");

	obj[78] + l(pdBodyPtr, obj[10]) + p(pdFontDw, "150") + p(pdFontDh, "150") + p(pdMMContributions, "320, 409, 0, 295") + p(pdFontUniqueID, "F0010_HSANSS_cS0");

	obj[79] + l(pdTextFragmentPtr, obj[81]) + l(pdTypographyPtr, obj[77]) + l(pdTextLayoutPtr, obj[80]) + p(pdAngleRotation, "0") + p(pdTextString_08, "SPEED\x0DLIMIT\x0D");

	obj[80] + p(pdSpacingRow, "-15") + p(pdKerning, "1") + p(pdAlign, "1") + p(pdAlignEnd, "1");

	obj[81] + l(pdTypographyPtr, obj[78]) + l(pdTextLayoutPtr, obj[80]) + p(pdAngleRotation, "0") + p(pdTextString_08, "75");

	obj[82] + l(pdBodyPtr, obj[10]) + p(pdMMContributions, "320, 409, 0, 295") + p(pdFontDw, "22") + p(pdFontDh, "28") + p(pdFontUniqueID, "F0010_HSANSS_cS0");

	obj[83] + l(pdBodyPtr, obj[10]) + p(pdFontDw, "35") + p(pdFontDh, "45") + p(pdMMContributions, "320, 409, 0, 295") + p(pdFontUniqueID, "F0010_HSANSS_cS0");

	obj[84] + l(pdBodyPtr, obj[10]) + p(pdMMContributions, "320, 409, 0, 295") + p(pdFontDw, "50") + p(pdFontDh, "55") + p(pdFontUniqueID, "F0010_HSANSS_cS0");

	obj[85] + p(pdSpacingRow, "-5") + p(pdKerning, "3") + p(pdAlign, "1") + p(pdAlignEnd, "1");

	obj[86] + p(pdSpacingRow, "5") + p(pdKerning, "3") + p(pdAlign, "1") + p(pdAlignEnd, "1");

	obj[87] + l(pdTextFragmentPtr, obj[89]) + l(pdTextLayoutPtr, obj[86]) + l(pdTypographyPtr, obj[88]) + p(pdTextString_08, "P");

	obj[88] + l(pdBodyPtr, obj[10]) + p(pdFontDw, "115") + p(pdFontDh, "110") + p(pdFontUniqueID, "F0010_HSANSS_cS0");

	obj[89] + l(pdTextLayoutPtr, obj[86]) + l(pdTextFragmentPtr, obj[96]) + l(pdTypographyPtr, obj[90]) + p(pdTextString_08, "\x0D");

	obj[90] + l(pdBodyPtr, obj[10]) + p(pdMMContributions, "320, 409, 0, 295") + p(pdFontDw, "18") + p(pdFontDh, "18") + p(pdFontUniqueID, "F0010_HSANSS_cS0");

	obj[91] + p(pdSpacingRow, "-11") + p(pdKerning, "3") + p(pdAlign, "1") + p(pdAlignEnd, "1");

	obj[92] + l(pdTextFragmentPtr, obj[95]) + l(pdTextLayoutPtr, obj[91]) + l(pdTypographyPtr, obj[83]) + p(pdTextString_08, "AM-");

	obj[93] + l(pdTextFragmentPtr, obj[94]) + l(pdTextLayoutPtr, obj[91]) + l(pdTypographyPtr, obj[83]) + p(pdTextString_08, "PM");

	obj[94] + l(pdTextLayoutPtr, obj[91]) + l(pdTextFragmentPtr, obj[99]) + l(pdTypographyPtr, obj[82]) + p(pdTextString_08, "\x0D");

	obj[95] + l(pdTextFragmentPtr, obj[93]) + l(pdTextLayoutPtr, obj[91]) + l(pdTypographyPtr, obj[84]) + p(pdTextString_08, "6");

	obj[96] + l(pdTextFragmentPtr, obj[98]) + l(pdTextLayoutPtr, obj[91]) + l(pdTypographyPtr, obj[97]) + p(pdTextString_08, "30 M\x0D");

	obj[97] + l(pdBodyPtr, obj[10]) + p(pdFontDw, "50") + p(pdFontDh, "55") + p(pdFontUniqueID, "F0010_HSANSS_cS0");

	obj[98] + l(pdTextFragmentPtr, obj[92]) + l(pdTextLayoutPtr, obj[91]) + l(pdTypographyPtr, obj[84]) + p(pdTextString_08, "9");

	obj[99] + l(pdTextLayoutPtr, obj[85]) + l(pdTypographyPtr, obj[82]) + p(pdTextString_08, "MON-FRI");

	obj[100] + l(pdTypographyPtr, obj[101]) + l(pdTextLayoutPtr, obj[102]) + p(pdTextString_08, "DO NOT\x0D\x0DENTER");

	obj[101] + l(pdBodyPtr, obj[4]) + p(pdFontDw, "48") + p(pdFontDh, "48") + p(pdFontUniqueID, "F0010_HSANSS_cS0");

	obj[102] + p(pdSpacingRow, "2") + p(pdKerning, "1") + p(pdAlign, "1") + p(pdAlignEnd, "1");

	obj[103] + p(pdRGBT, "FF004000");

	obj[104] + l(pdStylePtr, obj[103]) + p(pdLineThickness, "2500");

	obj[105] + l(pdLinePtr, obj[104]) + p(pdDw, "200") + p(pdDh, "200");

	obj[106] + l(pdGroupMemberPtr, obj[107]);

	obj[107] + l(pdFnDrawObjectPtr, obj[12]) + l(pdGroupMemberPtr, obj[108]) + p(pdFnDraw, "5") + p(pdDy, "-10");

	obj[108] + l(pdFnDrawObjectPtr, obj[15]) + l(pdGroupMemberPtr, obj[109]) + p(pdFnDraw, "16") + p(pdDx, "73") + p(pdDy, "236");

	obj[109] + l(pdFnDrawObjectPtr, obj[105]) + p(pdFnDraw, "4") + p(pdDx, "35") + p(pdDy, "50");

	// PART 2: Image placement
	// 9 PowerDoc Image(s)
	obj[41].Pin(0, 40, 40, pdDrawGroup);
	obj[45].Pin(0, 440, 40, pdDrawGroup);
	obj[49].Pin(0, 440, 430, pdDrawGroup);
	obj[55].Pin(0, 840, 430, pdDrawGroup);
	obj[59].Pin(0, 40, 820, pdDrawGroup);
	obj[63].Pin(0, 440, 820, pdDrawGroup);
	obj[66].Pin(0, 840, 820, pdDrawGroup);
	obj[74].Pin(0, 840, 40, pdDrawGroup);
	obj[106].Pin(0, 40, 430, pdDrawGroup);
}


local doc = CDTDocumentBasic();
doc.Make();
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
